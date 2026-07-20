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
#include "zend_enum.h"
#include "zend_exceptions.h"
#include "php_gd.h"
#include "gd_codec_write.h"
#include "gd_webp.h"
#include "gd_metadata.h"
#include "ext/spl/spl_exceptions.h"
#include <stdint.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_webp_arginfo.h"
#include "gd_webp_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#if defined(HAVE_GD_WEBP) || defined(HAVE_GD_WEBP_ANIM_READ_API) || defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static void php_gd_webp_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_webp_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_webp_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_webp_throw("Failed to open WebP input");
		return false;
	}
	*bytes = php_gd_webp_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_webp_throw("Failed to read WebP input");
		return false;
	}
	return true;
}

static bool php_gd_webp_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_webp_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_webp_throw("Failed to read WebP input");
		return false;
	}
	return true;
}

static uint32_t php_gd_webp_le32(const unsigned char *p)
{
	return ((uint32_t) p[0]) | (((uint32_t) p[1]) << 8) | (((uint32_t) p[2]) << 16) | (((uint32_t) p[3]) << 24);
}

static bool php_gd_webp_bytes_are_animated(zend_string *bytes)
{
	const unsigned char *data = (const unsigned char *) ZSTR_VAL(bytes);
	size_t len = ZSTR_LEN(bytes);
	size_t pos = 12;

	if (len < 12 || memcmp(data, "RIFF", 4) != 0 || memcmp(data + 8, "WEBP", 4) != 0) {
		return false;
	}

	while (pos + 8 <= len) {
		uint32_t chunk_size = php_gd_webp_le32(data + pos + 4);
		size_t payload = pos + 8;

		if (memcmp(data + pos, "ANIM", 4) == 0 || memcmp(data + pos, "ANMF", 4) == 0) {
			return true;
		}
		if (memcmp(data + pos, "VP8X", 4) == 0 && chunk_size >= 1 && payload < len && (data[payload] & 0x02) != 0) {
			return true;
		}
		if (chunk_size > SIZE_MAX - payload) {
			return false;
		}
		pos = payload + chunk_size + (chunk_size & 1);
	}

	return false;
}
#endif

#ifdef HAVE_GD_WEBP
static zend_class_entry *php_gd_webp_write_options_ce;

static void php_gd_webp_build_write_options(zval *options_zv, gdWebpWriteOptions *options)
{
	zval rv;
	zval *value;

	gdWebpWriteOptionsInit(options);
	if (options_zv == NULL) {
		return;
	}
	value = zend_read_property(php_gd_webp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("quality"), true, &rv);
	options->quality = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
	if (Z_TYPE_P(value) == IS_OBJECT) {
		options->metadata = php_gd_metadata_from_zval(value);
	}
}

static void *php_gd_webp_encode(zval *image_zv, zval *options_zv, int *size)
{
	gdWebpWriteOptions options;

	php_gd_webp_build_write_options(options_zv, &options);
	return gdImageWebpPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), size, &options);
}

static gdImagePtr php_gd_webp_decode_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return NULL;
	}
	if (php_gd_webp_bytes_are_animated(bytes)) {
		php_gd_webp_throw("WebP input contains animation; use Gd\\Webp\\AnimReader");
		return NULL;
	}
	return gdImageCreateFromWebpPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
}

PHP_METHOD(Gd_Webp_WriteOptions, __construct)
{
	zend_long quality = -1;
	zval *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(quality)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (quality < -1) {
		zend_argument_value_error(1, "must be greater than or equal to -1");
		RETURN_THROWS();
	}
	zend_update_property_long(php_gd_webp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("quality"), quality);
	if (metadata) zend_update_property(php_gd_webp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata); else zend_update_property_null(php_gd_webp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
}

PHP_METHOD(Gd_Webp_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Webp_Codec, fromString)
{
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_webp_decode_bytes(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_webp_throw("Failed to decode WebP image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Webp_Codec, fromFile)
{
	zend_string *path, *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_webp_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_webp_throw("Failed to decode WebP image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Webp_Codec, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_webp_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_webp_throw("Failed to decode WebP image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

static void php_gd_webp_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv, *destination, *options_zv = NULL;
	gdIOCtx *ctx;
	int size;
	void *data;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_webp_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		php_gd_webp_throw("Failed to open WebP output");
		RETURN_THROWS();
	}

	data = php_gd_webp_encode(image_zv, options_zv, &size);
	if (data == NULL || gdPutBuf(data, size, ctx) != size) {
		gdFree(data);
		ctx->gd_free(ctx);
		php_gd_webp_throw("Failed to encode WebP image");
		RETURN_THROWS();
	}
	gdFree(data);
	ctx->gd_free(ctx);
}

PHP_METHOD(Gd_Webp_Codec, toFile)
{
	php_gd_webp_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Webp_Codec, toStream)
{
	php_gd_webp_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Webp_Codec, toString)
{
	zval *image_zv, *options_zv = NULL;
	int size = 0;
	void *data;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_webp_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	data = php_gd_webp_encode(image_zv, options_zv, &size);
	if (data == NULL || size < 0) {
		gdFree(data);
		php_gd_webp_throw("Failed to encode WebP image");
		RETURN_THROWS();
	}

	RETVAL_STRINGL(data, size);
	gdFree(data);
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API) || defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static zend_class_entry *php_gd_webp_dispose_method_ce;
static zend_class_entry *php_gd_webp_blend_method_ce;

static void php_gd_webp_dispose(zval *result, int dispose)
{
	zend_long case_id;

	switch (dispose) {
		case gdWebpDisposeNone: case_id = ZEND_ENUM_Gd_Webp_DisposeMethod_None; break;
		case gdWebpDisposeBackground: case_id = ZEND_ENUM_Gd_Webp_DisposeMethod_Background; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_webp_dispose_method_ce, case_id));
}

static void php_gd_webp_blend(zval *result, int blend)
{
	zend_long case_id;

	switch (blend) {
		case gdWebpBlendAlpha: case_id = ZEND_ENUM_Gd_Webp_BlendMethod_Alpha; break;
		case gdWebpBlendNone: case_id = ZEND_ENUM_Gd_Webp_BlendMethod_None; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_webp_blend_method_ce, case_id));
}

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
static zend_class_entry *php_gd_webp_info_ce;
#ifdef HAVE_GD_BUNDLED
static zend_class_entry *php_gd_webp_reader_ce;
static zend_object_handlers php_gd_webp_reader_handlers;
#endif
#ifdef HAVE_GD_WEBP_ANIM_READ_API
static zend_class_entry *php_gd_webp_frame_ce;
static zend_class_entry *php_gd_webp_anim_reader_ce;
static zend_object_handlers php_gd_webp_anim_reader_handlers;
#endif

#ifdef HAVE_GD_BUNDLED
typedef struct {
	zend_string *bytes;
	zval info;
	bool read;
	bool failed;
	zend_object std;
} php_gd_webp_reader_object;

static php_gd_webp_reader_object *php_gd_webp_reader_from_object(zend_object *object)
{
	return (php_gd_webp_reader_object *) ((char *) object - offsetof(php_gd_webp_reader_object, std));
}

#define Z_GD_WEBP_READER_P(zv) php_gd_webp_reader_from_object(Z_OBJ_P((zv)))
#endif

#ifdef HAVE_GD_WEBP_ANIM_READ_API
typedef struct {
	gdWebpReadPtr reader;
	zval info;
	bool exhausted;
	bool failed;
	zend_object std;
} php_gd_webp_anim_reader_object;
#endif

#ifdef HAVE_GD_WEBP_ANIM_READ_API
static php_gd_webp_anim_reader_object *php_gd_webp_anim_reader_from_object(zend_object *object)
{
	return (php_gd_webp_anim_reader_object *) ((char *) object - offsetof(php_gd_webp_anim_reader_object, std));
}

#define Z_GD_WEBP_ANIM_READER_P(zv) php_gd_webp_anim_reader_from_object(Z_OBJ_P((zv)))
#endif

static void php_gd_webp_create_info(zval *result, const gdWebpInfo *info, gdImageMetadata *metadata)
{
	zval value;

	object_init_ex(result, php_gd_webp_info_ce);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("frameCount"), info->frame_count);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("loopCount"), info->loop_count);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("backgroundColor"), info->background_color);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("formatFlags"), info->format_flags);
	zend_update_property_bool(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("isAnimated"), info->is_animation != 0);
	php_gd_metadata_create_zval(&value, metadata);
	zend_update_property(php_gd_webp_info_ce, Z_OBJ_P(result), ZEND_STRL("metadata"), &value);
	zval_ptr_dtor(&value);
}

#ifdef HAVE_GD_WEBP_ANIM_READ_API
static void php_gd_webp_create_frame(zval *result, gdImagePtr image, const gdWebpFrameInfo *info)
{
	zval value;

	object_init_ex(result, php_gd_webp_frame_ce);
	php_gd_assign_libgdimageptr_as_extgdimage(&value, image);
	zend_update_property(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("image"), &value);
	zval_ptr_dtor(&value);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("frameIndex"), info->frame_index);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("x"), info->x);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("y"), info->y);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("durationMs"), info->duration);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("timestampMs"), info->timestamp);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("disposeTag"), info->dispose);
	php_gd_webp_dispose(&value, info->dispose);
	zend_update_property(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("dispose"), &value);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("blendTag"), info->blend);
	php_gd_webp_blend(&value, info->blend);
	zend_update_property(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("blend"), &value);
	zend_update_property_bool(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("hasAlpha"), info->has_alpha != 0);
	zend_update_property_bool(php_gd_webp_frame_ce, Z_OBJ_P(result), ZEND_STRL("complete"), info->complete != 0);
}
#endif

#ifdef HAVE_GD_BUNDLED
static zend_object *php_gd_webp_reader_create(zend_class_entry *class_entry)
{
	php_gd_webp_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_webp_reader_handlers;
	return &reader->std;
}

static void php_gd_webp_reader_free(zend_object *object)
{
	php_gd_webp_reader_object *reader = php_gd_webp_reader_from_object(object);

	if (reader->bytes != NULL) {
		zend_string_release(reader->bytes);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_webp_initialize_info_reader(zval *result, zend_string *bytes)
{
	gdWebpReadOptions options;
	gdWebpReadPtr webp;
	gdWebpInfo info;
	gdImageMetadata *metadata;
	php_gd_webp_reader_object *reader;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}
	metadata = gdImageMetadataCreate();
	if (metadata == NULL) {
		php_gd_webp_throw("Failed to allocate WebP metadata");
		return false;
	}
	gdWebpReadOptionsInit(&options);
	webp = gdWebpReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &options);
	if (webp == NULL || !gdWebpReadGetInfo(webp, &info) || gdWebpReadGetMetadata(webp, metadata) != GD_META_OK) {
		if (webp != NULL) gdWebpReadClose(webp);
		gdImageMetadataFree(metadata);
		php_gd_webp_throw("Failed to open WebP input");
		return false;
	}
	gdWebpReadClose(webp);

	object_init_ex(result, php_gd_webp_reader_ce);
	reader = Z_GD_WEBP_READER_P(result);
	reader->bytes = zend_string_copy(bytes);
	php_gd_webp_create_info(&reader->info, &info, metadata);
	return true;
}
#endif

#ifdef HAVE_GD_WEBP_ANIM_READ_API
static zend_object *php_gd_webp_anim_reader_create(zend_class_entry *class_entry)
{
	php_gd_webp_anim_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->reader = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->exhausted = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_webp_anim_reader_handlers;
	return &reader->std;
}

static void php_gd_webp_anim_reader_free(zend_object *object)
{
	php_gd_webp_anim_reader_object *reader = php_gd_webp_anim_reader_from_object(object);

	if (reader->reader != NULL) {
		gdWebpReadClose(reader->reader);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_webp_initialize_reader(zval *result, zend_string *bytes)
{
	gdWebpReadPtr webp;
	gdWebpReadOptions options;
	gdWebpInfo info;
	gdImageMetadata *metadata;
	php_gd_webp_anim_reader_object *reader;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}

	gdWebpReadOptionsInit(&options);
	options.coalesced = 1;
	metadata = gdImageMetadataCreate();
	if (metadata == NULL) {
		php_gd_webp_throw("Failed to allocate WebP metadata");
		return false;
	}
	webp = gdWebpReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &options);
	if (webp == NULL || !gdWebpReadGetInfo(webp, &info) || gdWebpReadGetMetadata(webp, metadata) != GD_META_OK) {
		if (webp != NULL) {
			gdWebpReadClose(webp);
		}
		gdImageMetadataFree(metadata);
		php_gd_webp_throw("Failed to open WebP input");
		return false;
	}

	object_init_ex(result, php_gd_webp_anim_reader_ce);
	reader = Z_GD_WEBP_ANIM_READER_P(result);
	reader->reader = webp;
	php_gd_webp_create_info(&reader->info, &info, metadata);
	return true;
}

static bool php_gd_webp_is_animated_bytes(zend_string *bytes)
{
	int result;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}
	result = gdWebpIsAnimatedPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
	if (result < 0) {
		php_gd_webp_throw("Failed to read WebP input");
		return false;
	}
	return result != 0;
}
#endif

PHP_METHOD(Gd_Webp_Info, __construct)
{
	zend_long width, height, frame_count, loop_count, background_color, format_flags;
	zend_bool is_animated;
	zval *metadata;

	ZEND_PARSE_PARAMETERS_START(8, 8)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(frame_count)
		Z_PARAM_LONG(loop_count)
		Z_PARAM_LONG(background_color)
		Z_PARAM_LONG(format_flags)
		Z_PARAM_BOOL(is_animated)
		Z_PARAM_OBJECT_OF_CLASS(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("frameCount"), frame_count);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("loopCount"), loop_count);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("backgroundColor"), background_color);
	zend_update_property_long(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("formatFlags"), format_flags);
	zend_update_property_bool(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("isAnimated"), is_animated);
	zend_update_property(php_gd_webp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata);
}

#ifdef HAVE_GD_BUNDLED
PHP_METHOD(Gd_Webp_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Webp_Reader, fromString)
{
	zend_string *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_webp_initialize_info_reader(return_value, bytes)) RETURN_THROWS();
}

PHP_METHOD(Gd_Webp_Reader, fromFile)
{
	zend_string *path, *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_webp_read_file_bytes(path, &bytes)) RETURN_THROWS();
	if (!php_gd_webp_initialize_info_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Webp_Reader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_webp_read_stream_bytes(stream_zv, &bytes)) RETURN_THROWS();
	if (!php_gd_webp_initialize_info_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Webp_Reader, info)
{
	php_gd_webp_reader_object *reader = Z_GD_WEBP_READER_P(ZEND_THIS);
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&reader->info);
}

PHP_METHOD(Gd_Webp_Reader, read)
{
	php_gd_webp_reader_object *reader = Z_GD_WEBP_READER_P(ZEND_THIS);
	gdWebpReadOptions options;
	gdWebpReadPtr webp;
	gdWebpFrameInfo info;
	gdImagePtr image = NULL;
	int result;

	ZEND_PARSE_PARAMETERS_NONE();
	if (reader->failed) {
		php_gd_webp_throw("WebP reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->read) {
		php_gd_webp_throw("WebP image has already been read");
		RETURN_THROWS();
	}
	gdWebpReadOptionsInit(&options);
	options.coalesced = 1;
	webp = gdWebpReadOpenPtr((int) ZSTR_LEN(reader->bytes), ZSTR_VAL(reader->bytes), &options);
	result = webp == NULL ? -1 : gdWebpReadNextImage(webp, &info, &image);
	if (webp != NULL) gdWebpReadClose(webp);
	reader->read = true;
	if (result != 1 || image == NULL) {
		reader->failed = true;
		php_gd_webp_throw("Failed to decode WebP image");
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}
#endif

#ifdef HAVE_GD_WEBP_ANIM_READ_API
PHP_METHOD(Gd_Webp_Frame, __construct)
{
	zval *image, *dispose = NULL, *blend = NULL;
	zend_long frame_index, x, y, width, height, duration_ms, timestamp_ms, dispose_tag, blend_tag;
	bool has_alpha, complete;

	ZEND_PARSE_PARAMETERS_START(14, 14)
		Z_PARAM_OBJECT_OF_CLASS(image, gd_image_ce)
		Z_PARAM_LONG(frame_index)
		Z_PARAM_LONG(x)
		Z_PARAM_LONG(y)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(duration_ms)
		Z_PARAM_LONG(timestamp_ms)
		Z_PARAM_LONG(dispose_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(dispose, php_gd_webp_dispose_method_ce)
		Z_PARAM_LONG(blend_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(blend, php_gd_webp_blend_method_ce)
		Z_PARAM_BOOL(has_alpha)
		Z_PARAM_BOOL(complete)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("image"), image);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("frameIndex"), frame_index);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("x"), x);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("y"), y);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("durationMs"), duration_ms);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("timestampMs"), timestamp_ms);
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("disposeTag"), dispose_tag);
	if (dispose) zend_update_property(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("dispose"), dispose); else zend_update_property_null(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("dispose"));
	zend_update_property_long(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("blendTag"), blend_tag);
	if (blend) zend_update_property(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("blend"), blend); else zend_update_property_null(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("blend"));
	zend_update_property_bool(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("hasAlpha"), has_alpha);
	zend_update_property_bool(php_gd_webp_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("complete"), complete);
}

PHP_METHOD(Gd_Webp_AnimReader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Webp_AnimReader, fromString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_initialize_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Webp_AnimReader, fromFile)
{
	zend_string *path, *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_webp_initialize_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Webp_AnimReader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_webp_initialize_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Webp_AnimReader, isAnimatedString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_BOOL(php_gd_webp_is_animated_bytes(bytes));
}

PHP_METHOD(Gd_Webp_AnimReader, isAnimatedFile)
{
	zend_string *path, *bytes;
	bool result;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	result = php_gd_webp_is_animated_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	RETURN_BOOL(result);
}

PHP_METHOD(Gd_Webp_AnimReader, isAnimatedStream)
{
	zval *stream_zv;
	zend_string *bytes;
	bool result;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_webp_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	result = php_gd_webp_is_animated_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	RETURN_BOOL(result);
}

PHP_METHOD(Gd_Webp_AnimReader, info)
{
	php_gd_webp_anim_reader_object *reader = Z_GD_WEBP_ANIM_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_OBJ_COPY(Z_OBJ(reader->info));
}

PHP_METHOD(Gd_Webp_AnimReader, next)
{
	php_gd_webp_anim_reader_object *reader = Z_GD_WEBP_ANIM_READER_P(ZEND_THIS);
	gdWebpFrameInfo info;
	gdImagePtr image = NULL;
	int result;

	ZEND_PARSE_PARAMETERS_NONE();

	if (reader->failed) {
		php_gd_webp_throw("WebP reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->exhausted) {
		RETURN_NULL();
	}

	result = gdWebpReadNextImage(reader->reader, &info, &image);
	if (result == 0) {
		reader->exhausted = true;
		RETURN_NULL();
	}
	if (result < 0 || image == NULL) {
		reader->failed = true;
		php_gd_webp_throw("Failed to decode WebP frame");
		RETURN_THROWS();
	}

	php_gd_webp_create_frame(return_value, image, &info);
}
#endif

#ifdef HAVE_GD_WEBP_ANIM_WRITE_API
static zend_class_entry *php_gd_webp_anim_write_options_ce;
static zend_class_entry *php_gd_webp_anim_writer_ce;
static zend_object_handlers php_gd_webp_anim_writer_handlers;

typedef enum {
	PHP_GD_WEBP_DESTINATION_CONTEXT,
	PHP_GD_WEBP_DESTINATION_MEMORY
} php_gd_webp_destination;

typedef struct {
	gdWebpWritePtr writer;
	gdIOCtx *ctx;
	php_gd_webp_destination destination;
	bool finalized;
	zend_long frame_count;
	int canvas_width;
	int canvas_height;
	zend_object std;
} php_gd_webp_anim_writer_object;

static php_gd_webp_anim_writer_object *php_gd_webp_anim_writer_from_object(zend_object *object)
{
	return (php_gd_webp_anim_writer_object *) ((char *) object - offsetof(php_gd_webp_anim_writer_object, std));
}

#define Z_GD_WEBP_ANIM_WRITER_P(zv) php_gd_webp_anim_writer_from_object(Z_OBJ_P((zv)))

static zend_object *php_gd_webp_anim_writer_create(zend_class_entry *class_entry)
{
	php_gd_webp_anim_writer_object *writer = zend_object_alloc(sizeof(*writer), class_entry);

	writer->writer = NULL;
	writer->ctx = NULL;
	writer->destination = PHP_GD_WEBP_DESTINATION_CONTEXT;
	writer->finalized = false;
	writer->frame_count = 0;
	writer->canvas_width = 0;
	writer->canvas_height = 0;
	zend_object_std_init(&writer->std, class_entry);
	object_properties_init(&writer->std, class_entry);
	writer->std.handlers = &php_gd_webp_anim_writer_handlers;
	return &writer->std;
}

static void php_gd_webp_writer_close(php_gd_webp_anim_writer_object *writer)
{
	if (writer->writer != NULL) {
		gdWebpWriteClose(writer->writer);
		writer->writer = NULL;
	}
	if (writer->ctx != NULL) {
		writer->ctx->gd_free(writer->ctx);
		writer->ctx = NULL;
	}
}

static void php_gd_webp_anim_writer_free(zend_object *object)
{
	php_gd_webp_anim_writer_object *writer = php_gd_webp_anim_writer_from_object(object);

	php_gd_webp_writer_close(writer);
	zend_object_std_dtor(&writer->std);
}

static void php_gd_webp_build_options(zval *options_zv, gdWebpAnimWriteOptions *options)
{
	zval rv;
	zval *value;

	gdWebpAnimWriteOptionsInit(options);
	options->method = 4;
	options->kmin = 9;
	options->kmax = 17;
	if (options_zv == NULL) {
		return;
	}

	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("canvasWidth"), true, &rv);
	options->canvas_width = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("canvasHeight"), true, &rv);
	options->canvas_height = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("loopCount"), true, &rv);
	options->loop_count = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("backgroundColor"), true, &rv);
	options->background_color = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("quality"), true, &rv);
	options->quality = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("lossless"), true, &rv);
	options->lossless = Z_TYPE_P(value) == IS_TRUE;
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("method"), true, &rv);
	options->method = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("minimizeSize"), true, &rv);
	options->minimize_size = Z_TYPE_P(value) == IS_TRUE;
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("kmin"), true, &rv);
	options->kmin = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("kmax"), true, &rv);
	options->kmax = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_webp_anim_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("allowMixed"), true, &rv);
	options->allow_mixed = Z_TYPE_P(value) == IS_TRUE;
}

static bool php_gd_webp_create_writer(zval *return_value, gdWebpWritePtr webp,
		gdIOCtx *ctx, php_gd_webp_destination destination, const gdWebpAnimWriteOptions *options)
{
	php_gd_webp_anim_writer_object *writer;

	if (webp == NULL) {
		if (ctx != NULL) {
			ctx->gd_free(ctx);
		}
		php_gd_webp_throw("Failed to initialize WebP output");
		return false;
	}

	object_init_ex(return_value, php_gd_webp_anim_writer_ce);
	writer = Z_GD_WEBP_ANIM_WRITER_P(return_value);
	writer->writer = webp;
	writer->ctx = ctx;
	writer->destination = destination;
	writer->canvas_width = options != NULL ? options->canvas_width : 0;
	writer->canvas_height = options != NULL ? options->canvas_height : 0;
	return true;
}

PHP_METHOD(Gd_Webp_AnimWriteOptions, __construct)
{
	zend_long canvas_width = 0, canvas_height = 0, loop_count = 0, background_color = 0;
	zend_long quality = -1, method = 4, kmin = 9, kmax = 17;
	bool lossless = false, minimize_size = false, allow_mixed = false;

	ZEND_PARSE_PARAMETERS_START(0, 11)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(canvas_width)
		Z_PARAM_LONG(canvas_height)
		Z_PARAM_LONG(loop_count)
		Z_PARAM_LONG(background_color)
		Z_PARAM_LONG(quality)
		Z_PARAM_BOOL(lossless)
		Z_PARAM_LONG(method)
		Z_PARAM_BOOL(minimize_size)
		Z_PARAM_LONG(kmin)
		Z_PARAM_LONG(kmax)
		Z_PARAM_BOOL(allow_mixed)
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
	if (background_color < 0 || (zend_ulong) background_color > UINT32_MAX) {
		zend_argument_value_error(4, "must be between 0 and 4294967295");
		RETURN_THROWS();
	}
	if (quality < -1 || quality > 100) {
		zend_argument_value_error(5, "must be between -1 and 100");
		RETURN_THROWS();
	}
	if (method < 0 || method > 6) {
		zend_argument_value_error(7, "must be between 0 and 6");
		RETURN_THROWS();
	}
	if (kmin < 0 || kmin > INT_MAX) {
		zend_argument_value_error(9, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (kmax < 0 || kmax > INT_MAX) {
		zend_argument_value_error(10, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("canvasWidth"), canvas_width);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("canvasHeight"), canvas_height);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("loopCount"), loop_count);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("backgroundColor"), background_color);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("quality"), quality);
	zend_update_property_bool(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("lossless"), lossless);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("method"), method);
	zend_update_property_bool(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("minimizeSize"), minimize_size);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("kmin"), kmin);
	zend_update_property_long(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("kmax"), kmax);
	zend_update_property_bool(php_gd_webp_anim_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("allowMixed"), allow_mixed);
}

PHP_METHOD(Gd_Webp_AnimWriter, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Webp_AnimWriter, toFile)
{
	zval *path_zv;
	zval *options_zv = NULL;
	gdWebpAnimWriteOptions options;
	gdIOCtx *ctx;
	gdWebpWritePtr webp;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(path_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_webp_anim_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	ZEND_ASSERT(Z_TYPE_P(path_zv) == IS_STRING);
	ctx = php_gd_create_output_context(path_zv, 1);
	if (ctx == NULL) {
		php_gd_webp_throw("Failed to open WebP output");
		RETURN_THROWS();
	}
	php_gd_webp_build_options(options_zv, &options);
	webp = gdWebpWriteOpenCtx(ctx, &options);
	if (!php_gd_webp_create_writer(return_value, webp, ctx, PHP_GD_WEBP_DESTINATION_CONTEXT, &options)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Webp_AnimWriter, toStream)
{
	zval *stream_zv;
	zval *options_zv = NULL;
	gdWebpAnimWriteOptions options;
	gdIOCtx *ctx;
	gdWebpWritePtr webp;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_webp_anim_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ctx = php_gd_create_output_context(stream_zv, 1);
	if (ctx == NULL) {
		php_gd_webp_throw("Failed to open WebP output");
		RETURN_THROWS();
	}
	php_gd_webp_build_options(options_zv, &options);
	webp = gdWebpWriteOpenCtx(ctx, &options);
	if (!php_gd_webp_create_writer(return_value, webp, ctx, PHP_GD_WEBP_DESTINATION_CONTEXT, &options)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Webp_AnimWriter, toMemory)
{
	zval *options_zv = NULL;
	gdWebpAnimWriteOptions options;
	gdWebpWritePtr webp;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_webp_anim_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_webp_build_options(options_zv, &options);
	webp = gdWebpWriteOpenPtr(&options);
	if (!php_gd_webp_create_writer(return_value, webp, NULL, PHP_GD_WEBP_DESTINATION_MEMORY, &options)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Webp_AnimWriter, addFrame)
{
	php_gd_webp_anim_writer_object *writer = Z_GD_WEBP_ANIM_WRITER_P(ZEND_THIS);
	zval *image_zv;
	zend_long duration_ms;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_LONG(duration_ms)
	ZEND_PARSE_PARAMETERS_END();

	if (writer->finalized) {
		php_gd_webp_throw("WebP writer has already been finalized");
		RETURN_THROWS();
	}
	if (duration_ms < 0 || duration_ms > INT_MAX) {
		zend_argument_value_error(2, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}

	image = php_gd_libgdimageptr_from_zval_p(image_zv);
	if (!image->trueColor) {
		php_gd_webp_throw("WebP animation writer only supports truecolor images");
		RETURN_THROWS();
	}
	if (writer->frame_count == 0) {
		if (writer->canvas_width <= 0) {
			writer->canvas_width = image->sx;
		}
		if (writer->canvas_height <= 0) {
			writer->canvas_height = image->sy;
		}
	}
	if (image->sx != writer->canvas_width || image->sy != writer->canvas_height) {
		php_gd_webp_throw("WebP animation frames must match canvas size");
		RETURN_THROWS();
	}
	if (!gdWebpWriteAddImage(writer->writer, image, (int) duration_ms)) {
		php_gd_webp_throw("Failed to add WebP frame");
		RETURN_THROWS();
	}
	writer->frame_count++;
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Webp_AnimWriter, finish)
{
	php_gd_webp_anim_writer_object *writer = Z_GD_WEBP_ANIM_WRITER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (writer->finalized) {
		php_gd_webp_throw("WebP writer has already been finalized");
		RETURN_THROWS();
	}
	if (writer->frame_count == 0) {
		php_gd_webp_throw("Cannot finalize a WebP writer without frames");
		RETURN_THROWS();
	}

	writer->finalized = true;
	if (writer->destination == PHP_GD_WEBP_DESTINATION_MEMORY) {
		int size = 0;
		void *data = gdWebpWritePtrFinish(writer->writer, &size);
		writer->writer = NULL;
		if (data == NULL || size < 0) {
			gdFree(data);
			php_gd_webp_throw("Failed to finalize WebP output");
			RETURN_THROWS();
		}
		RETVAL_STRINGL(data, size);
		gdFree(data);
		return;
	}

	gdWebpWriteClose(writer->writer);
	writer->writer = NULL;
	if (writer->ctx != NULL) {
		writer->ctx->gd_free(writer->ctx);
		writer->ctx = NULL;
	}
	RETURN_NULL();
}
#endif

#endif /* HAVE_GD_BUNDLED || HAVE_GD_WEBP_ANIM_READ_API */

#endif /* HAVE_GD_WEBP_ANIM_READ_API || HAVE_GD_WEBP_ANIM_WRITE_API */

void php_gd_webp_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#if defined(HAVE_GD_WEBP_ANIM_READ_API) || defined(HAVE_GD_WEBP_ANIM_WRITE_API)
	php_gd_webp_dispose_method_ce = register_class_Gd_Webp_DisposeMethod();
	php_gd_webp_blend_method_ce = register_class_Gd_Webp_BlendMethod();
#endif

#ifdef HAVE_GD_WEBP
	zend_class_entry *codec_ce;

	php_gd_webp_write_options_ce = register_class_Gd_Webp_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Webp_Codec();
	php_gd_register_codec_write(php_gd_webp_write_options_ce, codec_ce);
	php_gd_register_codec_format("Webp", php_gd_webp_write_options_ce);
	php_gd_register_codec_extension("webp", php_gd_webp_write_options_ce);
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
	register_class_Gd_Webp_FormatFlag();
	php_gd_webp_info_ce = register_class_Gd_Webp_Info();
#ifdef HAVE_GD_BUNDLED
	php_gd_webp_reader_ce = register_class_Gd_Webp_Reader();
	php_gd_webp_reader_ce->create_object = php_gd_webp_reader_create;

	memcpy(&php_gd_webp_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_webp_reader_handlers.offset = offsetof(php_gd_webp_reader_object, std);
	php_gd_webp_reader_handlers.free_obj = php_gd_webp_reader_free;
	php_gd_webp_reader_handlers.clone_obj = NULL;
#endif
#ifdef HAVE_GD_WEBP_ANIM_READ_API
	php_gd_webp_frame_ce = register_class_Gd_Webp_Frame();
	php_gd_webp_anim_reader_ce = register_class_Gd_Webp_AnimReader();
	php_gd_webp_anim_reader_ce->create_object = php_gd_webp_anim_reader_create;

	memcpy(&php_gd_webp_anim_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_webp_anim_reader_handlers.offset = offsetof(php_gd_webp_anim_reader_object, std);
	php_gd_webp_anim_reader_handlers.free_obj = php_gd_webp_anim_reader_free;
	php_gd_webp_anim_reader_handlers.clone_obj = NULL;
#endif
#endif

#ifdef HAVE_GD_WEBP_ANIM_WRITE_API
	php_gd_webp_anim_write_options_ce = register_class_Gd_Webp_AnimWriteOptions();
	php_gd_webp_anim_writer_ce = register_class_Gd_Webp_AnimWriter();
	php_gd_webp_anim_writer_ce->create_object = php_gd_webp_anim_writer_create;

	memcpy(&php_gd_webp_anim_writer_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_webp_anim_writer_handlers.offset = offsetof(php_gd_webp_anim_writer_object, std);
	php_gd_webp_anim_writer_handlers.free_obj = php_gd_webp_anim_writer_free;
	php_gd_webp_anim_writer_handlers.clone_obj = NULL;
#endif
}
