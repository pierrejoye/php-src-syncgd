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
#include "ext/spl/spl_exceptions.h"
#include <limits.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
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

typedef struct {
	int quality;
	int speed;
	bool lossless;
	int chroma_subsampling;
} php_gd_avif_write_options;

static bool php_gd_avif_read_write_options(zval *options_zv, php_gd_avif_write_options *options)
{
	zval rv;
	zval *value;

	options->quality = -1;
	options->speed = 6;
	options->lossless = false;
	options->chroma_subsampling = PHP_GD_AVIF_CHROMA_SUBSAMPLING_AUTO;
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
		return true;
	}
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Avif_ChromaSubsampling_Yuv420:
			options->chroma_subsampling = PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV420;
			return true;
		case ZEND_ENUM_Gd_Avif_ChromaSubsampling_Yuv444:
			options->chroma_subsampling = PHP_GD_AVIF_CHROMA_SUBSAMPLING_YUV444;
			return true;
		default:
			php_gd_avif_throw("Unsupported AVIF chroma subsampling option");
			return false;
	}
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

PHP_METHOD(Gd_Avif_ReadOptions, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Avif_WriteOptions, __construct)
{
	zend_long quality = -1, speed = -1;
	bool lossless = false;
	zval *chroma_subsampling = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(quality)
		Z_PARAM_LONG(speed)
		Z_PARAM_BOOL(lossless)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(chroma_subsampling, php_gd_avif_chroma_subsampling_ce)
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
}

PHP_METHOD(Gd_Avif_Codec, toStream)
{
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
}
#endif

void php_gd_avif_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_AVIF
	zend_class_entry *codec_ce;

	php_gd_avif_chroma_subsampling_ce = register_class_Gd_Avif_ChromaSubsampling();
	php_gd_avif_read_options_ce = register_class_Gd_Avif_ReadOptions();
	php_gd_avif_write_options_ce = register_class_Gd_Avif_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Avif_Codec();
	php_gd_register_codec_write(php_gd_avif_write_options_ce, codec_ce);
	php_gd_register_codec_format("Avif", php_gd_avif_write_options_ce);
	php_gd_register_codec_extension("avif", php_gd_avif_write_options_ce);
#endif
}
