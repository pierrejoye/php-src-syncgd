/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_enum.h"
#include "zend_exceptions.h"
#include "php_gd.h"
#include "gd_codec_write.h"
#include "gd_avif.h"
#include "gd_metadata.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>
#include <string.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gd_avif_metadata.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_avif_arginfo.h"
#include "gd_avif_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#ifdef HAVE_GD_AVIF
enum {
	PHP_GD_AVIF_CHROMA_SUBSAMPLING_AUTO = 0,
	PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV420 = 1,
	PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV444 = 2
};

static zend_class_entry *php_gd_avif_chroma_subsampling_ce;
static zend_class_entry *php_gd_avif_read_options_ce;
static zend_class_entry *php_gd_avif_write_options_ce;
static zend_class_entry *php_gd_avif_info_ce;
static zend_class_entry *php_gd_avif_reader_ce;
static zend_object_handlers php_gd_avif_reader_handlers;

typedef struct {
	zend_string *bytes;
	zval info;
	bool read;
	bool failed;
	zend_object std;
} php_gd_avif_reader_object;

typedef struct {
	int width;
	int height;
	int is_animation;
	int is_progressive;
	int frame_count;
	double duration;
	int has_alpha;
	int bit_depth;
	int yuv_format;
} php_gd_avif_info;

static php_gd_avif_reader_object *php_gd_avif_reader_from_object(zend_object *object)
{
	return (php_gd_avif_reader_object *) ((char *) object - offsetof(php_gd_avif_reader_object, std));
}

#define Z_GD_AVIF_READER_P(zv) php_gd_avif_reader_from_object(Z_OBJ_P((zv)))

static void php_gd_avif_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_avif_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_avif_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_avif_throw("Failed to open AVIF input");
		return false;
	}
	*bytes = php_gd_avif_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_avif_throw("Failed to read AVIF input");
		return false;
	}
	return true;
}

static bool php_gd_avif_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_avif_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_avif_throw("Failed to read AVIF input");
		return false;
	}
	return true;
}

static gdImagePtr php_gd_avif_decode_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return NULL;
	}
	return gdImageCreateFromAvifPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
}

static void php_gd_avif_create_info(zval *result, const php_gd_avif_info *info,
	gdImageMetadata *metadata)
{
	zval value;

	object_init_ex(result, php_gd_avif_info_ce);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_bool(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("isAnimation"), info->is_animation);
	zend_update_property_bool(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("isProgressive"), info->is_progressive);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("frameCount"), info->frame_count);
	zend_update_property_double(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("duration"), info->duration);
	zend_update_property_bool(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("hasAlpha"), info->has_alpha);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("bitDepth"), info->bit_depth);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("yuvFormat"), info->yuv_format);
	php_gd_metadata_create_zval(&value, metadata);
	zend_update_property(php_gd_avif_info_ce, Z_OBJ_P(result), ZEND_STRL("metadata"), &value);
	zval_ptr_dtor(&value);
}

static zend_object *php_gd_avif_reader_create(zend_class_entry *class_entry)
{
	php_gd_avif_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_avif_reader_handlers;
	return &reader->std;
}

static void php_gd_avif_reader_free(zend_object *object)
{
	php_gd_avif_reader_object *reader = php_gd_avif_reader_from_object(object);

	if (reader->bytes != NULL) {
		zend_string_release(reader->bytes);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_avif_initialize_reader(zval *result, zend_string *bytes)
{
	php_gd_avif_reader_object *reader;
	gdImageMetadata *metadata = gdImageMetadataCreate();
	php_gd_avif_info info;

	if (metadata == NULL) {
		php_gd_avif_throw("Failed to allocate AVIF metadata");
		return false;
	}
#ifdef HAVE_GD_BUNDLED
	gdAvifInfo gd_info;
	gdAvifInfoInit(&gd_info);
	gd_info.metadata = metadata;
	if (ZSTR_LEN(bytes) > INT_MAX || gdAvifGetInfoPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &gd_info) != GD_META_OK) {
		gdImageMetadataFree(metadata);
		php_gd_avif_throw("Failed to read AVIF metadata");
		return false;
	}
	info.width = gd_info.width;
	info.height = gd_info.height;
	info.is_animation = gd_info.is_animation;
	info.is_progressive = gd_info.is_progressive;
	info.frame_count = gd_info.frame_count;
	info.duration = gd_info.duration;
	info.has_alpha = gd_info.has_alpha;
	info.bit_depth = gd_info.bit_depth;
	info.yuv_format = gd_info.yuv_format;
#else
	gdImagePtr image;
	if (ZSTR_LEN(bytes) > INT_MAX) {
		gdImageMetadataFree(metadata);
		php_gd_avif_throw("AVIF input is too large");
		return false;
	}
	image = gdImageCreateFromAvifPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
	if (image == NULL) {
		gdImageMetadataFree(metadata);
		php_gd_avif_throw("Failed to read AVIF input");
		return false;
	}
	info.width = gdImageSX(image);
	info.height = gdImageSY(image);
	info.is_animation = false;
	info.is_progressive = false;
	info.frame_count = 1;
	info.duration = 0.0;
	info.has_alpha = false;
	info.bit_depth = 8;
	info.yuv_format = GD_AVIF_PIXEL_FORMAT_NONE;
	gdImageDestroy(image);
#endif

	object_init_ex(result, php_gd_avif_reader_ce);
	reader = Z_GD_AVIF_READER_P(result);
	reader->bytes = zend_string_copy(bytes);
	php_gd_avif_create_info(&reader->info, &info, metadata);
	return true;
}

typedef struct {
	int quality;
	int speed;
	bool lossless;
	int chroma_subsampling;
	gdImageMetadata *metadata;
} php_gd_avif_write_options;

static bool php_gd_avif_read_write_options(zval *options_zv, php_gd_avif_write_options *options)
{
	zval rv;
	zval *value;

	options->quality = -1;
	options->speed = 6;
	options->lossless = false;
	options->chroma_subsampling = PHP_GD_AVIF_CHROMA_SUBSAMPLING_AUTO;
	options->metadata = NULL;
	if (options_zv == NULL) {
		return true;
	}

	value = zend_read_property(php_gd_avif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("quality"), true, &rv);
	options->quality = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_avif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("speed"), true, &rv);
	options->speed = (int) Z_LVAL_P(value);
	if (options->speed == -1) {
		options->speed = 6;
	}
	value = zend_read_property(php_gd_avif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("lossless"), true, &rv);
	options->lossless = zend_is_true(value);
	value = zend_read_property(php_gd_avif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("chromaSubsampling"), true, &rv);
	if (Z_TYPE_P(value) == IS_NULL) {
		value = zend_read_property(php_gd_avif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
		if (Z_TYPE_P(value) == IS_OBJECT) {
			options->metadata = php_gd_metadata_from_zval(value);
		}
		return true;
	}
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Avif_ChromaSubsampling_Yuv420:
			options->chroma_subsampling = PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV420;
			break;
		case ZEND_ENUM_Gd_Avif_ChromaSubsampling_Yuv444:
			options->chroma_subsampling = PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV444;
			break;
		default:
			php_gd_avif_throw("Unsupported AVIF chroma subsampling option");
			return false;
	}
	value = zend_read_property(php_gd_avif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
	if (Z_TYPE_P(value) == IS_OBJECT) {
		options->metadata = php_gd_metadata_from_zval(value);
	}
	return true;
}

static bool php_gd_avif_encode_to_string(zval *image_zv, zval *options_zv, zend_string **bytes)
{
	php_gd_avif_write_options options;
	int size = 0;
	void *data;

	if (!php_gd_avif_read_write_options(options_zv, &options)) {
		return false;
	}

#ifdef HAVE_GD_BUNDLED
	gdAvifWriteOptions gd_options;

	gdAvifWriteOptionsInit(&gd_options);
	gd_options.quality = options.quality;
	gd_options.speed = options.speed;
	gd_options.lossless = options.lossless ? 1 : 0;
	switch (options.chroma_subsampling) {
		case PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV420:
			gd_options.chroma_subsampling = GD_AVIF_CHROMA_SUBSAMPLING_YUV420;
			break;
		case PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV444:
			gd_options.chroma_subsampling = GD_AVIF_CHROMA_SUBSAMPLING_YUV444;
			break;
		default:
			gd_options.chroma_subsampling = GD_AVIF_CHROMA_SUBSAMPLING_AUTO;
			break;
	}
	gd_options.metadata = options.metadata;
	data = gdImageAvifPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &gd_options);
#else
	if (options.chroma_subsampling != PHP_GD_AVIF_CHROMA_SUBSAMPLING_AUTO) {
		php_gd_avif_throw("Forced AVIF chroma subsampling is not supported by this gd build");
		return false;
	}
	data = gdImageAvifPtrEx(
		php_gd_libgdimageptr_from_zval_p(image_zv),
		&size,
		options.lossless ? 100 : options.quality,
		options.speed
	);
#endif

	if (data == NULL || size < 0) {
		gdFree(data);
		php_gd_avif_throw("Failed to encode AVIF image");
		return false;
	}

	*bytes = zend_string_init(data, (size_t) size, 0);
	gdFree(data);
	return true;
}

#ifdef HAVE_GD_BUNDLED
static void php_gd_avif_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv;
	zval *destination;
	zval *options_zv = NULL;
	php_gd_avif_write_options options;
	gdAvifWriteOptions gd_options;
	gdIOCtx *ctx;
	int result;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ZEND_ASSERT(require_stream || Z_TYPE_P(destination) == IS_STRING);

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		if (!EG(exception)) {
			php_gd_avif_throw("Failed to open AVIF output");
		}
		RETURN_THROWS();
	}

	if (!php_gd_avif_read_write_options(options_zv, &options)) {
		ctx->gd_free(ctx);
		RETURN_THROWS();
	}
	gdAvifWriteOptionsInit(&gd_options);
	gd_options.quality = options.quality;
	gd_options.speed = options.speed;
	gd_options.lossless = options.lossless ? 1 : 0;
	switch (options.chroma_subsampling) {
		case PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV420:
			gd_options.chroma_subsampling = GD_AVIF_CHROMA_SUBSAMPLING_YUV420;
			break;
		case PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV444:
			gd_options.chroma_subsampling = GD_AVIF_CHROMA_SUBSAMPLING_YUV444;
			break;
		default:
			gd_options.chroma_subsampling = GD_AVIF_CHROMA_SUBSAMPLING_AUTO;
			break;
	}
	gd_options.metadata = options.metadata;
	result = gdImageAvifCtxWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), ctx, &gd_options);
	ctx->gd_free(ctx);

	if (result != 0) {
		php_gd_avif_throw("Failed to encode AVIF image");
		RETURN_THROWS();
	}
}
#endif

PHP_METHOD(Gd_Avif_ReadOptions, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Avif_Info, __construct)
{
	zend_long width, height;
	bool is_animation, is_progressive, has_alpha;
	zend_long frame_count, bit_depth, yuv_format;
	double duration;
	zval *metadata;

	ZEND_PARSE_PARAMETERS_START(11, 11)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_BOOL(is_animation)
		Z_PARAM_BOOL(is_progressive)
		Z_PARAM_LONG(frame_count)
		Z_PARAM_DOUBLE(duration)
		Z_PARAM_BOOL(has_alpha)
		Z_PARAM_LONG(bit_depth)
		Z_PARAM_LONG(yuv_format)
		Z_PARAM_OBJECT_OF_CLASS(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_bool(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("isAnimation"), is_animation);
	zend_update_property_bool(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("isProgressive"), is_progressive);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("frameCount"), frame_count);
	zend_update_property_double(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("duration"), duration);
	zend_update_property_bool(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("hasAlpha"), has_alpha);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("bitDepth"), bit_depth);
	zend_update_property_long(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yuvFormat"), yuv_format);
	zend_update_property(php_gd_avif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata);
}

PHP_METHOD(Gd_Avif_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Avif_Reader, fromString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		RETURN_THROWS();
	}
	if (!php_gd_avif_initialize_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Avif_Reader, fromFile)
{
	zend_string *path, *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_avif_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_string_release(bytes);
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		RETURN_THROWS();
	}
	if (!php_gd_avif_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Avif_Reader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_avif_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_string_release(bytes);
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		RETURN_THROWS();
	}
	if (!php_gd_avif_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Avif_Reader, info)
{
	php_gd_avif_reader_object *reader = Z_GD_AVIF_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&reader->info);
}

PHP_METHOD(Gd_Avif_Reader, read)
{
	php_gd_avif_reader_object *reader = Z_GD_AVIF_READER_P(ZEND_THIS);
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_NONE();
	if (reader->failed) {
		php_gd_avif_throw("AVIF reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->read) {
		php_gd_avif_throw("AVIF image has already been read");
		RETURN_THROWS();
	}

	image = php_gd_avif_decode_bytes(reader->bytes);
	reader->read = true;
	if (image == NULL) {
		reader->failed = true;
		php_gd_avif_throw("Failed to decode AVIF image");
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Avif_WriteOptions, __construct)
{
	zend_long quality = -1, speed = -1;
	bool lossless = false;
	zval *chroma_subsampling = NULL, *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 5)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(quality)
		Z_PARAM_LONG(speed)
		Z_PARAM_BOOL(lossless)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(chroma_subsampling, php_gd_avif_chroma_subsampling_ce)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (quality < -1 || quality > 100) {
		zend_argument_value_error(1, "must be between -1 and 100");
		RETURN_THROWS();
	}
	if (speed < -1 || speed > 10) {
		zend_argument_value_error(2, "must be between -1 and 10");
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("quality"), quality);
	zend_update_property_long(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("speed"), speed);
	zend_update_property_bool(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("lossless"), lossless);
	if (chroma_subsampling != NULL) {
		zend_update_property(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("chromaSubsampling"), chroma_subsampling);
	} else {
		zend_update_property_null(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("chromaSubsampling"));
	}
	if (metadata) zend_update_property(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata); else zend_update_property_null(php_gd_avif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
}

PHP_METHOD(Gd_Avif_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Avif_Codec, fromString)
{
	zend_string *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_avif_decode_bytes(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_avif_throw("Failed to decode AVIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Avif_Codec, fromFile)
{
	zend_string *path, *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_avif_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_avif_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_avif_throw("Failed to decode AVIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Avif_Codec, fromStream)
{
	zval *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_avif_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_avif_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_avif_throw("Failed to decode AVIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Avif_Codec, toString)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_avif_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	RETURN_STR(bytes);
}

PHP_METHOD(Gd_Avif_Codec, toFile)
{
#ifdef HAVE_GD_BUNDLED
	php_gd_avif_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
#else
	zval *image_zv, *options_zv = NULL;
	zend_string *path, *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_avif_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	stream = php_stream_open_wrapper(ZSTR_VAL(path), "wb", REPORT_ERRORS | IGNORE_PATH, NULL);
	if (stream == NULL) {
		zend_string_release_ex(bytes, 0);
		php_gd_avif_throw("Failed to open AVIF output");
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		php_stream_close(stream);
		zend_string_release_ex(bytes, 0);
		php_gd_avif_throw("Failed to write AVIF output");
		RETURN_THROWS();
	}
	php_stream_close(stream);
	zend_string_release_ex(bytes, 0);
#endif
}

PHP_METHOD(Gd_Avif_Codec, toStream)
{
#ifdef HAVE_GD_BUNDLED
	php_gd_avif_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
#else
	zval *image_zv, *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_avif_write_options_ce)
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
	if (!php_gd_avif_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		zend_string_release_ex(bytes, 0);
		php_gd_avif_throw("Failed to write AVIF output");
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
#endif
}
#endif

void php_gd_avif_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_AVIF
	zend_class_entry *codec_ce;

	php_gd_avif_chroma_subsampling_ce = register_class_Gd_Avif_ChromaSubsampling();
	php_gd_avif_read_options_ce = register_class_Gd_Avif_ReadOptions();
	php_gd_avif_info_ce = register_class_Gd_Avif_Info();
	php_gd_avif_reader_ce = register_class_Gd_Avif_Reader();
	php_gd_avif_reader_ce->create_object = php_gd_avif_reader_create;

	memcpy(&php_gd_avif_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_avif_reader_handlers.offset = offsetof(php_gd_avif_reader_object, std);
	php_gd_avif_reader_handlers.free_obj = php_gd_avif_reader_free;
	php_gd_avif_reader_handlers.clone_obj = NULL;

	php_gd_avif_write_options_ce = register_class_Gd_Avif_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Avif_Codec();
	php_gd_register_codec_write(php_gd_avif_write_options_ce, codec_ce);
	php_gd_register_codec_format("Avif", php_gd_avif_write_options_ce);
	php_gd_register_codec_extension("avif", php_gd_avif_write_options_ce);
#endif
}
