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
#include "gd_qoi.h"
#include "ext/spl/spl_exceptions.h"

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_qoi_arginfo.h"
#include "gd_qoi_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#ifdef HAVE_GD_QOI
static zend_class_entry *php_gd_qoi_colorspace_ce;
static zend_class_entry *php_gd_qoi_write_options_ce;

static int php_gd_qoi_colorspace(zval *colorspace)
{
	ZEND_ASSERT(Z_TYPE_P(colorspace) == IS_OBJECT);

	switch (zend_enum_fetch_case_id(Z_OBJ_P(colorspace))) {
		case ZEND_ENUM_Gd_Qoi_Colorspace_Linear:
			return GD_QOI_LINEAR;
		case ZEND_ENUM_Gd_Qoi_Colorspace_SRGB:
		default:
			return GD_QOI_SRGB;
	}
}

static void php_gd_qoi_throw_open_failure(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static gdImagePtr php_gd_qoi_decode_from_stream(php_stream *stream)
{
	gdImagePtr im = NULL;
	FILE *fp = NULL;

	if (php_stream_is(stream, PHP_STREAM_IS_STDIO)
		&& FAILURE != php_stream_cast(stream, PHP_STREAM_AS_STDIO, (void **) &fp, REPORT_ERRORS)
	) {
		im = gdImageCreateFromQoi(fp);
		if (fp != NULL) {
			fflush(fp);
		}
	}

	if (im == NULL) {
		zend_string *bytes = php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);

		if (bytes == NULL) {
			return NULL;
		}
		if (ZSTR_LEN(bytes) > ZEND_LONG_MAX) {
			zend_string_release_ex(bytes, 0);
			return NULL;
		}

		im = gdImageCreateFromQoiPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
		zend_string_release_ex(bytes, 0);
	}

	return im;
}

PHP_METHOD(Gd_Qoi_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Qoi_WriteOptions, __construct)
{
	zval *colorspace_zv = NULL;
	zval default_colorspace;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(colorspace_zv, php_gd_qoi_colorspace_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (colorspace_zv == NULL) {
		ZVAL_OBJ(&default_colorspace, zend_enum_get_case_by_id(php_gd_qoi_colorspace_ce, ZEND_ENUM_Gd_Qoi_Colorspace_SRGB));
		colorspace_zv = &default_colorspace;
	}

	zend_update_property(php_gd_qoi_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorspace"), colorspace_zv);
}

PHP_METHOD(Gd_Qoi_Codec, fromFile)
{
	zend_string *path;
	php_stream *stream;
	gdImagePtr im;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);
	if (stream == NULL) {
		php_gd_qoi_throw_open_failure("Failed to open QOI input");
		RETURN_THROWS();
	}

	im = php_gd_qoi_decode_from_stream(stream);
	php_stream_close(stream);

	if (im == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to decode QOI image", 0);
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, im);
}

PHP_METHOD(Gd_Qoi_Codec, fromStream)
{
	zval *stream_zv;
	php_stream *stream;
	gdImagePtr im;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(stream_zv)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}

	php_stream_from_zval_no_verify(stream, stream_zv);
	if (stream == NULL) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}

	im = php_gd_qoi_decode_from_stream(stream);
	if (im == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to decode QOI image", 0);
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, im);
}

PHP_METHOD(Gd_Qoi_Codec, fromString)
{
	zend_string *bytes;
	gdImagePtr im;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (ZSTR_LEN(bytes) > ZEND_LONG_MAX) {
		zend_argument_value_error(1, "is too long");
		RETURN_THROWS();
	}

	im = gdImageCreateFromQoiPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
	if (im == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to decode QOI image", 0);
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, im);
}

static void php_gd_qoi_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv;
	zval *destination;
	zval *options_zv = NULL;
	zval rv, *colorspace_zv;
	gdIOCtx *ctx;
	int colorspace;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_qoi_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ZEND_ASSERT(require_stream || Z_TYPE_P(destination) == IS_STRING);

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		php_gd_qoi_throw_open_failure("Failed to open QOI output");
		RETURN_THROWS();
	}

	if (options_zv != NULL) {
		colorspace_zv = zend_read_property(php_gd_qoi_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("colorspace"), true, &rv);
		colorspace = php_gd_qoi_colorspace(colorspace_zv);
	} else {
		colorspace = GD_QOI_SRGB;
	}

	gdImageQoiCtxEx(php_gd_libgdimageptr_from_zval_p(image_zv), ctx, colorspace);
	ctx->gd_free(ctx);
}

PHP_METHOD(Gd_Qoi_Codec, toFile)
{
	php_gd_qoi_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Qoi_Codec, toStream)
{
	php_gd_qoi_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Qoi_Codec, toString)
{
	zval *image_zv;
	zval *options_zv = NULL;
	zval rv, *colorspace_zv;
	int size = 0;
	int colorspace;
	void *data;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_qoi_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (options_zv != NULL) {
		colorspace_zv = zend_read_property(php_gd_qoi_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("colorspace"), true, &rv);
		colorspace = php_gd_qoi_colorspace(colorspace_zv);
	} else {
		colorspace = GD_QOI_SRGB;
	}

	data = gdImageQoiPtrEx(php_gd_libgdimageptr_from_zval_p(image_zv), &size, colorspace);
	if (data == NULL || size < 0) {
		gdFree(data);
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to encode QOI image", 0);
		RETURN_THROWS();
	}

	RETVAL_STRINGL(data, size);
	gdFree(data);
}
#endif

void php_gd_qoi_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_QOI
	zend_class_entry *codec_ce;

	php_gd_qoi_colorspace_ce = register_class_Gd_Qoi_Colorspace();
	php_gd_qoi_write_options_ce = register_class_Gd_Qoi_WriteOptions(php_gd_get_codec_write_options_ce());
	codec_ce = register_class_Gd_Qoi_Codec();
	php_gd_register_codec_write(php_gd_qoi_write_options_ce, codec_ce);
	php_gd_register_codec_format("Qoi", php_gd_qoi_write_options_ce);
	php_gd_register_codec_extension("qoi", php_gd_qoi_write_options_ce);
#endif
}
