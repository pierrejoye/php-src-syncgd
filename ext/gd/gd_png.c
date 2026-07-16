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
#include "gd_png.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>
#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_png_arginfo.h"
#include "gd_png_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#ifdef HAVE_GD_PNG_CODEC
static zend_class_entry *php_gd_png_color_type_ce;
static zend_class_entry *php_gd_png_interlace_method_ce;
static zend_class_entry *php_gd_png_physical_unit_ce;
static zend_class_entry *php_gd_png_filter_ce;
static zend_class_entry *php_gd_png_compression_strategy_ce;
static zend_class_entry *php_gd_png_write_options_ce;
static zend_class_entry *php_gd_png_info_ce;
static zend_class_entry *php_gd_png_reader_ce;
static zend_object_handlers php_gd_png_reader_handlers;

typedef struct {
	zend_string *bytes;
	zval info;
	bool read;
	bool failed;
	zend_object std;
} php_gd_png_reader_object;

static php_gd_png_reader_object *php_gd_png_reader_from_object(zend_object *object)
{
	return (php_gd_png_reader_object *) ((char *) object - offsetof(php_gd_png_reader_object, std));
}

#define Z_GD_PNG_READER_P(zv) php_gd_png_reader_from_object(Z_OBJ_P((zv)))

static void php_gd_png_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_png_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_png_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_png_throw("Failed to open PNG input");
		return false;
	}
	*bytes = php_gd_png_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_png_throw("Failed to read PNG input");
		return false;
	}
	return true;
}

static bool php_gd_png_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_png_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_png_throw("Failed to read PNG input");
		return false;
	}
	return true;
}

static bool php_gd_png_validate_bytes(zend_string *bytes, uint32_t arg_num)
{
	if (ZSTR_LEN(bytes) == 0) {
		zend_argument_value_error(arg_num, "must not be empty");
		return false;
	}
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(arg_num, "must not exceed %d bytes", INT_MAX);
		return false;
	}
	return true;
}

static bool php_gd_png_validate_input_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) == 0 || ZSTR_LEN(bytes) > INT_MAX) {
		php_gd_png_throw("Failed to read PNG input");
		return false;
	}
	return true;
}

static gdImagePtr php_gd_png_decode_bytes(zend_string *bytes);

static void php_gd_png_color_type(zval *result, int color_type)
{
	zend_long case_id;

	switch (color_type) {
		case 0: case_id = ZEND_ENUM_Gd_Png_ColorType_Grayscale; break;
		case 2: case_id = ZEND_ENUM_Gd_Png_ColorType_Rgb; break;
		case 3: case_id = ZEND_ENUM_Gd_Png_ColorType_Palette; break;
		case 4: case_id = ZEND_ENUM_Gd_Png_ColorType_GrayscaleAlpha; break;
		case 6: case_id = ZEND_ENUM_Gd_Png_ColorType_Rgba; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_png_color_type_ce, case_id));
}

static void php_gd_png_interlace_method(zval *result, int interlace_method)
{
	zend_long case_id;

	switch (interlace_method) {
		case 0: case_id = ZEND_ENUM_Gd_Png_InterlaceMethod_None; break;
		case 1: case_id = ZEND_ENUM_Gd_Png_InterlaceMethod_Adam7; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_png_interlace_method_ce, case_id));
}

static void php_gd_png_physical_unit(zval *result, int physical_unit)
{
	zend_long case_id;

	switch (physical_unit) {
		case 0: case_id = ZEND_ENUM_Gd_Png_PhysicalUnit_Unknown; break;
		case 1: case_id = ZEND_ENUM_Gd_Png_PhysicalUnit_Meter; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_png_physical_unit_ce, case_id));
}

static void php_gd_png_metadata_comments(zval *comments, const gdImageMetadata *metadata)
{
	size_t i;

	array_init(comments);
	if (metadata == NULL) {
		return;
	}

	for (i = 0; i < gdImageMetadataGetProfileCount(metadata); i++) {
		const char *key = NULL;
		const unsigned char *data = NULL;
		size_t size = 0;
		const unsigned char *nul;

		if (gdImageMetadataGetProfileAt(metadata, i, &key, &data, &size) != GD_META_OK ||
				key == NULL || data == NULL || strncmp(key, "png:text:", sizeof("png:text:") - 1) != 0) {
			continue;
		}
		nul = memchr(data, 0, size);
		if (nul == NULL) {
			continue;
		}
		add_assoc_stringl_ex(comments, key + sizeof("png:text:") - 1, strlen(key + sizeof("png:text:") - 1),
			(const char *) nul + 1, size - (size_t)(nul - data) - 1);
	}
}

static void php_gd_png_create_info(zval *result, const gdPngInfo *info)
{
	zval value;
	zval comments;

	object_init_ex(result, php_gd_png_info_ce);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("bitDepth"), info->bit_depth);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("colorTypeTag"), info->color_type);
	php_gd_png_color_type(&value, info->color_type);
	zend_update_property(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("colorType"), &value);
	zend_update_property_bool(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("hasAlpha"), info->has_alpha != 0);
	zend_update_property_bool(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("hasTransparency"), info->has_transparency != 0);
	if (info->palette_entries >= 0) {
		zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("paletteEntries"), info->palette_entries);
	} else {
		zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("paletteEntries"));
	}
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("interlaceMethodTag"), info->interlace_method);
	php_gd_png_interlace_method(&value, info->interlace_method);
	zend_update_property(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("interlaceMethod"), &value);
	if (info->x_pixels_per_unit >= 0) {
		zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("xPixelsPerUnit"), info->x_pixels_per_unit);
	} else {
		zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("xPixelsPerUnit"));
	}
	if (info->y_pixels_per_unit >= 0) {
		zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("yPixelsPerUnit"), info->y_pixels_per_unit);
	} else {
		zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("yPixelsPerUnit"));
	}
	if (info->resolution_x >= 0) {
		zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("resolutionX"), info->resolution_x);
	} else {
		zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("resolutionX"));
	}
	if (info->resolution_y >= 0) {
		zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("resolutionY"), info->resolution_y);
	} else {
		zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("resolutionY"));
	}
	if (info->physical_unit >= 0) {
		php_gd_png_physical_unit(&value, info->physical_unit);
		zend_update_property(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("physicalUnit"), &value);
	} else {
		zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("physicalUnit"));
	}
	php_gd_png_metadata_comments(&comments, info->metadata);
	zend_update_property(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("comments"), &comments);
	zval_ptr_dtor(&comments);
	zend_update_property_bool(php_gd_png_info_ce, Z_OBJ_P(result), ZEND_STRL("decodedTrueColor"), info->decoded_truecolor != 0);
}

static bool php_gd_png_filter_to_mask(zval *filter, unsigned int *mask)
{
	zend_object *object = Z_OBJ_P(filter);

	if (object->ce != php_gd_png_filter_ce) {
		return false;
	}

	switch (zend_enum_fetch_case_id(object)) {
		case ZEND_ENUM_Gd_Png_Filter_None:
			*mask |= GD_PNG_FILTER_NONE;
			break;
		case ZEND_ENUM_Gd_Png_Filter_Sub:
			*mask |= GD_PNG_FILTER_SUB;
			break;
		case ZEND_ENUM_Gd_Png_Filter_Up:
			*mask |= GD_PNG_FILTER_UP;
			break;
		case ZEND_ENUM_Gd_Png_Filter_Average:
			*mask |= GD_PNG_FILTER_AVERAGE;
			break;
		case ZEND_ENUM_Gd_Png_Filter_Paeth:
			*mask |= GD_PNG_FILTER_PAETH;
			break;
		default:
			return false;
	}

	return true;
}

static int php_gd_png_compression_strategy(zval *strategy)
{
	zend_object *object = Z_OBJ_P(strategy);

	switch (zend_enum_fetch_case_id(object)) {
		case ZEND_ENUM_Gd_Png_CompressionStrategy_Filtered:
			return GD_PNG_COMPRESSION_STRATEGY_FILTERED;
		case ZEND_ENUM_Gd_Png_CompressionStrategy_HuffmanOnly:
			return GD_PNG_COMPRESSION_STRATEGY_HUFFMAN_ONLY;
		case ZEND_ENUM_Gd_Png_CompressionStrategy_Rle:
			return GD_PNG_COMPRESSION_STRATEGY_RLE;
		case ZEND_ENUM_Gd_Png_CompressionStrategy_Fixed:
			return GD_PNG_COMPRESSION_STRATEGY_FIXED;
		default:
			return GD_PNG_COMPRESSION_STRATEGY_DEFAULT;
	}
}

static bool php_gd_png_validate_comments(zval *comments, uint32_t arg_num)
{
	zend_string *key;
	zval *value;

	ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(comments), key, value) {
		if (key == NULL) {
			zend_argument_type_error(arg_num, "must contain only string keys");
			return false;
		}
		if (ZSTR_LEN(key) == 0 || ZSTR_LEN(key) > 79) {
			zend_argument_value_error(arg_num, "keys must be between 1 and 79 bytes");
			return false;
		}
		if (memchr(ZSTR_VAL(key), '\0', ZSTR_LEN(key)) != NULL) {
			zend_argument_value_error(arg_num, "keys must not contain null bytes");
			return false;
		}
		if (Z_TYPE_P(value) != IS_STRING) {
			zend_argument_type_error(arg_num, "must contain only string values");
			return false;
		}
		if (memchr(Z_STRVAL_P(value), '\0', Z_STRLEN_P(value)) != NULL) {
			zend_argument_value_error(arg_num, "values must not contain null bytes");
			return false;
		}
	} ZEND_HASH_FOREACH_END();

	return true;
}

static gdImageMetadata *php_gd_png_create_metadata_from_comments(zval *comments_zv)
{
	gdImageMetadata *metadata;
	zend_string *key;
	zval *value;

	if (comments_zv == NULL || zend_hash_num_elements(Z_ARRVAL_P(comments_zv)) == 0) {
		return NULL;
	}

	metadata = gdImageMetadataCreate();
	if (metadata == NULL) {
		return NULL;
	}

	ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(comments_zv), key, value) {
		zend_string *payload;
		zend_string *metadata_key;
		int status;

		payload = zend_string_alloc(ZSTR_LEN(key) + 1 + Z_STRLEN_P(value), 0);
		memcpy(ZSTR_VAL(payload), ZSTR_VAL(key), ZSTR_LEN(key));
		ZSTR_VAL(payload)[ZSTR_LEN(key)] = '\0';
		memcpy(ZSTR_VAL(payload) + ZSTR_LEN(key) + 1, Z_STRVAL_P(value), Z_STRLEN_P(value));

		metadata_key = strpprintf(0, "png:text:%s", ZSTR_VAL(key));
		status = gdImageMetadataSetProfile(metadata, ZSTR_VAL(metadata_key),
			(const unsigned char *) ZSTR_VAL(payload), ZSTR_LEN(payload));
		zend_string_release(metadata_key);
		zend_string_release(payload);
		if (status != GD_META_OK) {
			gdImageMetadataFree(metadata);
			return NULL;
		}
	} ZEND_HASH_FOREACH_END();

	return metadata;
}

static void php_gd_png_build_options(zval *options_zv, gdPngWriteOptions *options)
{
	zval rv;
	zval *value;
	zval *filter;

	gdPngWriteOptionsInit(options);
	if (options_zv == NULL) {
		return;
	}

	value = zend_read_property(php_gd_png_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("compressionLevel"), true, &rv);
	options->compression_level = (int) Z_LVAL_P(value);

	value = zend_read_property(php_gd_png_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("filters"), true, &rv);
	options->filters = GD_PNG_FILTER_AUTO;
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), filter) {
		php_gd_png_filter_to_mask(filter, &options->filters);
	} ZEND_HASH_FOREACH_END();

	value = zend_read_property(php_gd_png_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("compressionStrategy"), true, &rv);
	options->compression_strategy = php_gd_png_compression_strategy(value);
}

PHP_METHOD(Gd_Png_WriteOptions, __construct)
{
	zend_long compression_level = -1;
	zval *filters = NULL;
	zval *compression_strategy = NULL;
	zval *comments = NULL;
	zval filters_zv;
	zval comments_zv;
	zval default_strategy_zv;
	zval *filter;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(compression_level)
		Z_PARAM_ARRAY(filters)
		Z_PARAM_OBJECT_OF_CLASS(compression_strategy, php_gd_png_compression_strategy_ce)
		Z_PARAM_ARRAY(comments)
	ZEND_PARSE_PARAMETERS_END();

	if (compression_level < -1 || compression_level > 9) {
		zend_argument_value_error(1, "must be between -1 and 9");
		RETURN_THROWS();
	}

	if (filters != NULL) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(filters), filter) {
			if (Z_TYPE_P(filter) != IS_OBJECT || Z_OBJCE_P(filter) != php_gd_png_filter_ce) {
				zend_argument_type_error(2, "must contain only values of type Gd\\Png\\Filter");
				RETURN_THROWS();
			}
		} ZEND_HASH_FOREACH_END();
	}
	if (comments != NULL && !php_gd_png_validate_comments(comments, 4)) {
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_png_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compressionLevel"), compression_level);
	if (filters != NULL) {
		ZVAL_COPY(&filters_zv, filters);
	} else {
		array_init(&filters_zv);
	}
	zend_update_property(php_gd_png_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("filters"), &filters_zv);
	zval_ptr_dtor(&filters_zv);
	if (compression_strategy == NULL) {
		ZVAL_OBJ(&default_strategy_zv, zend_enum_get_case_by_id(php_gd_png_compression_strategy_ce, ZEND_ENUM_Gd_Png_CompressionStrategy_Default));
		compression_strategy = &default_strategy_zv;
	}
	zend_update_property(php_gd_png_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compressionStrategy"), compression_strategy);
	if (comments != NULL) {
		ZVAL_COPY(&comments_zv, comments);
	} else {
		array_init(&comments_zv);
	}
	zend_update_property(php_gd_png_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("comments"), &comments_zv);
	zval_ptr_dtor(&comments_zv);
}

PHP_METHOD(Gd_Png_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Png_Codec, fromString)
{
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_png_validate_bytes(bytes, 1)) {
		RETURN_THROWS();
	}
	image = php_gd_png_decode_bytes(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_png_throw("Failed to decode PNG image");
		}
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Png_Codec, fromFile)
{
	zend_string *path, *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_png_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_png_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	image = php_gd_png_decode_bytes(bytes);
	zend_string_release(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_png_throw("Failed to decode PNG image");
		}
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Png_Codec, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_png_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_png_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	image = php_gd_png_decode_bytes(bytes);
	zend_string_release(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_png_throw("Failed to decode PNG image");
		}
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Png_Info, __construct)
{
	zend_long width, height, bit_depth, color_type_tag, interlace_method_tag;
	zval *color_type = NULL, *interlace_method = NULL, *physical_unit = NULL;
	bool has_alpha, has_transparency, decoded_truecolor;
	zend_long palette_entries = 0, x_pixels_per_unit = 0, y_pixels_per_unit = 0;
	zend_long resolution_x = 0, resolution_y = 0;
	bool palette_entries_is_null = true, x_pixels_per_unit_is_null = true, y_pixels_per_unit_is_null = true;
	bool resolution_x_is_null = true, resolution_y_is_null = true;
	zval *comments;

	ZEND_PARSE_PARAMETERS_START(17, 17)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(bit_depth)
		Z_PARAM_LONG(color_type_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(color_type, php_gd_png_color_type_ce)
		Z_PARAM_BOOL(has_alpha)
		Z_PARAM_BOOL(has_transparency)
		Z_PARAM_LONG_OR_NULL(palette_entries, palette_entries_is_null)
		Z_PARAM_LONG(interlace_method_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(interlace_method, php_gd_png_interlace_method_ce)
		Z_PARAM_LONG_OR_NULL(x_pixels_per_unit, x_pixels_per_unit_is_null)
		Z_PARAM_LONG_OR_NULL(y_pixels_per_unit, y_pixels_per_unit_is_null)
		Z_PARAM_LONG_OR_NULL(resolution_x, resolution_x_is_null)
		Z_PARAM_LONG_OR_NULL(resolution_y, resolution_y_is_null)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(physical_unit, php_gd_png_physical_unit_ce)
		Z_PARAM_ARRAY(comments)
		Z_PARAM_BOOL(decoded_truecolor)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("bitDepth"), bit_depth);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorTypeTag"), color_type_tag);
	if (color_type) zend_update_property(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorType"), color_type); else zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorType"));
	zend_update_property_bool(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("hasAlpha"), has_alpha);
	zend_update_property_bool(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("hasTransparency"), has_transparency);
	if (palette_entries_is_null) zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("paletteEntries")); else zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("paletteEntries"), palette_entries);
	zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interlaceMethodTag"), interlace_method_tag);
	if (interlace_method) zend_update_property(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interlaceMethod"), interlace_method); else zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interlaceMethod"));
	if (x_pixels_per_unit_is_null) zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xPixelsPerUnit")); else zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xPixelsPerUnit"), x_pixels_per_unit);
	if (y_pixels_per_unit_is_null) zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yPixelsPerUnit")); else zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yPixelsPerUnit"), y_pixels_per_unit);
	if (resolution_x_is_null) zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionX")); else zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionX"), resolution_x);
	if (resolution_y_is_null) zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionY")); else zend_update_property_long(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionY"), resolution_y);
	if (physical_unit) zend_update_property(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("physicalUnit"), physical_unit); else zend_update_property_null(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("physicalUnit"));
	zend_update_property(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("comments"), comments);
	zend_update_property_bool(php_gd_png_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("decodedTrueColor"), decoded_truecolor);
}

PHP_METHOD(Gd_Png_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

static zend_object *php_gd_png_reader_create(zend_class_entry *class_entry)
{
	php_gd_png_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_png_reader_handlers;
	return &reader->std;
}

static void php_gd_png_reader_free(zend_object *object)
{
	php_gd_png_reader_object *reader = php_gd_png_reader_from_object(object);

	if (reader->bytes != NULL) {
		zend_string_release(reader->bytes);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_png_create_info_from_bytes(zval *info_zv, zend_string *bytes)
{
	gdPngInfo info;
	gdImageMetadata *metadata;

	metadata = gdImageMetadataCreate();
	if (metadata == NULL) {
		php_gd_png_throw("Failed to create PNG metadata");
		return false;
	}
	gdPngInfoInit(&info);
	info.metadata = metadata;
	if (gdPngGetInfoPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &info) != 0) {
		gdImageMetadataFree(metadata);
		php_gd_png_throw("Failed to read PNG info");
		return false;
	}
	php_gd_png_create_info(info_zv, &info);
	gdImageMetadataFree(metadata);
	return true;
}

static bool php_gd_png_initialize_reader(zval *result, zend_string *bytes)
{
	php_gd_png_reader_object *reader;

	object_init_ex(result, php_gd_png_reader_ce);
	reader = Z_GD_PNG_READER_P(result);
	reader->bytes = zend_string_copy(bytes);
	if (!php_gd_png_create_info_from_bytes(&reader->info, bytes)) {
		return false;
	}
	return true;
}

static gdImagePtr php_gd_png_decode_bytes(zend_string *bytes)
{
	return gdImageCreateFromPngPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
}

PHP_METHOD(Gd_Png_Reader, fromString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_png_validate_bytes(bytes, 1)) {
		RETURN_THROWS();
	}
	if (!php_gd_png_initialize_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Png_Reader, fromFile)
{
	zend_string *path, *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_png_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_png_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	if (!php_gd_png_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Png_Reader, fromStream)
{
	zval *stream_zv;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_png_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_png_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	if (!php_gd_png_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Png_Reader, info)
{
	php_gd_png_reader_object *reader = Z_GD_PNG_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&reader->info);
}

PHP_METHOD(Gd_Png_Reader, read)
{
	php_gd_png_reader_object *reader = Z_GD_PNG_READER_P(ZEND_THIS);
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_NONE();

	if (reader->failed) {
		php_gd_png_throw("PNG reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->read) {
		php_gd_png_throw("PNG image has already been read");
		RETURN_THROWS();
	}

	image = gdImageCreateFromPngPtr((int) ZSTR_LEN(reader->bytes), ZSTR_VAL(reader->bytes));
	reader->read = true;
	if (image == NULL) {
		reader->failed = true;
		php_gd_png_throw("Failed to decode PNG image");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

static void php_gd_png_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv;
	zval *destination;
	zval *options_zv = NULL;
	gdPngWriteOptions options;
	gdImageMetadata *metadata = NULL;
	gdIOCtx *ctx;
	int result;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_png_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ZEND_ASSERT(require_stream || Z_TYPE_P(destination) == IS_STRING);

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		if (!EG(exception)) {
			zend_throw_exception(php_gd_codec_exception_ce, "Failed to open PNG output", 0);
		}
		RETURN_THROWS();
	}

	php_gd_png_build_options(options_zv, &options);
	if (options_zv != NULL) {
		zval rv;
		zval *comments = zend_read_property(php_gd_png_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("comments"), true, &rv);
		metadata = php_gd_png_create_metadata_from_comments(comments);
		if (metadata == NULL && zend_hash_num_elements(Z_ARRVAL_P(comments)) > 0) {
			ctx->gd_free(ctx);
			php_gd_png_throw("Failed to create PNG metadata");
			RETURN_THROWS();
		}
		options.metadata = metadata;
	}
	result = gdImagePngCtxWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), ctx, &options);
	ctx->gd_free(ctx);
	if (metadata != NULL) {
		gdImageMetadataFree(metadata);
	}

	if (result != 0) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to encode PNG image", 0);
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Png_Codec, toFile)
{
	php_gd_png_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Png_Codec, toStream)
{
	php_gd_png_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Png_Codec, toString)
{
	zval *image_zv;
	zval *options_zv = NULL;
	gdPngWriteOptions options;
	gdImageMetadata *metadata = NULL;
	int size = 0;
	void *data;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_png_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_png_build_options(options_zv, &options);
	if (options_zv != NULL) {
		zval rv;
		zval *comments = zend_read_property(php_gd_png_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("comments"), true, &rv);
		metadata = php_gd_png_create_metadata_from_comments(comments);
		if (metadata == NULL && zend_hash_num_elements(Z_ARRVAL_P(comments)) > 0) {
			php_gd_png_throw("Failed to create PNG metadata");
			RETURN_THROWS();
		}
		options.metadata = metadata;
	}
	data = gdImagePngPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &options);
	if (metadata != NULL) {
		gdImageMetadataFree(metadata);
	}
	if (data == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to encode PNG image", 0);
		RETURN_THROWS();
	}

	RETVAL_STRINGL(data, size);
	gdFree(data);
}
#endif

void php_gd_png_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_PNG_CODEC
	zend_class_entry *codec_ce;

	php_gd_png_color_type_ce = register_class_Gd_Png_ColorType();
	php_gd_png_interlace_method_ce = register_class_Gd_Png_InterlaceMethod();
	php_gd_png_physical_unit_ce = register_class_Gd_Png_PhysicalUnit();
	php_gd_png_filter_ce = register_class_Gd_Png_Filter();
	php_gd_png_compression_strategy_ce = register_class_Gd_Png_CompressionStrategy();
	php_gd_png_write_options_ce = register_class_Gd_Png_WriteOptions(php_gd_get_codec_write_options_ce());
	php_gd_png_info_ce = register_class_Gd_Png_Info();
	php_gd_png_reader_ce = register_class_Gd_Png_Reader();
	php_gd_png_reader_ce->create_object = php_gd_png_reader_create;

	memcpy(&php_gd_png_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_png_reader_handlers.offset = offsetof(php_gd_png_reader_object, std);
	php_gd_png_reader_handlers.free_obj = php_gd_png_reader_free;
	php_gd_png_reader_handlers.clone_obj = NULL;

	codec_ce = register_class_Gd_Png_Codec();
	php_gd_register_codec_write(php_gd_png_write_options_ce, codec_ce);
	php_gd_register_codec_format("Png", php_gd_png_write_options_ce);
	php_gd_register_codec_extension("png", php_gd_png_write_options_ce);
#endif
}
