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
#include "gd_gif.h"
#include "ext/spl/spl_exceptions.h"

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_gif_arginfo.h"
#include "gd_gif_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;
static zend_class_entry *php_gd_gif_write_options_ce;

static void php_gd_gif_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

PHP_METHOD(Gd_Gif_WriteOptions, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

static zend_string *php_gd_gif_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_gif_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_gif_throw("Failed to open GIF input");
		return false;
	}
	*bytes = php_gd_gif_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_gif_throw("Failed to read GIF input");
		return false;
	}
	return true;
}

static bool php_gd_gif_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_gif_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_gif_throw("Failed to read GIF input");
		return false;
	}
	return true;
}

static bool php_gd_gif_bytes_are_animated(zend_string *bytes)
{
	const unsigned char *data = (const unsigned char *) ZSTR_VAL(bytes);
	size_t len = ZSTR_LEN(bytes);
	size_t pos;
	unsigned int frames = 0;

	if (len < 13 || (memcmp(data, "GIF87a", 6) != 0 && memcmp(data, "GIF89a", 6) != 0)) {
		return false;
	}

	pos = 13;
	if (data[10] & 0x80) {
		size_t color_table_size = 3u * (1u << ((data[10] & 0x07) + 1));
		if (color_table_size > len - pos) {
			return false;
		}
		pos += color_table_size;
	}

	while (pos < len) {
		unsigned char marker = data[pos++];

		if (marker == 0x3b) {
			return false;
		}
		if (marker == 0x21) {
			pos++;
			while (pos < len) {
				unsigned int block_size = data[pos++];
				if (block_size == 0) {
					break;
				}
				if (block_size > len - pos) {
					return false;
				}
				pos += block_size;
			}
			continue;
		}
		if (marker == 0x2c) {
			if (++frames > 1) {
				return true;
			}
			if (9 > len - pos) {
				return false;
			}
			if (data[pos + 8] & 0x80) {
				size_t color_table_size = 3u * (1u << ((data[pos + 8] & 0x07) + 1));
				if (9 + color_table_size > len - pos) {
					return false;
				}
				pos += 9 + color_table_size;
			} else {
				pos += 9;
			}
			if (pos >= len) {
				return false;
			}
			pos++;
			while (pos < len) {
				unsigned int block_size = data[pos++];
				if (block_size == 0) {
					break;
				}
				if (block_size > len - pos) {
					return false;
				}
				pos += block_size;
			}
			continue;
		}

		return false;
	}

	return false;
}

static gdImagePtr php_gd_gif_decode_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return NULL;
	}
	if (php_gd_gif_bytes_are_animated(bytes)) {
		php_gd_gif_throw("GIF input contains multiple frames; use Gd\\Gif\\AnimReader");
		return NULL;
	}
	return gdImageCreateFromGifPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
}

static void php_gd_gif_with_stable_quantization(gdImagePtr image,
		void (*write)(gdImagePtr image, void *context), void *context)
{
	int quantization_method = image->paletteQuantizationMethod;
	int quantization_speed = image->paletteQuantizationSpeed;

	if (image->trueColor) {
		gdImageTrueColorToPaletteSetMethod(image, GD_QUANT_JQUANT, 0);
	}
	write(image, context);
	if (image->trueColor) {
		gdImageTrueColorToPaletteSetMethod(image, quantization_method, quantization_speed);
	}
}

static void php_gd_gif_write_ctx_callback(gdImagePtr image, void *context)
{
	gdImageGifCtx(image, (gdIOCtx *) context);
}

typedef struct {
	int *size;
	void *data;
} php_gd_gif_ptr_context;

static void php_gd_gif_write_ptr_callback(gdImagePtr image, void *context)
{
	php_gd_gif_ptr_context *ptr_context = (php_gd_gif_ptr_context *) context;
	ptr_context->data = gdImageGifPtr(image, ptr_context->size);
}

PHP_METHOD(Gd_Gif_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Gif_Codec, fromString)
{
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_gif_decode_bytes(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_gif_throw("Failed to decode GIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Gif_Codec, fromFile)
{
	zend_string *path, *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_gif_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_gif_throw("Failed to decode GIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Gif_Codec, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_gif_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_gif_throw("Failed to decode GIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

static void php_gd_gif_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv, *destination, *options_zv = NULL;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_gif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		php_gd_gif_throw("Failed to open GIF output");
		RETURN_THROWS();
	}

	php_gd_gif_with_stable_quantization(
		php_gd_libgdimageptr_from_zval_p(image_zv),
		php_gd_gif_write_ctx_callback,
		ctx);
	ctx->gd_free(ctx);
}

PHP_METHOD(Gd_Gif_Codec, toFile)
{
	php_gd_gif_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Gif_Codec, toStream)
{
	php_gd_gif_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Gif_Codec, toString)
{
	zval *image_zv;
	zval *options_zv = NULL;
	int size = 0;
	php_gd_gif_ptr_context context = {&size, NULL};

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_gif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_gif_with_stable_quantization(
		php_gd_libgdimageptr_from_zval_p(image_zv),
		php_gd_gif_write_ptr_callback,
		&context);
	if (context.data == NULL || size < 0) {
		gdFree(context.data);
		php_gd_gif_throw("Failed to encode GIF image");
		RETURN_THROWS();
	}

	RETVAL_STRINGL(context.data, size);
	gdFree(context.data);
}

#if defined(HAVE_GD_GIF_ANIM_READ_API) || defined(HAVE_GD_GIF_ANIM_WRITE_API)
static zend_class_entry *php_gd_gif_disposal_method_ce;

static void php_gd_gif_disposal(zval *result, int disposal)
{
	zend_long case_id;

	switch (disposal) {
		case GD_GIF_DISPOSAL_UNKNOWN: case_id = ZEND_ENUM_Gd_Gif_DisposalMethod_Unknown; break;
		case GD_GIF_DISPOSAL_NONE: case_id = ZEND_ENUM_Gd_Gif_DisposalMethod_None; break;
		case GD_GIF_DISPOSAL_RESTORE_BACKGROUND: case_id = ZEND_ENUM_Gd_Gif_DisposalMethod_RestoreBackground; break;
		case GD_GIF_DISPOSAL_RESTORE_PREVIOUS: case_id = ZEND_ENUM_Gd_Gif_DisposalMethod_RestorePrevious; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_gif_disposal_method_ce, case_id));
}

static int php_gd_gif_disposal_value(zval *value)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Gif_DisposalMethod_Unknown:
			return GD_GIF_DISPOSAL_UNKNOWN;
		case ZEND_ENUM_Gd_Gif_DisposalMethod_RestoreBackground:
			return GD_GIF_DISPOSAL_RESTORE_BACKGROUND;
		case ZEND_ENUM_Gd_Gif_DisposalMethod_RestorePrevious:
			return GD_GIF_DISPOSAL_RESTORE_PREVIOUS;
		case ZEND_ENUM_Gd_Gif_DisposalMethod_None:
		default:
			return GD_GIF_DISPOSAL_NONE;
	}
}

#endif

#ifdef HAVE_GD_GIF_ANIM_READ_API
static zend_class_entry *php_gd_gif_info_ce;
static zend_class_entry *php_gd_gif_frame_ce;
static zend_class_entry *php_gd_gif_anim_reader_ce;
static zend_object_handlers php_gd_gif_anim_reader_handlers;

typedef struct {
	gdGifReadPtr reader;
	zend_string *bytes;
	zval info;
	bool exhausted;
	bool failed;
	zend_object std;
} php_gd_gif_anim_reader_object;

static php_gd_gif_anim_reader_object *php_gd_gif_anim_reader_from_object(zend_object *object)
{
	return (php_gd_gif_anim_reader_object *) ((char *) object - offsetof(php_gd_gif_anim_reader_object, std));
}

#define Z_GD_GIF_ANIM_READER_P(zv) php_gd_gif_anim_reader_from_object(Z_OBJ_P((zv)))

static void php_gd_gif_create_info(zval *result, const gdGifInfo *info)
{
	object_init_ex(result, php_gd_gif_info_ce);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(result), ZEND_STRL("backgroundIndex"), info->backgroundIndex);
	zend_update_property_bool(php_gd_gif_info_ce, Z_OBJ_P(result), ZEND_STRL("globalColorTable"), info->globalColorTable != 0);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(result), ZEND_STRL("loopCount"), info->loopCount);
}

static void php_gd_gif_create_frame(zval *result, gdImagePtr image, const gdGifFrameInfo *info)
{
	zval value;

	object_init_ex(result, php_gd_gif_frame_ce);
	php_gd_assign_libgdimageptr_as_extgdimage(&value, image);
	zend_update_property(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("image"), &value);
	zval_ptr_dtor(&value);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("frameIndex"), info->frameIndex);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("x"), info->x);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("y"), info->y);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("delayMs"), info->delay * 10);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("disposalTag"), info->disposal);
	php_gd_gif_disposal(&value, info->disposal);
	zend_update_property(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("disposal"), &value);
	if (info->transparentIndex >= 0) {
		zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("transparentIndex"), info->transparentIndex);
	} else {
		zend_update_property_null(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("transparentIndex"));
	}
	zend_update_property_bool(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("localColorTable"), info->localColorTable != 0);
	zend_update_property_bool(php_gd_gif_frame_ce, Z_OBJ_P(result), ZEND_STRL("interlaced"), info->interlace != 0);
}

static zend_object *php_gd_gif_anim_reader_create(zend_class_entry *class_entry)
{
	php_gd_gif_anim_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->reader = NULL;
	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->exhausted = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_gif_anim_reader_handlers;
	return &reader->std;
}

static void php_gd_gif_anim_reader_free(zend_object *object)
{
	php_gd_gif_anim_reader_object *reader = php_gd_gif_anim_reader_from_object(object);

	if (reader->reader != NULL) {
		gdGifReadClose(reader->reader);
	}
	if (reader->bytes != NULL) {
		zend_string_release_ex(reader->bytes, 0);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_gif_initialize_reader(zval *result, zend_string *bytes)
{
	gdGifReadPtr gif;
	gdGifInfo info;
	php_gd_gif_anim_reader_object *reader;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}

	gif = gdGifReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
	if (gif == NULL || !gdGifReadGetInfo(gif, &info)) {
		if (gif != NULL) {
			gdGifReadClose(gif);
		}
		php_gd_gif_throw("Failed to open GIF input");
		return false;
	}

	object_init_ex(result, php_gd_gif_anim_reader_ce);
	reader = Z_GD_GIF_ANIM_READER_P(result);
	reader->reader = gif;
	reader->bytes = zend_string_copy(bytes);
	php_gd_gif_create_info(&reader->info, &info);
	return true;
}

static bool php_gd_gif_is_animated_bytes(zend_string *bytes)
{
	int result;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return false;
	}
	result = gdGifIsAnimatedPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
	if (result < 0) {
		php_gd_gif_throw("Failed to read GIF input");
		return false;
	}
	return result != 0;
}

PHP_METHOD(Gd_Gif_Info, __construct)
{
	zend_long width, height, background_index, loop_count;
	bool global_color_table;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(background_index)
		Z_PARAM_BOOL(global_color_table)
		Z_PARAM_LONG(loop_count)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("backgroundIndex"), background_index);
	zend_update_property_bool(php_gd_gif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("globalColorTable"), global_color_table);
	zend_update_property_long(php_gd_gif_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("loopCount"), loop_count);
}

PHP_METHOD(Gd_Gif_Frame, __construct)
{
	zval *image, *disposal = NULL;
	zend_long frame_index, x, y, width, height, delay_ms, disposal_tag, transparent_index = 0;
	bool transparent_is_null = true, local_color_table, interlaced;

	ZEND_PARSE_PARAMETERS_START(12, 12)
		Z_PARAM_OBJECT_OF_CLASS(image, gd_image_ce)
		Z_PARAM_LONG(frame_index)
		Z_PARAM_LONG(x)
		Z_PARAM_LONG(y)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(delay_ms)
		Z_PARAM_LONG(disposal_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(disposal, php_gd_gif_disposal_method_ce)
		Z_PARAM_LONG_OR_NULL(transparent_index, transparent_is_null)
		Z_PARAM_BOOL(local_color_table)
		Z_PARAM_BOOL(interlaced)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("image"), image);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("frameIndex"), frame_index);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("x"), x);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("y"), y);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("delayMs"), delay_ms);
	zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("disposalTag"), disposal_tag);
	if (disposal) zend_update_property(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("disposal"), disposal); else zend_update_property_null(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("disposal"));
	if (transparent_is_null) zend_update_property_null(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("transparentIndex")); else zend_update_property_long(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("transparentIndex"), transparent_index);
	zend_update_property_bool(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("localColorTable"), local_color_table);
	zend_update_property_bool(php_gd_gif_frame_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interlaced"), interlaced);
}

PHP_METHOD(Gd_Gif_AnimReader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Gif_AnimReader, fromString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_initialize_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Gif_AnimReader, fromFile)
{
	zend_string *path, *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_gif_initialize_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Gif_AnimReader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_gif_initialize_reader(return_value, bytes)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Gif_AnimReader, isAnimatedString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_BOOL(php_gd_gif_is_animated_bytes(bytes));
}

PHP_METHOD(Gd_Gif_AnimReader, isAnimatedFile)
{
	zend_string *path, *bytes;
	bool result;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	result = php_gd_gif_is_animated_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	RETURN_BOOL(result);
}

PHP_METHOD(Gd_Gif_AnimReader, isAnimatedStream)
{
	zval *stream_zv;
	zend_string *bytes;
	bool result;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_gif_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	result = php_gd_gif_is_animated_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	RETURN_BOOL(result);
}

PHP_METHOD(Gd_Gif_AnimReader, info)
{
	php_gd_gif_anim_reader_object *reader = Z_GD_GIF_ANIM_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_OBJ_COPY(Z_OBJ(reader->info));
}

PHP_METHOD(Gd_Gif_AnimReader, next)
{
	php_gd_gif_anim_reader_object *reader = Z_GD_GIF_ANIM_READER_P(ZEND_THIS);
	gdGifFrameInfo info;
	gdImagePtr image = NULL;
	int result;

	ZEND_PARSE_PARAMETERS_NONE();

	if (reader->failed) {
		php_gd_gif_throw("GIF reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->exhausted) {
		RETURN_NULL();
	}

	result = gdGifReadNextImage(reader->reader, &info, &image);
	if (result == 0) {
		reader->exhausted = true;
		RETURN_NULL();
	}
	if (result < 0 || image == NULL) {
		reader->failed = true;
		php_gd_gif_throw("Failed to decode GIF frame");
		RETURN_THROWS();
	}

	php_gd_gif_create_frame(return_value, image, &info);
}
#endif

#ifdef HAVE_GD_GIF_ANIM_WRITE_API
static zend_class_entry *php_gd_gif_frame_options_ce;
static zend_class_entry *php_gd_gif_anim_writer_ce;
static zend_object_handlers php_gd_gif_anim_writer_handlers;

typedef enum {
	PHP_GD_GIF_DESTINATION_CONTEXT,
	PHP_GD_GIF_DESTINATION_MEMORY
} php_gd_gif_destination;

typedef struct {
	gdIOCtx *ctx;
	php_gd_gif_destination destination;
	bool global_color_map;
	int loops;
	bool started;
	bool finalized;
	zend_long frame_count;
	int canvas_width;
	int canvas_height;
	gdImagePtr previous;
	zend_object std;
} php_gd_gif_anim_writer_object;

static php_gd_gif_anim_writer_object *php_gd_gif_anim_writer_from_object(zend_object *object)
{
	return (php_gd_gif_anim_writer_object *) ((char *) object - offsetof(php_gd_gif_anim_writer_object, std));
}

#define Z_GD_GIF_ANIM_WRITER_P(zv) php_gd_gif_anim_writer_from_object(Z_OBJ_P((zv)))

static zend_object *php_gd_gif_anim_writer_create(zend_class_entry *class_entry)
{
	php_gd_gif_anim_writer_object *writer = zend_object_alloc(sizeof(*writer), class_entry);

	writer->ctx = NULL;
	writer->destination = PHP_GD_GIF_DESTINATION_CONTEXT;
	writer->global_color_map = true;
	writer->loops = 0;
	writer->started = false;
	writer->finalized = false;
	writer->frame_count = 0;
	writer->canvas_width = 0;
	writer->canvas_height = 0;
	writer->previous = NULL;
	zend_object_std_init(&writer->std, class_entry);
	object_properties_init(&writer->std, class_entry);
	writer->std.handlers = &php_gd_gif_anim_writer_handlers;
	return &writer->std;
}

static void php_gd_gif_anim_writer_free(zend_object *object)
{
	php_gd_gif_anim_writer_object *writer = php_gd_gif_anim_writer_from_object(object);

	if (writer->ctx != NULL) {
		writer->ctx->gd_free(writer->ctx);
	}
	if (writer->previous != NULL) {
		gdImageDestroy(writer->previous);
	}
	zend_object_std_dtor(&writer->std);
}

static void php_gd_gif_create_writer(zval *return_value, gdIOCtx *ctx,
		php_gd_gif_destination destination, bool global_color_map, zend_long loops)
{
	php_gd_gif_anim_writer_object *writer;

	object_init_ex(return_value, php_gd_gif_anim_writer_ce);
	writer = Z_GD_GIF_ANIM_WRITER_P(return_value);
	writer->ctx = ctx;
	writer->destination = destination;
	writer->global_color_map = global_color_map;
	writer->loops = (int) loops;
}

static gdImagePtr php_gd_gif_begin_image(gdImagePtr image)
{
	if (image->trueColor) {
		return gdImageCreatePaletteFromTrueColor(image, 1, 256);
	}
	return image;
}

PHP_METHOD(Gd_Gif_FrameOptions, __construct)
{
	zend_long x = 0, y = 0, delay_ms = 0;
	zval *disposal = NULL;
	bool local_color_map = false, optimize_against_previous = true;
	zval default_value;

	ZEND_PARSE_PARAMETERS_START(0, 6)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(x)
		Z_PARAM_LONG(y)
		Z_PARAM_LONG(delay_ms)
		Z_PARAM_OBJECT_OF_CLASS(disposal, php_gd_gif_disposal_method_ce)
		Z_PARAM_BOOL(local_color_map)
		Z_PARAM_BOOL(optimize_against_previous)
	ZEND_PARSE_PARAMETERS_END();

	if (x < 0 || x > INT_MAX) {
		zend_argument_value_error(1, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (y < 0 || y > INT_MAX) {
		zend_argument_value_error(2, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (delay_ms < 0 || delay_ms > INT_MAX || delay_ms % 10 != 0) {
		zend_argument_value_error(3, "must be a non-negative millisecond value divisible by 10 and not greater than %d", INT_MAX);
		RETURN_THROWS();
	}
	if (disposal == NULL) {
		ZVAL_OBJ(&default_value, zend_enum_get_case_by_id(php_gd_gif_disposal_method_ce, ZEND_ENUM_Gd_Gif_DisposalMethod_None));
		disposal = &default_value;
	}

	zend_update_property_long(php_gd_gif_frame_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("x"), x);
	zend_update_property_long(php_gd_gif_frame_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("y"), y);
	zend_update_property_long(php_gd_gif_frame_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("delayMs"), delay_ms);
	zend_update_property(php_gd_gif_frame_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("disposal"), disposal);
	zend_update_property_bool(php_gd_gif_frame_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("localColorMap"), local_color_map);
	zend_update_property_bool(php_gd_gif_frame_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("optimizeAgainstPrevious"), optimize_against_previous);
}

PHP_METHOD(Gd_Gif_AnimWriter, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Gif_AnimWriter, toFile)
{
	zend_string *path;
	bool global_color_map = true;
	zend_long loops = 0;
	gdIOCtx *ctx;
	zval path_zv;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(global_color_map)
		Z_PARAM_LONG(loops)
	ZEND_PARSE_PARAMETERS_END();

	if (loops < 0 || loops > 65535) {
		zend_argument_value_error(3, "must be between 0 and 65535");
		RETURN_THROWS();
	}

	ZVAL_STR(&path_zv, path);
	ctx = php_gd_create_output_context(&path_zv, 1);
	if (ctx == NULL) {
		php_gd_gif_throw("Failed to open GIF output");
		RETURN_THROWS();
	}
	php_gd_gif_create_writer(return_value, ctx, PHP_GD_GIF_DESTINATION_CONTEXT, global_color_map, loops);
}

PHP_METHOD(Gd_Gif_AnimWriter, toStream)
{
	zval *stream_zv;
	bool global_color_map = true;
	zend_long loops = 0;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(global_color_map)
		Z_PARAM_LONG(loops)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}
	if (loops < 0 || loops > 65535) {
		zend_argument_value_error(3, "must be between 0 and 65535");
		RETURN_THROWS();
	}

	ctx = php_gd_create_output_context(stream_zv, 1);
	if (ctx == NULL) {
		php_gd_gif_throw("Failed to open GIF output");
		RETURN_THROWS();
	}
	php_gd_gif_create_writer(return_value, ctx, PHP_GD_GIF_DESTINATION_CONTEXT, global_color_map, loops);
}

PHP_METHOD(Gd_Gif_AnimWriter, toMemory)
{
	bool global_color_map = true;
	zend_long loops = 0;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(global_color_map)
		Z_PARAM_LONG(loops)
	ZEND_PARSE_PARAMETERS_END();

	if (loops < 0 || loops > 65535) {
		zend_argument_value_error(2, "must be between 0 and 65535");
		RETURN_THROWS();
	}

	ctx = gdNewDynamicCtxEx(2048, NULL, 1);
	if (ctx == NULL) {
		php_gd_gif_throw("Failed to initialize GIF output");
		RETURN_THROWS();
	}
	php_gd_gif_create_writer(return_value, ctx, PHP_GD_GIF_DESTINATION_MEMORY, global_color_map, loops);
}

PHP_METHOD(Gd_Gif_AnimWriter, addFrame)
{
	php_gd_gif_anim_writer_object *writer = Z_GD_GIF_ANIM_WRITER_P(ZEND_THIS);
	zval *image_zv, *options_zv = NULL, tmp;
	gdImagePtr image, previous = NULL, begin_image = NULL, previous_clone;
	zend_long x = 0, y = 0, delay_ms = 0;
	bool local_color_map = false, optimize_against_previous = true;
	int disposal = GD_GIF_DISPOSAL_NONE;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_gif_frame_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (writer->finalized) {
		php_gd_gif_throw("GIF writer has already been finalized");
		RETURN_THROWS();
	}

	image = php_gd_libgdimageptr_from_zval_p(image_zv);
	if (options_zv != NULL) {
		x = zval_get_long(zend_read_property(php_gd_gif_frame_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("x"), true, &tmp));
		y = zval_get_long(zend_read_property(php_gd_gif_frame_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("y"), true, &tmp));
		delay_ms = zval_get_long(zend_read_property(php_gd_gif_frame_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("delayMs"), true, &tmp));
		disposal = php_gd_gif_disposal_value(zend_read_property(php_gd_gif_frame_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("disposal"), true, &tmp));
		local_color_map = zend_is_true(zend_read_property(php_gd_gif_frame_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("localColorMap"), true, &tmp));
		optimize_against_previous = zend_is_true(zend_read_property(php_gd_gif_frame_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("optimizeAgainstPrevious"), true, &tmp));
	}

	if (!writer->started) {
		if (x != 0 || y != 0) {
			php_gd_gif_throw("The first GIF frame must start at offset 0,0");
			goto fail;
		}
		writer->canvas_width = image->sx;
		writer->canvas_height = image->sy;
		begin_image = php_gd_gif_begin_image(image);
		if (begin_image == NULL) {
			php_gd_gif_throw("Failed to initialize GIF writer palette");
			goto fail;
		}
		gdImageGifAnimBeginCtx(begin_image, writer->ctx, writer->global_color_map ? 1 : 0, writer->loops);
		if (begin_image != image) {
			gdImageDestroy(begin_image);
		}
		writer->started = true;
	} else if (x + image->sx > writer->canvas_width || y + image->sy > writer->canvas_height) {
		php_gd_gif_throw("GIF frame rectangle exceeds the logical screen");
		goto fail;
	}

	if (optimize_against_previous) {
		if (x != 0 || y != 0 || image->sx != writer->canvas_width || image->sy != writer->canvas_height) {
			php_gd_gif_throw("GIF frame optimization requires offset 0,0 and full-canvas frame dimensions");
			goto fail;
		}
		previous = writer->previous;
	}

	gdImageGifAnimAddCtx(image, writer->ctx, local_color_map ? 1 : 0, (int) x, (int) y,
			(int) (delay_ms / 10), disposal, previous);

	if (x == 0 && y == 0 && image->sx == writer->canvas_width && image->sy == writer->canvas_height) {
		previous_clone = gdImageClone(image);
		if (previous_clone == NULL) {
			php_gd_gif_throw("Failed to retain GIF frame for optimization");
			goto fail;
		}
		if (writer->previous != NULL) {
			gdImageDestroy(writer->previous);
		}
		writer->previous = previous_clone;
	} else if (writer->previous != NULL) {
		gdImageDestroy(writer->previous);
		writer->previous = NULL;
	}
	writer->frame_count++;
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));

fail:
	RETURN_THROWS();
}

PHP_METHOD(Gd_Gif_AnimWriter, finish)
{
	php_gd_gif_anim_writer_object *writer = Z_GD_GIF_ANIM_WRITER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (writer->finalized) {
		php_gd_gif_throw("GIF writer has already been finalized");
		RETURN_THROWS();
	}
	if (writer->frame_count == 0) {
		php_gd_gif_throw("Cannot finalize a GIF writer without frames");
		RETURN_THROWS();
	}

	writer->finalized = true;
	gdImageGifAnimEndCtx(writer->ctx);
	if (writer->destination == PHP_GD_GIF_DESTINATION_MEMORY) {
		int size = 0;
		void *data = gdDPExtractData(writer->ctx, &size);
		writer->ctx->gd_free(writer->ctx);
		writer->ctx = NULL;
		if (data == NULL || size < 0) {
			gdFree(data);
			php_gd_gif_throw("Failed to finalize GIF output");
			RETURN_THROWS();
		}
		RETVAL_STRINGL(data, size);
		gdFree(data);
		return;
	}

	writer->ctx->gd_free(writer->ctx);
	writer->ctx = NULL;
	RETURN_NULL();
}
#endif

void php_gd_gif_minit(void)
{
	zend_class_entry *codec_ce;

	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

	php_gd_gif_write_options_ce = register_class_Gd_Gif_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Gif_Codec();
	php_gd_register_codec_write(php_gd_gif_write_options_ce, codec_ce);
	php_gd_register_codec_format("Gif", php_gd_gif_write_options_ce);
	php_gd_register_codec_extension("gif", php_gd_gif_write_options_ce);

#if defined(HAVE_GD_GIF_ANIM_READ_API) || defined(HAVE_GD_GIF_ANIM_WRITE_API)
	php_gd_gif_disposal_method_ce = register_class_Gd_Gif_DisposalMethod();
#endif

#ifdef HAVE_GD_GIF_ANIM_READ_API
	php_gd_gif_info_ce = register_class_Gd_Gif_Info();
	php_gd_gif_frame_ce = register_class_Gd_Gif_Frame();
	php_gd_gif_anim_reader_ce = register_class_Gd_Gif_AnimReader();
	php_gd_gif_anim_reader_ce->create_object = php_gd_gif_anim_reader_create;

	memcpy(&php_gd_gif_anim_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_gif_anim_reader_handlers.offset = offsetof(php_gd_gif_anim_reader_object, std);
	php_gd_gif_anim_reader_handlers.free_obj = php_gd_gif_anim_reader_free;
	php_gd_gif_anim_reader_handlers.clone_obj = NULL;
#endif

#ifdef HAVE_GD_GIF_ANIM_WRITE_API
	php_gd_gif_frame_options_ce = register_class_Gd_Gif_FrameOptions();
	php_gd_gif_anim_writer_ce = register_class_Gd_Gif_AnimWriter();
	php_gd_gif_anim_writer_ce->create_object = php_gd_gif_anim_writer_create;

	memcpy(&php_gd_gif_anim_writer_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_gif_anim_writer_handlers.offset = offsetof(php_gd_gif_anim_writer_object, std);
	php_gd_gif_anim_writer_handlers.free_obj = php_gd_gif_anim_writer_free;
	php_gd_gif_anim_writer_handlers.clone_obj = NULL;
#endif
}
