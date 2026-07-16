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
#include "gd_heif.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_heif_arginfo.h"
#include "gd_heif_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#ifdef HAVE_GD_HEIF
static zend_class_entry *php_gd_heif_compression_format_ce;
static zend_class_entry *php_gd_heif_chroma_subsampling_ce;
static zend_class_entry *php_gd_heif_read_options_ce;
static zend_class_entry *php_gd_heif_write_options_ce;

static void php_gd_heif_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_heif_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_heif_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_heif_throw("Failed to open HEIF input");
		return false;
	}
	*bytes = php_gd_heif_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_heif_throw("Failed to read HEIF input");
		return false;
	}
	return true;
}

static bool php_gd_heif_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_heif_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_heif_throw("Failed to read HEIF input");
		return false;
	}
	return true;
}

static gdImagePtr php_gd_heif_decode_bytes(zend_string *bytes, zval *options_zv)
{
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		return NULL;
	}

#ifdef HAVE_GD_BUNDLED
	gdHeifReadOptions options;
	zval rv;
	zval *value;

	gdHeifReadOptionsInit(&options);
	if (options_zv != NULL) {
		value = zend_read_property(php_gd_heif_read_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("ignoreTransformations"), true, &rv);
		options.ignore_transformations = zend_is_true(value);
	}
	return gdImageCreateFromHeifPtrWithOptions((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &options);
#else
	if (options_zv != NULL) {
		zval rv;
		zval *value = zend_read_property(php_gd_heif_read_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("ignoreTransformations"), true, &rv);
		if (!zend_is_true(value)) {
			php_gd_heif_throw("Respecting HEIF transformations is not supported by this gd build");
			return NULL;
		}
	}
	return gdImageCreateFromHeifPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
#endif
}

static gdHeifCodec php_gd_heif_codec_from_zval(zval *value)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Heif_CompressionFormat_Hevc:
			return GD_HEIF_CODEC_HEVC;
		case ZEND_ENUM_Gd_Heif_CompressionFormat_Av1:
			return GD_HEIF_CODEC_AV1;
		default:
			return GD_HEIF_CODEC_UNKNOWN;
	}
}

static gdHeifChroma php_gd_heif_chroma_from_zval(zval *value)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Heif_ChromaSubsampling_Yuv420:
			return GD_HEIF_CHROMA_420;
		case ZEND_ENUM_Gd_Heif_ChromaSubsampling_Yuv422:
			return GD_HEIF_CHROMA_422;
		case ZEND_ENUM_Gd_Heif_ChromaSubsampling_Yuv444:
			return GD_HEIF_CHROMA_444;
		default:
			return NULL;
	}
}

typedef struct {
	int quality;
	bool lossless;
	gdHeifCodec codec;
	gdHeifChroma chroma;
} php_gd_heif_write_options;

static bool php_gd_heif_read_write_options(zval *options_zv, php_gd_heif_write_options *options)
{
	zval rv;
	zval *value;

	options->quality = -1;
	options->lossless = false;
	options->codec = GD_HEIF_CODEC_HEVC;
	options->chroma = GD_HEIF_CHROMA_444;
	if (options_zv == NULL) {
		return true;
	}

	value = zend_read_property(php_gd_heif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("quality"), true, &rv);
	options->quality = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_heif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("lossless"), true, &rv);
	options->lossless = zend_is_true(value);
	value = zend_read_property(php_gd_heif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("codec"), true, &rv);
	options->codec = php_gd_heif_codec_from_zval(value);
	value = zend_read_property(php_gd_heif_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("chromaSubsampling"), true, &rv);
	options->chroma = php_gd_heif_chroma_from_zval(value);

	if (options->codec == GD_HEIF_CODEC_UNKNOWN || options->chroma == NULL) {
		php_gd_heif_throw("Unsupported HEIF write option");
		return false;
	}
	return true;
}

static bool php_gd_heif_encode_to_string(zval *image_zv, zval *options_zv, zend_string **bytes)
{
	php_gd_heif_write_options options;
	int size = 0;
	void *data;

	if (!php_gd_heif_read_write_options(options_zv, &options)) {
		return false;
	}

#ifdef HAVE_GD_BUNDLED
	gdHeifWriteOptions gd_options;

	gdHeifWriteOptionsInit(&gd_options);
	gd_options.quality = options.quality;
	gd_options.lossless = options.lossless ? 1 : 0;
	gd_options.codec = options.codec;
	gd_options.chroma = options.chroma;
	data = gdImageHeifPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &gd_options);
#else
	data = gdImageHeifPtrEx(
		php_gd_libgdimageptr_from_zval_p(image_zv),
		&size,
		options.lossless ? 200 : options.quality,
		options.codec,
		options.chroma
	);
#endif

	if (data == NULL || size < 0) {
		gdFree(data);
		php_gd_heif_throw("Failed to encode HEIF image");
		return false;
	}

	*bytes = zend_string_init(data, (size_t) size, 0);
	gdFree(data);
	return true;
}

PHP_METHOD(Gd_Heif_ReadOptions, __construct)
{
	bool ignore_transformations = true;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(ignore_transformations)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_bool(php_gd_heif_read_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("ignoreTransformations"), ignore_transformations);
}

PHP_METHOD(Gd_Heif_WriteOptions, __construct)
{
	zend_long quality = -1;
	bool lossless = false;
	zval *codec = NULL, *chroma_subsampling = NULL;
	zval default_codec_zv, default_chroma_zv;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(quality)
		Z_PARAM_BOOL(lossless)
		Z_PARAM_OBJECT_OF_CLASS(codec, php_gd_heif_compression_format_ce)
		Z_PARAM_OBJECT_OF_CLASS(chroma_subsampling, php_gd_heif_chroma_subsampling_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (quality < -1 || quality > 100) {
		zend_argument_value_error(1, "must be between -1 and 100");
		RETURN_THROWS();
	}

	if (codec == NULL) {
		ZVAL_OBJ(&default_codec_zv, zend_enum_get_case_by_id(php_gd_heif_compression_format_ce, ZEND_ENUM_Gd_Heif_CompressionFormat_Hevc));
		codec = &default_codec_zv;
	}
	if (chroma_subsampling == NULL) {
		ZVAL_OBJ(&default_chroma_zv, zend_enum_get_case_by_id(php_gd_heif_chroma_subsampling_ce, ZEND_ENUM_Gd_Heif_ChromaSubsampling_Yuv444));
		chroma_subsampling = &default_chroma_zv;
	}

	zend_update_property_long(php_gd_heif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("quality"), quality);
	zend_update_property_bool(php_gd_heif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("lossless"), lossless);
	zend_update_property(php_gd_heif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("codec"), codec);
	zend_update_property(php_gd_heif_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("chromaSubsampling"), chroma_subsampling);
}

PHP_METHOD(Gd_Heif_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Heif_Codec, fromString)
{
	zend_string *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_heif_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_heif_decode_bytes(bytes, options_zv);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_heif_throw("Failed to decode HEIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Heif_Codec, fromFile)
{
	zend_string *path, *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_heif_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_heif_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_heif_decode_bytes(bytes, options_zv);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_heif_throw("Failed to decode HEIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Heif_Codec, fromStream)
{
	zval *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_heif_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_heif_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	image = php_gd_heif_decode_bytes(bytes, options_zv);
	zend_string_release_ex(bytes, 0);
	if (image == NULL) {
		if (!EG(exception)) {
			php_gd_heif_throw("Failed to decode HEIF image");
		}
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Heif_Codec, toString)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_heif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_heif_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	RETURN_STR(bytes);
}

PHP_METHOD(Gd_Heif_Codec, toFile)
{
	zval *image_zv, *options_zv = NULL;
	zend_string *path, *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_heif_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_heif_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	stream = php_stream_open_wrapper(ZSTR_VAL(path), "wb", REPORT_ERRORS | IGNORE_PATH, NULL);
	if (stream == NULL) {
		zend_string_release_ex(bytes, 0);
		php_gd_heif_throw("Failed to open HEIF output");
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		php_stream_close(stream);
		zend_string_release_ex(bytes, 0);
		php_gd_heif_throw("Failed to write HEIF output");
		RETURN_THROWS();
	}
	php_stream_close(stream);
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Heif_Codec, toStream)
{
	zval *image_zv, *stream_zv, *options_zv = NULL;
	zend_string *bytes;
	php_stream *stream;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_heif_write_options_ce)
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
	if (!php_gd_heif_encode_to_string(image_zv, options_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (php_stream_write(stream, ZSTR_VAL(bytes), ZSTR_LEN(bytes)) != ZSTR_LEN(bytes)) {
		zend_string_release_ex(bytes, 0);
		php_gd_heif_throw("Failed to write HEIF output");
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
}
#endif

void php_gd_heif_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_HEIF
	zend_class_entry *codec_ce;

	php_gd_heif_compression_format_ce = register_class_Gd_Heif_CompressionFormat();
	php_gd_heif_chroma_subsampling_ce = register_class_Gd_Heif_ChromaSubsampling();
	php_gd_heif_read_options_ce = register_class_Gd_Heif_ReadOptions();
	php_gd_heif_write_options_ce = register_class_Gd_Heif_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Heif_Codec();
	php_gd_register_codec_write(php_gd_heif_write_options_ce, codec_ce);
	php_gd_register_codec_format("Heif", php_gd_heif_write_options_ce);
	php_gd_register_codec_extension("heif", php_gd_heif_write_options_ce);
	php_gd_register_codec_extension("heic", php_gd_heif_write_options_ce);
#endif
}
