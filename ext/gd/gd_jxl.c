/*
   +----------------------------------------------------------------------+
   | Copyright © The PHP Group and Contributors.                          |
   +----------------------------------------------------------------------+
   | This source file is subject to the Modified BSD License that is      |
   | bundled with this package in the file LICENSE, and is available      |
   | through the World Wide Web at https://www.php.net/license/bsd-3-clause. |
   |                                                                      |
   | SPDX-License-Identifier: BSD-3-Clause                                |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_exceptions.h"
#include "php_gd.h"
#include "gd_codec_write.h"
#include "gd_jxl.h"
#include "gd_metadata.h"
#include "ext/spl/spl_exceptions.h"
#include <stdint.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_jxl_arginfo.h"

static zend_class_entry *php_gd_codec_exception_ce;

#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API) || defined(HAVE_GD_JXL_ANIM_WRITE_API)
static void php_gd_jxl_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_jxl_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_jxl_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_jxl_throw("Failed to open JXL input");
		return false;
	}
	*bytes = php_gd_jxl_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_jxl_throw("Failed to read JXL input");
		return false;
	}
	return true;
}

static bool php_gd_jxl_read_stream_bytes(zval *stream_zv, zend_string **bytes)
{
	php_stream *stream;

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		return false;
	}
	php_stream_from_zval_no_verify(stream, stream_zv);
	if (stream == NULL) {
		zend_argument_type_error(1, "must be a valid stream resource");
		return false;
	}
	*bytes = php_gd_jxl_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_jxl_throw("Failed to read JXL input");
		return false;
	}
	return true;
}

static bool php_gd_jxl_validate_encode_options(bool lossless, double distance, zend_long effort,
		uint32_t distance_arg, uint32_t effort_arg)
{
	if (distance < 0.0 || distance > 25.0) {
		zend_argument_value_error(distance_arg, "must be between 0 and 25");
		return false;
	}
	if (effort < 1 || effort > 9) {
		zend_argument_value_error(effort_arg, "must be between 1 and 9");
		return false;
	}
	(void) lossless;
	return true;
}
#endif

#ifdef HAVE_GD_JXL_CODEC
static zend_class_entry *php_gd_jxl_write_options_ce;

static void php_gd_jxl_read_write_options(zval *options_zv, gdJxlWriteOptions *options)
{
	zval rv;
	zval *value;

	gdJxlWriteOptionsInit(options);
	if (options_zv == NULL) {
		return;
	}

	value = zend_read_property(php_gd_jxl_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("lossless"), true, &rv);
	options->lossless = Z_TYPE_P(value) == IS_TRUE;
	value = zend_read_property(php_gd_jxl_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("distance"), true, &rv);
	options->distance = (float) Z_DVAL_P(value);
	value = zend_read_property(php_gd_jxl_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("effort"), true, &rv);
	options->effort = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_jxl_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
	if (Z_TYPE_P(value) == IS_OBJECT) {
		options->metadata = php_gd_metadata_from_zval(value);
	}
}

static gdImagePtr php_gd_jxl_decode_bytes(zend_string *bytes)
{
	gdJxlReadOptions options;
	gdJxlInfo info;
	gdJxlReadPtr reader;
	gdImagePtr image = NULL;
	int result;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return NULL;
	}
	gdJxlReadOptionsInit(&options);
	reader = gdJxlReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &options);
	if (reader == NULL || !gdJxlReadGetInfo(reader, &info)) {
		gdJxlReadClose(reader);
		return NULL;
	}
	if (info.animated) {
		gdJxlReadClose(reader);
		php_gd_jxl_throw("JXL input contains animation; use Gd\\Jxl\\AnimReader");
		return NULL;
	}
	result = gdJxlReadNextImage(reader, NULL, &image);
	gdJxlReadClose(reader);
	return result == 1 ? image : NULL;
}

static bool php_gd_jxl_encode_to_string(zval *image_zv, zval *options_zv, zend_string **bytes)
{
	gdJxlWriteOptions options;
	int size = 0;
	void *data;

	php_gd_jxl_read_write_options(options_zv, &options);
	data = gdImageJxlPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &options);
	if (data == NULL || size < 0) {
		gdFree(data);
		php_gd_jxl_throw("Failed to encode JXL image");
		return false;
	}

	*bytes = zend_string_init(data, (size_t) size, 0);
	gdFree(data);
	return true;
}

PHP_METHOD(Gd_Jxl_WriteOptions, __construct)
{
	bool lossless = false;
	double distance = 1.0;
	zend_long effort = 7;
	zval *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(lossless)
		Z_PARAM_DOUBLE(distance)
		Z_PARAM_LONG(effort)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_validate_encode_options(lossless, distance, effort, 2, 3)) {
		RETURN_THROWS();
	}

	zend_update_property_bool(php_gd_jxl_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("lossless"), lossless);
	zend_update_property_double(php_gd_jxl_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("distance"), distance);
	zend_update_property_long(php_gd_jxl_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("effort"), effort);
	if (metadata) zend_update_property(php_gd_jxl_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata); else zend_update_property_null(php_gd_jxl_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
}

PHP_METHOD(Gd_Jxl_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Jxl_Codec, fromString)
{
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_jxl_decode_bytes(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_jxl_throw("Failed to decode JXL image");
		}
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Jxl_Codec, fromFile)
{
	zend_string *path, *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_jxl_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_jxl_throw("Failed to decode JXL image");
		}
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Jxl_Codec, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_jxl_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_jxl_throw("Failed to decode JXL image");
		}
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Jxl_Codec, toString)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jxl_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	RETURN_STR(bytes);
}

PHP_METHOD(Gd_Jxl_Codec, toFile)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *path, *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jxl_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	stream = php_stream_open_wrapper(ZSTR_VAL(path), "wb", REPORT_ERRORS | IGNORE_PATH, NULL);
	if (stream == NULL) {
		zend_string_release_ex(bytes, 0);
		php_gd_jxl_throw("Failed to open JXL output");
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		php_stream_close(stream);
		zend_string_release_ex(bytes, 0);
		php_gd_jxl_throw("Failed to write JXL output");
		RETURN_THROWS();
	}
	php_stream_close(stream);
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Jxl_Codec, toStream)
{
	zval *image_zv, *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jxl_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	php_stream_from_zval_no_verify(stream, stream_zv);
	if (stream == NULL) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	if (!php_gd_jxl_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		zend_string_release_ex(bytes, 0);
		php_gd_jxl_throw("Failed to write JXL output");
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}
#endif

#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API)
static zend_class_entry *php_gd_jxl_info_ce;

static void php_gd_jxl_create_info(zval *result, const gdJxlInfo *info, gdImageMetadata *metadata)
{
	object_init_ex(result, php_gd_jxl_info_ce);
	zend_update_property_long(php_gd_jxl_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_jxl_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_bool(php_gd_jxl_info_ce, Z_OBJ_P(result), ZEND_STRL("animated"), info->animated != 0);
	zend_update_property_long(php_gd_jxl_info_ce, Z_OBJ_P(result), ZEND_STRL("loopCount"), info->loop_count);
	if (metadata != NULL) {
		zval metadata_zv;
		php_gd_metadata_create_zval(&metadata_zv, metadata);
		zend_update_property(php_gd_jxl_info_ce, Z_OBJ_P(result), ZEND_STRL("metadata"), &metadata_zv);
		zval_ptr_dtor(&metadata_zv);
	} else {
		zend_update_property_null(php_gd_jxl_info_ce, Z_OBJ_P(result), ZEND_STRL("metadata"));
	}
}

PHP_METHOD(Gd_Jxl_Info, __construct)
{
	zend_long width, height, loop_count;
	bool animated;
	zval *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(4, 5)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_BOOL(animated)
		Z_PARAM_LONG(loop_count)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_jxl_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_jxl_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_bool(php_gd_jxl_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("animated"), animated);
	zend_update_property_long(php_gd_jxl_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("loopCount"), loop_count);
	if (metadata) {
		zend_update_property(php_gd_jxl_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata);
	} else {
		zend_update_property_null(php_gd_jxl_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
	}
}
#endif

#ifdef HAVE_GD_JXL_CODEC
static zend_class_entry *php_gd_jxl_reader_ce;
static zend_object_handlers php_gd_jxl_reader_handlers;

typedef struct {
	gdJxlReadPtr reader;
	zval info;
	bool read;
	bool failed;
	zend_object std;
} php_gd_jxl_reader_object;

static php_gd_jxl_reader_object *php_gd_jxl_reader_from_object(zend_object *object)
{
	return (php_gd_jxl_reader_object *) ((char *) object - offsetof(php_gd_jxl_reader_object, std));
}

#define Z_GD_JXL_READER_P(zv) php_gd_jxl_reader_from_object(Z_OBJ_P((zv)))

static zend_object *php_gd_jxl_reader_create(zend_class_entry *class_entry)
{
	php_gd_jxl_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->reader = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_jxl_reader_handlers;
	return &reader->std;
}

static void php_gd_jxl_reader_free(zend_object *object)
{
	php_gd_jxl_reader_object *reader = php_gd_jxl_reader_from_object(object);

	gdJxlReadClose(reader->reader);
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_jxl_initialize_still_reader(zval *result, zend_string *bytes)
{
	gdJxlReadOptions options;
	gdJxlInfo info;
	gdJxlReadPtr native_reader;
	gdImageMetadata *metadata;
	php_gd_jxl_reader_object *reader;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}
	gdJxlReadOptionsInit(&options);
	native_reader = gdJxlReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &options);
	if (native_reader == NULL || !gdJxlReadGetInfo(native_reader, &info)) {
		gdJxlReadClose(native_reader);
		php_gd_jxl_throw("Failed to open JXL input");
		return false;
	}
	metadata = NULL;
	if (!info.animated) {
		metadata = gdImageMetadataCreate();
		if (metadata == NULL || gdJxlReadGetMetadata(native_reader, metadata) != GD_META_OK) {
			gdImageMetadataFree(metadata);
			gdJxlReadClose(native_reader);
			php_gd_jxl_throw("Failed to read JXL metadata");
			return false;
		}
	}

	object_init_ex(result, php_gd_jxl_reader_ce);
	reader = Z_GD_JXL_READER_P(result);
	reader->reader = native_reader;
	php_gd_jxl_create_info(&reader->info, &info, metadata);
	return true;
}

PHP_METHOD(Gd_Jxl_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Jxl_Reader, fromString)
{
	zend_string *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_jxl_initialize_still_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Jxl_Reader, fromFile)
{
	zend_string *path, *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_jxl_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jxl_initialize_still_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Jxl_Reader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_jxl_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jxl_initialize_still_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Jxl_Reader, info)
{
	php_gd_jxl_reader_object *reader = Z_GD_JXL_READER_P(ZEND_THIS);
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_OBJ_COPY(Z_OBJ(reader->info));
}

PHP_METHOD(Gd_Jxl_Reader, read)
{
	php_gd_jxl_reader_object *reader = Z_GD_JXL_READER_P(ZEND_THIS);
	gdImagePtr image = NULL;
	zval rv;
	zval *animated;
	int result;

	ZEND_PARSE_PARAMETERS_NONE();
	if (reader->failed) {
		php_gd_jxl_throw("JXL reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->read) {
		php_gd_jxl_throw("JXL reader has already been read");
		RETURN_THROWS();
	}
	reader->read = true;
	animated = zend_read_property(php_gd_jxl_info_ce, Z_OBJ(reader->info), ZEND_STRL("animated"), true, &rv);
	if (Z_TYPE_P(animated) == IS_TRUE) {
		reader->failed = true;
		php_gd_jxl_throw("JXL input contains animation; use Gd\\Jxl\\AnimReader");
		RETURN_THROWS();
	}
	result = gdJxlReadNextImage(reader->reader, NULL, &image);
	if (result != 1 || image == NULL) {
		reader->failed = true;
		php_gd_jxl_throw("Failed to decode JXL image");
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}
#endif

#ifdef HAVE_GD_JXL_ANIM_READ_API
static zend_class_entry *php_gd_jxl_frame_ce;
static zend_class_entry *php_gd_jxl_anim_reader_ce;
static zend_object_handlers php_gd_jxl_anim_reader_handlers;

typedef struct {
	gdJxlReadPtr reader;
	zval info;
	bool exhausted;
	bool failed;
	zend_long frame_index;
	zend_long timestamp_ms;
	zend_object std;
} php_gd_jxl_anim_reader_object;

static php_gd_jxl_anim_reader_object *php_gd_jxl_anim_reader_from_object(zend_object *object)
{
	return (php_gd_jxl_anim_reader_object *) ((char *) object - offsetof(php_gd_jxl_anim_reader_object, std));
}

#define Z_GD_JXL_ANIM_READER_P(zv) php_gd_jxl_anim_reader_from_object(Z_OBJ_P((zv)))

static void php_gd_jxl_create_frame(zval *result, gdImagePtr image, zend_long frame_index,
		zend_long delay_ms, zend_long timestamp_ms)
{
	zval image_zv;

	object_init_ex(result, php_gd_jxl_frame_ce);
	php_gd_assign_libgdimageptr_as_extgdimage(&image_zv, image);
	zend_update_property(php_gd_jxl_frame_ce, Z_OBJ_P(result), ZEND_STRL("image"), &image_zv);
	zval_ptr_dtor(&image_zv);
	zend_update_property_long(php_gd_jxl_frame_ce, Z_OBJ_P(result), ZEND_STRL("frameIndex"), frame_index);
	zend_update_property_long(php_gd_jxl_frame_ce, Z_OBJ_P(result), ZEND_STRL("delayMs"), delay_ms);
	zend_update_property_long(php_gd_jxl_frame_ce, Z_OBJ_P(result), ZEND_STRL("timestampMs"), timestamp_ms);
}

static zend_object *php_gd_jxl_anim_reader_create(zend_class_entry *class_entry)
{
	php_gd_jxl_anim_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->reader = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->exhausted = false;
	reader->failed = false;
	reader->frame_index = 0;
	reader->timestamp_ms = 0;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_jxl_anim_reader_handlers;
	return &reader->std;
}

static void php_gd_jxl_anim_reader_free(zend_object *object)
{
	php_gd_jxl_anim_reader_object *reader = php_gd_jxl_anim_reader_from_object(object);

	if (reader->reader != NULL) {
		gdJxlReadClose(reader->reader);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_jxl_initialize_reader(zval *result, zend_string *bytes)
{
	gdJxlReadPtr jxl;
	gdJxlReadOptions options;
	gdJxlInfo info;
	php_gd_jxl_anim_reader_object *reader;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}

	gdJxlReadOptionsInit(&options);
	options.coalesced = 1;
	jxl = gdJxlReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &options);
	if (jxl == NULL || !gdJxlReadGetInfo(jxl, &info)) {
		if (jxl != NULL) {
			gdJxlReadClose(jxl);
		}
		php_gd_jxl_throw("Failed to open JXL input");
		return false;
	}

	object_init_ex(result, php_gd_jxl_anim_reader_ce);
	reader = Z_GD_JXL_ANIM_READER_P(result);
	reader->reader = jxl;
	php_gd_jxl_create_info(&reader->info, &info, NULL);
	return true;
}

PHP_METHOD(Gd_Jxl_Frame, __construct)
{
	zval *image_zv;
	zend_long frame_index, delay_ms, timestamp_ms;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_LONG(frame_index)
		Z_PARAM_LONG(delay_ms)
		Z_PARAM_LONG(timestamp_ms)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property(php_gd_jxl_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("image"), image_zv);
	zend_update_property_long(php_gd_jxl_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("frameIndex"), frame_index);
	zend_update_property_long(php_gd_jxl_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("delayMs"), delay_ms);
	zend_update_property_long(php_gd_jxl_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("timestampMs"), timestamp_ms);
}

PHP_METHOD(Gd_Jxl_AnimReader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Jxl_AnimReader, fromString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_initialize_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Jxl_AnimReader, fromFile)
{
	zend_string *path, *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jxl_initialize_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Jxl_AnimReader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jxl_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jxl_initialize_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Jxl_AnimReader, info)
{
	php_gd_jxl_anim_reader_object *reader = Z_GD_JXL_ANIM_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_OBJ_COPY(Z_OBJ(reader->info));
}

PHP_METHOD(Gd_Jxl_AnimReader, next)
{
	php_gd_jxl_anim_reader_object *reader = Z_GD_JXL_ANIM_READER_P(ZEND_THIS);
	gdImagePtr image = NULL;
	int delay_ms = 0;
	int result;

	ZEND_PARSE_PARAMETERS_NONE();

	if (reader->failed) {
		php_gd_jxl_throw("JXL reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->exhausted) {
		RETURN_NULL();
	}

	result = gdJxlReadNextImage(reader->reader, &delay_ms, &image);
	if (result == 0) {
		reader->exhausted = true;
		RETURN_NULL();
	}
	if (result < 0 || image == NULL) {
		reader->failed = true;
		php_gd_jxl_throw("Failed to decode JXL frame");
		RETURN_THROWS();
	}

	php_gd_jxl_create_frame(return_value, image, reader->frame_index, delay_ms, reader->timestamp_ms);
	reader->frame_index++;
	reader->timestamp_ms += delay_ms;
}
#endif

#ifdef HAVE_GD_JXL_ANIM_WRITE_API
static zend_class_entry *php_gd_jxl_anim_write_options_ce;
static zend_class_entry *php_gd_jxl_anim_writer_ce;
static zend_object_handlers php_gd_jxl_anim_writer_handlers;

typedef enum {
	PHP_GD_JXL_DESTINATION_CONTEXT,
	PHP_GD_JXL_DESTINATION_MEMORY
} php_gd_jxl_destination;

typedef struct {
	gdJxlWritePtr writer;
	gdIOCtx *ctx;
	php_gd_jxl_destination destination;
	bool finalized;
	zend_long frame_count;
	int canvas_width;
	int canvas_height;
	int loop_count;
	bool lossless;
	double distance;
	int effort;
	zend_object std;
} php_gd_jxl_anim_writer_object;

static php_gd_jxl_anim_writer_object *php_gd_jxl_anim_writer_from_object(zend_object *object)
{
	return (php_gd_jxl_anim_writer_object *) ((char *) object - offsetof(php_gd_jxl_anim_writer_object, std));
}

#define Z_GD_JXL_ANIM_WRITER_P(zv) php_gd_jxl_anim_writer_from_object(Z_OBJ_P((zv)))

static zend_object *php_gd_jxl_anim_writer_create(zend_class_entry *class_entry)
{
	php_gd_jxl_anim_writer_object *writer = zend_object_alloc(sizeof(*writer), class_entry);

	writer->writer = NULL;
	writer->ctx = NULL;
	writer->destination = PHP_GD_JXL_DESTINATION_CONTEXT;
	writer->finalized = false;
	writer->frame_count = 0;
	writer->canvas_width = 0;
	writer->canvas_height = 0;
	writer->loop_count = 0;
	writer->lossless = false;
	writer->distance = 1.0;
	writer->effort = 7;
	zend_object_std_init(&writer->std, class_entry);
	object_properties_init(&writer->std, class_entry);
	writer->std.handlers = &php_gd_jxl_anim_writer_handlers;
	return &writer->std;
}

static void php_gd_jxl_anim_writer_close(php_gd_jxl_anim_writer_object *writer)
{
	if (writer->writer != NULL) {
		gdJxlWriteClose(writer->writer);
		writer->writer = NULL;
	}
	if (writer->ctx != NULL) {
		writer->ctx->gd_free(writer->ctx);
		writer->ctx = NULL;
	}
}

static void php_gd_jxl_anim_writer_free(zend_object *object)
{
	php_gd_jxl_anim_writer_object *writer = php_gd_jxl_anim_writer_from_object(object);

	php_gd_jxl_anim_writer_close(writer);
	zend_object_std_dtor(&writer->std);
}

static void php_gd_jxl_read_anim_options(zval *options_zv, php_gd_jxl_anim_writer_object *writer)
{
	zval rv;
	zval *value;

	if (options_zv == NULL) {
		return;
	}
	value = zend_read_property(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("canvasWidth"), true, &rv);
	writer->canvas_width = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("canvasHeight"), true, &rv);
	writer->canvas_height = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("loopCount"), true, &rv);
	writer->loop_count = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("lossless"), true, &rv);
	writer->lossless = Z_TYPE_P(value) == IS_TRUE;
	value = zend_read_property(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("distance"), true, &rv);
	writer->distance = Z_DVAL_P(value);
	value = zend_read_property(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("effort"), true, &rv);
	writer->effort = (int) Z_LVAL_P(value);
}

static bool php_gd_jxl_create_anim_writer(zval *return_value, gdIOCtx *ctx,
		php_gd_jxl_destination destination, zval *options_zv)
{
	php_gd_jxl_anim_writer_object *writer;

	object_init_ex(return_value, php_gd_jxl_anim_writer_ce);
	writer = Z_GD_JXL_ANIM_WRITER_P(return_value);
	writer->ctx = ctx;
	writer->destination = destination;
	php_gd_jxl_read_anim_options(options_zv, writer);
	return true;
}

PHP_METHOD(Gd_Jxl_AnimWriteOptions, __construct)
{
	zend_long canvas_width = 0, canvas_height = 0, loop_count = 0, effort = 7;
	bool lossless = false;
	double distance = 1.0;

	ZEND_PARSE_PARAMETERS_START(0, 6)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(canvas_width)
		Z_PARAM_LONG(canvas_height)
		Z_PARAM_LONG(loop_count)
		Z_PARAM_BOOL(lossless)
		Z_PARAM_DOUBLE(distance)
		Z_PARAM_LONG(effort)
	ZEND_PARSE_PARAMETERS_END();

	if (canvas_width < 0 || canvas_width > INT_MAX) {
		zend_argument_value_error(1, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (canvas_height < 0 || canvas_height > INT_MAX) {
		zend_argument_value_error(2, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (loop_count < 0 || loop_count > 65535) {
		zend_argument_value_error(3, "must be between 0 and 65535");
		RETURN_THROWS();
	}
	if (!php_gd_jxl_validate_encode_options(lossless, distance, effort, 5, 6)) {
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("canvasWidth"), canvas_width);
	zend_update_property_long(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("canvasHeight"), canvas_height);
	zend_update_property_long(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("loopCount"), loop_count);
	zend_update_property_bool(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("lossless"), lossless);
	zend_update_property_double(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("distance"), distance);
	zend_update_property_long(php_gd_jxl_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("effort"), effort);
}

PHP_METHOD(Gd_Jxl_AnimWriter, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Jxl_AnimWriter, toFile)
{
	zval *options_zv = NULL;
	zend_string *path;
	zval path_zv;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jxl_anim_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_STR(&path_zv, path);
	ctx = php_gd_create_output_context(&path_zv, 1);
	if (ctx == NULL) {
		php_gd_jxl_throw("Failed to open JXL output");
		RETURN_THROWS();
	}
	php_gd_jxl_create_anim_writer(return_value, ctx, PHP_GD_JXL_DESTINATION_CONTEXT, options_zv);
}

PHP_METHOD(Gd_Jxl_AnimWriter, toStream)
{
	zval *stream_zv, *options_zv = NULL;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jxl_anim_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ctx = php_gd_create_output_context(stream_zv, 1);
	if (ctx == NULL) {
		php_gd_jxl_throw("Failed to open JXL output");
		RETURN_THROWS();
	}
	php_gd_jxl_create_anim_writer(return_value, ctx, PHP_GD_JXL_DESTINATION_CONTEXT, options_zv);
}

PHP_METHOD(Gd_Jxl_AnimWriter, toMemory)
{
	zval *options_zv = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jxl_anim_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_jxl_create_anim_writer(return_value, NULL, PHP_GD_JXL_DESTINATION_MEMORY, options_zv);
}

PHP_METHOD(Gd_Jxl_AnimWriter, addFrame)
{
	php_gd_jxl_anim_writer_object *writer = Z_GD_JXL_ANIM_WRITER_P(ZEND_THIS);
	zval *image_zv;
	zend_long delay_ms = 0;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(delay_ms)
	ZEND_PARSE_PARAMETERS_END();

	if (writer->finalized) {
		php_gd_jxl_throw("JXL writer has already been finalized");
		RETURN_THROWS();
	}
	if (delay_ms < 0 || delay_ms > INT_MAX) {
		zend_argument_value_error(2, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}

	image = php_gd_libgdimageptr_from_zval_p(image_zv);
	if (!image->trueColor) {
		php_gd_jxl_throw("JXL animation writer only supports truecolor images");
		RETURN_THROWS();
	}
	if (writer->frame_count == 0) {
		gdJxlAnimWriteOptions options;

		if (writer->canvas_width <= 0) {
			writer->canvas_width = image->sx;
		}
		if (writer->canvas_height <= 0) {
			writer->canvas_height = image->sy;
		}
		gdJxlAnimWriteOptionsInit(&options);
		options.canvasWidth = writer->canvas_width;
		options.canvasHeight = writer->canvas_height;
		options.lossless = writer->lossless ? 1 : 0;
		options.distance = (float) writer->distance;
		options.effort = writer->effort;
		options.loopCount = writer->loop_count;
		if (writer->destination == PHP_GD_JXL_DESTINATION_MEMORY) {
			writer->writer = gdJxlWriteOpenPtr(&options);
		} else {
			writer->writer = gdJxlWriteOpenCtx(writer->ctx, &options);
		}
		if (writer->writer == NULL) {
			php_gd_jxl_throw("Failed to initialize JXL output");
			RETURN_THROWS();
		}
	}
	if (image->sx != writer->canvas_width || image->sy != writer->canvas_height) {
		php_gd_jxl_throw("JXL animation frames must match canvas size");
		RETURN_THROWS();
	}
	if (!gdJxlWriteAddImage(writer->writer, image, (int) delay_ms)) {
		php_gd_jxl_throw("Failed to add JXL frame");
		RETURN_THROWS();
	}
	writer->frame_count++;
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Jxl_AnimWriter, finish)
{
	php_gd_jxl_anim_writer_object *writer = Z_GD_JXL_ANIM_WRITER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (writer->finalized) {
		php_gd_jxl_throw("JXL writer has already been finalized");
		RETURN_THROWS();
	}
	if (writer->frame_count == 0) {
		php_gd_jxl_throw("Cannot finalize a JXL writer without frames");
		RETURN_THROWS();
	}

	writer->finalized = true;
	if (writer->destination == PHP_GD_JXL_DESTINATION_MEMORY) {
		int size = 0;
		void *data = gdJxlWritePtrFinish(writer->writer, &size);
		writer->writer = NULL;
		if (data == NULL || size < 0) {
			gdFree(data);
			php_gd_jxl_throw("Failed to finalize JXL output");
			RETURN_THROWS();
		}
		RETVAL_STRINGL(data, size);
		gdFree(data);
		return;
	}

	gdJxlWriteClose(writer->writer);
	writer->writer = NULL;
	if (writer->ctx != NULL) {
		writer->ctx->gd_free(writer->ctx);
		writer->ctx = NULL;
	}
	RETURN_NULL();
}
#endif

void php_gd_jxl_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_JXL_CODEC
	zend_class_entry *codec_ce;

	php_gd_jxl_write_options_ce = register_class_Gd_Jxl_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Jxl_Codec();
	php_gd_jxl_info_ce = register_class_Gd_Jxl_Info();
	php_gd_jxl_reader_ce = register_class_Gd_Jxl_Reader();
	php_gd_jxl_reader_ce->create_object = php_gd_jxl_reader_create;
	memcpy(&php_gd_jxl_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_jxl_reader_handlers.offset = offsetof(php_gd_jxl_reader_object, std);
	php_gd_jxl_reader_handlers.free_obj = php_gd_jxl_reader_free;
	php_gd_jxl_reader_handlers.clone_obj = NULL;
	php_gd_register_codec_write(php_gd_jxl_write_options_ce, codec_ce);
	php_gd_register_codec_format("Jxl", php_gd_jxl_write_options_ce);
	php_gd_register_codec_extension("jxl", php_gd_jxl_write_options_ce);
#endif

#ifdef HAVE_GD_JXL_ANIM_READ_API
	#ifndef HAVE_GD_JXL_CODEC
	php_gd_jxl_info_ce = register_class_Gd_Jxl_Info();
	#endif
	php_gd_jxl_frame_ce = register_class_Gd_Jxl_Frame();
	php_gd_jxl_anim_reader_ce = register_class_Gd_Jxl_AnimReader();
	php_gd_jxl_anim_reader_ce->create_object = php_gd_jxl_anim_reader_create;

	memcpy(&php_gd_jxl_anim_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_jxl_anim_reader_handlers.offset = offsetof(php_gd_jxl_anim_reader_object, std);
	php_gd_jxl_anim_reader_handlers.free_obj = php_gd_jxl_anim_reader_free;
	php_gd_jxl_anim_reader_handlers.clone_obj = NULL;
#endif

#ifdef HAVE_GD_JXL_ANIM_WRITE_API
	php_gd_jxl_anim_write_options_ce = register_class_Gd_Jxl_AnimWriteOptions();
	php_gd_jxl_anim_writer_ce = register_class_Gd_Jxl_AnimWriter();
	php_gd_jxl_anim_writer_ce->create_object = php_gd_jxl_anim_writer_create;

	memcpy(&php_gd_jxl_anim_writer_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_jxl_anim_writer_handlers.offset = offsetof(php_gd_jxl_anim_writer_object, std);
	php_gd_jxl_anim_writer_handlers.free_obj = php_gd_jxl_anim_writer_free;
	php_gd_jxl_anim_writer_handlers.clone_obj = NULL;
#endif
}
