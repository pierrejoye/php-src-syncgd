/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
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
#include "gd_bmp.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_bmp_arginfo.h"
#include "gd_bmp_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;
static zend_class_entry *php_gd_bmp_compression_ce;
static zend_class_entry *php_gd_bmp_read_options_ce;
static zend_class_entry *php_gd_bmp_write_options_ce;

enum {
	PHP_GD_BMP_COMPRESSION_AUTO = -1,
	PHP_GD_BMP_COMPRESSION_NONE = 0,
	PHP_GD_BMP_COMPRESSION_RLE8 = 1,
	PHP_GD_BMP_COMPRESSION_RLE4 = 2
};

static void php_gd_bmp_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_bmp_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_bmp_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_bmp_throw("Failed to open BMP input");
		return false;
	}
	*bytes = php_gd_bmp_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_bmp_throw("Failed to read BMP input");
		return false;
	}
	return true;
}

static bool php_gd_bmp_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_bmp_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_bmp_throw("Failed to read BMP input");
		return false;
	}
	return true;
}

static gdImagePtr php_gd_bmp_decode_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return NULL;
	}
	return gdImageCreateFromBmpPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
}

typedef struct {
	int bits_per_pixel;
	int compression;
	bool quantize;
	bool force_v4_header;
	bool rgb555;
} php_gd_bmp_write_options;

static int php_gd_bmp_compression_from_zval(zval *value)
{
	if (Z_TYPE_P(value) == IS_NULL) {
		return PHP_GD_BMP_COMPRESSION_AUTO;
	}

	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Bmp_Compression_None:
			return PHP_GD_BMP_COMPRESSION_NONE;
		case ZEND_ENUM_Gd_Bmp_Compression_Rle8:
			return PHP_GD_BMP_COMPRESSION_RLE8;
		case ZEND_ENUM_Gd_Bmp_Compression_Rle4:
			return PHP_GD_BMP_COMPRESSION_RLE4;
		default:
			return -2;
	}
}

static bool php_gd_bmp_read_write_options(zval *options_zv, php_gd_bmp_write_options *options)
{
	zval rv;
	zval *value;

	options->bits_per_pixel = 0;
	options->compression = PHP_GD_BMP_COMPRESSION_AUTO;
	options->quantize = false;
	options->force_v4_header = false;
	options->rgb555 = false;
	if (options_zv == NULL) {
		return true;
	}

	value = zend_read_property(php_gd_bmp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("bitsPerPixel"), true, &rv);
	options->bits_per_pixel = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_bmp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("compression"), true, &rv);
	options->compression = php_gd_bmp_compression_from_zval(value);
	value = zend_read_property(php_gd_bmp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("quantize"), true, &rv);
	options->quantize = zend_is_true(value);
	value = zend_read_property(php_gd_bmp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("forceV4Header"), true, &rv);
	options->force_v4_header = zend_is_true(value);
	value = zend_read_property(php_gd_bmp_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("rgb555"), true, &rv);
	options->rgb555 = zend_is_true(value);

	if (options->compression == -2) {
		php_gd_bmp_throw("Unsupported BMP compression option");
		return false;
	}
	return true;
}

static bool php_gd_bmp_encode_to_string(zval *image_zv, zval *options_zv, zend_string **bytes)
{
	php_gd_bmp_write_options options;
	int size = 0;
	void *data;

	if (!php_gd_bmp_read_write_options(options_zv, &options)) {
		return false;
	}

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_BMP_EXTENDED)
	int compression;
	int flags = GD_BMP_FLAG_NONE;

	switch (options.compression) {
		case PHP_GD_BMP_COMPRESSION_AUTO:
			compression = -1;
			break;
		case PHP_GD_BMP_COMPRESSION_NONE:
			compression = GD_BMP_COMPRESS_NONE;
			break;
		case PHP_GD_BMP_COMPRESSION_RLE8:
			compression = GD_BMP_COMPRESS_RLE8;
			break;
		case PHP_GD_BMP_COMPRESSION_RLE4:
			compression = GD_BMP_COMPRESS_RLE4;
			break;
		default:
			php_gd_bmp_throw("Unsupported BMP compression option");
			return false;
	}

	if (options.quantize) {
		flags |= GD_BMP_FLAG_QUANTIZE;
	}
	if (options.force_v4_header) {
		flags |= GD_BMP_FLAG_FORCE_V4HDR;
	}
	if (options.rgb555) {
		flags |= GD_BMP_FLAG_RGB555;
	}
	data = gdImageBmpPtrEx(php_gd_libgdimageptr_from_zval_p(image_zv), &size, options.bits_per_pixel, compression, flags);
#else
	if (options.bits_per_pixel != 0 ||
			(options.compression != PHP_GD_BMP_COMPRESSION_AUTO && options.compression != PHP_GD_BMP_COMPRESSION_NONE) ||
			options.quantize || options.force_v4_header || options.rgb555) {
		php_gd_bmp_throw("Extended BMP write options are not supported by this gd build");
		return false;
	}
	data = gdImageBmpPtr(php_gd_libgdimageptr_from_zval_p(image_zv), &size, options.compression == PHP_GD_BMP_COMPRESSION_AUTO);
#endif

	if (data == NULL || size < 0) {
		gdFree(data);
		php_gd_bmp_throw("Failed to encode BMP image");
		return false;
	}

	*bytes = zend_string_init(data, (size_t) size, 0);
	gdFree(data);
	return true;
}

PHP_METHOD(Gd_Bmp_ReadOptions, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Bmp_WriteOptions, __construct)
{
	zend_long bits_per_pixel = 0;
	bool quantize = false, force_v4_header = false, rgb555 = false;
	zval *compression = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 5)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(bits_per_pixel)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(compression, php_gd_bmp_compression_ce)
		Z_PARAM_BOOL(quantize)
		Z_PARAM_BOOL(force_v4_header)
		Z_PARAM_BOOL(rgb555)
	ZEND_PARSE_PARAMETERS_END();

	switch (bits_per_pixel) {
		case 0:
		case 1:
		case 4:
		case 8:
		case 16:
		case 24:
		case 32:
			break;
		default:
			zend_argument_value_error(1, "must be one of 0, 1, 4, 8, 16, 24, or 32");
			RETURN_THROWS();
	}

	zend_update_property_long(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("bitsPerPixel"), bits_per_pixel);
	if (compression != NULL) {
		zend_update_property(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"), compression);
	} else {
		zend_update_property_null(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"));
	}
	zend_update_property_bool(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("quantize"), quantize);
	zend_update_property_bool(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("forceV4Header"), force_v4_header);
	zend_update_property_bool(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("rgb555"), rgb555);
}

PHP_METHOD(Gd_Bmp_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Bmp_Codec, fromString)
{
	zend_string *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_bmp_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_bmp_decode_bytes(bytes);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_bmp_throw("Failed to decode BMP image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Bmp_Codec, fromFile)
{
	zend_string *path, *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_bmp_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_bmp_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_bmp_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_bmp_throw("Failed to decode BMP image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Bmp_Codec, fromStream)
{
	zval *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_bmp_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_bmp_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_bmp_decode_bytes(bytes);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_bmp_throw("Failed to decode BMP image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Bmp_Codec, toString)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_bmp_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_bmp_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	RETURN_STR(bytes);
}

PHP_METHOD(Gd_Bmp_Codec, toFile)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *path, *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_bmp_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_bmp_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	stream = php_stream_open_wrapper(ZSTR_VAL(path), "wb", REPORT_ERRORS | IGNORE_PATH, NULL);
	if (stream == NULL) {
		zend_string_release_ex(bytes, 0);
		php_gd_bmp_throw("Failed to open BMP output");
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		php_stream_close(stream);
		zend_string_release_ex(bytes, 0);
		php_gd_bmp_throw("Failed to write BMP output");
		RETURN_THROWS();
	}
	php_stream_close(stream);
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Bmp_Codec, toStream)
{
	zval *image_zv, *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_bmp_write_options_ce)
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
	if (!php_gd_bmp_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		zend_string_release_ex(bytes, 0);
		php_gd_bmp_throw("Failed to write BMP output");
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}

void php_gd_bmp_minit(void)
{
	zend_class_entry *codec_ce;

	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();
	php_gd_bmp_compression_ce = register_class_Gd_Bmp_Compression();
	php_gd_bmp_read_options_ce = register_class_Gd_Bmp_ReadOptions();
	php_gd_bmp_write_options_ce = register_class_Gd_Bmp_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Bmp_Codec();
	php_gd_register_codec_write(php_gd_bmp_write_options_ce, codec_ce);
	php_gd_register_codec_format("Bmp", php_gd_bmp_write_options_ce);
	php_gd_register_codec_extension("bmp", php_gd_bmp_write_options_ce);
}
