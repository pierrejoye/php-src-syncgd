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
#include <limits.h>
#include <math.h>
#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "gd_2d.h"
#else
# include <gd.h>
#endif

#include "gd_image_arginfo.h"
#include "gd_image_decl.h"

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_SCALE_WITH_OPTIONS)
# define PHP_GD_HAVE_SCALE_WITH_OPTIONS 1
#endif

#ifndef GD_SCALE_INTERPOLATION_AUTO
# define GD_SCALE_INTERPOLATION_AUTO -1
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_AUTOCROP_WITH_OPTIONS)
# define PHP_GD_HAVE_AUTOCROP_WITH_OPTIONS 1
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_IMAGE_READ_CTX_EX)
# define PHP_GD_HAVE_IMAGE_READ_CTX_EX 1
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_PERCEPTUAL_DIFF)
# define PHP_GD_HAVE_PERCEPTUAL_DIFF 1
#endif

static zend_class_entry *php_gd_color_model_ce;
static zend_class_entry *php_gd_rotate_options_ce;
static zend_class_entry *php_gd_auto_crop_mode_ce;
static zend_class_entry *php_gd_auto_crop_options_ce;
static zend_class_entry *php_gd_scale_fit_ce;
static zend_class_entry *php_gd_scale_gravity_ce;
static zend_class_entry *php_gd_scale_strategy_ce;
static zend_class_entry *php_gd_interpolation_method_ce;
static zend_class_entry *php_gd_scale_options_ce;
#ifdef HAVE_GD_BUNDLED
static zend_class_entry *php_gd_transform_options_ce;
#endif
static zend_class_entry *php_gd_perceptual_diff_mode_ce;
static zend_class_entry *php_gd_perceptual_diff_options_ce;
static zend_class_entry *php_gd_perceptual_diff_result_ce;
static zend_class_entry *php_gd_codec_exception_ce;
static zend_class_entry *php_gd_codec_format_ce;
static zend_class_entry *php_gd_codec_write_options_ce;

#ifdef PHP_GD_HAVE_PERCEPTUAL_DIFF
static gdImageDiffMode php_gd_perceptual_diff_mode_from_zval(zval *mode_zv)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(mode_zv))) {
		case ZEND_ENUM_Gd_PerceptualDiffMode_None:
			return GD_IMAGE_DIFF_NONE;
		case ZEND_ENUM_Gd_PerceptualDiffMode_Overlay:
			return GD_IMAGE_DIFF_OVERLAY;
		case ZEND_ENUM_Gd_PerceptualDiffMode_Mask:
			return GD_IMAGE_DIFF_MASK;
		default:
			zend_throw_error(NULL, "Unsupported Gd\\PerceptualDiffMode");
			return GD_IMAGE_DIFF_NONE;
	}
}

static void php_gd_perceptual_diff_create_result(zval *return_value, const gdImagePerceptualDiffResult *result, gdImagePtr diff_image)
{
	zval diff_zv;

	object_init_ex(return_value, php_gd_perceptual_diff_result_ce);
	zend_update_property_long(php_gd_perceptual_diff_result_ce, Z_OBJ_P(return_value), ZEND_STRL("pixelsChanged"), result->pixels_changed);
	zend_update_property_double(php_gd_perceptual_diff_result_ce, Z_OBJ_P(return_value), ZEND_STRL("maximumDelta"), result->maximum_delta);

	if (diff_image != NULL) {
		php_gd_assign_libgdimageptr_as_extgdimage(&diff_zv, diff_image);
		zend_update_property(php_gd_perceptual_diff_result_ce, Z_OBJ_P(return_value), ZEND_STRL("diffImage"), &diff_zv);
		zval_ptr_dtor(&diff_zv);
	} else {
		zend_update_property_null(php_gd_perceptual_diff_result_ce, Z_OBJ_P(return_value), ZEND_STRL("diffImage"));
	}
}
#endif

static zend_class_entry *php_gd_write_options_ce_from_format_zval(zval *format_zv)
{
	zend_class_entry *options_ce = php_gd_find_write_options_ce_by_format(format_zv);

	if (options_ce == NULL) {
		zval *case_name = zend_enum_fetch_case_name(Z_OBJ_P(format_zv));
		zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "No encoder registered for image format %s", Z_STRVAL_P(case_name));
	}

	return options_ce;
}

static bool php_gd_prepare_write_options(zval *options_zv, zval *default_options, zend_class_entry *options_ce)
{
	ZVAL_UNDEF(default_options);

	if (options_zv != NULL) {
		if (Z_OBJCE_P(options_zv) != options_ce) {
			zend_throw_exception_ex(
				php_gd_codec_exception_ce,
				0,
				"Options class %s does not match image format",
				ZSTR_VAL(Z_OBJCE_P(options_zv)->name));
			return false;
		}

		return true;
	}

	return php_gd_create_default_write_options(default_options, options_ce);
}

static gdImagePtr php_gd_prepare_transform_source(gdImagePtr src, int *background_color)
{
	gdImagePtr prepared = gdImageClone(src);

	if (prepared == NULL) {
		return NULL;
	}

	if (!gdImageTrueColor(prepared)) {
		if (*background_color >= 0 && *background_color < gdImageColorsTotal(prepared)) {
			*background_color = gdTrueColorAlpha(
				prepared->red[*background_color],
				prepared->green[*background_color],
				prepared->blue[*background_color],
				prepared->alpha[*background_color]);
		}
		if (!gdImagePaletteToTrueColor(prepared)) {
			gdImageDestroy(prepared);
			return NULL;
		}
	}

	for (int y = 0; y < gdImageSY(prepared); y++) {
		for (int x = 0; x < gdImageSX(prepared); x++) {
			if (gdTrueColorGetAlpha(prepared->tpixels[y][x]) == gdAlphaTransparent) {
				prepared->tpixels[y][x] = *background_color;
			}
		}
	}

	prepared->alphaBlendingFlag = gdEffectReplace;
	prepared->saveAlphaFlag = 1;

	return prepared;
}

static bool php_gd_is_valid_truecolor_alpha(zend_long color)
{
	return color >= 0 && color <= 0x7fffffff;
}

static bool php_gd_validate_truecolor_alpha(zend_long color, uint32_t arg_num)
{
	if (!php_gd_is_valid_truecolor_alpha(color)) {
		zend_argument_value_error(arg_num, "must be a valid GD truecolor alpha color");
		return false;
	}

	return true;
}

static bool php_gd_validate_scale_dimension(zend_long value, uint32_t arg_num)
{
	if (value <= 0 || value > INT_MAX) {
		zend_argument_value_error(arg_num, "must be between 1 and %d", INT_MAX);
		return false;
	}

	return true;
}

#ifdef PHP_GD_HAVE_SCALE_WITH_OPTIONS
static gdScaleFit php_gd_scale_fit_from_zval(zval *fit_zv)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(fit_zv))) {
		case ZEND_ENUM_Gd_ScaleFit_Cover:
			return GD_SCALE_FIT_COVER;
			case ZEND_ENUM_Gd_ScaleFit_Contain:
				return GD_SCALE_FIT_CONTAIN;
			case ZEND_ENUM_Gd_ScaleFit_Fill:
				return GD_SCALE_FIT_FILL;
			case ZEND_ENUM_Gd_ScaleFit_Inside:
				return GD_SCALE_FIT_INSIDE;
			case ZEND_ENUM_Gd_ScaleFit_Outside:
				return GD_SCALE_FIT_OUTSIDE;
			default:
				zend_throw_error(NULL, "Unsupported Gd\\ScaleFit");
				return GD_SCALE_FIT_COVER;
	}
}

static gdScaleGravity php_gd_scale_gravity_from_zval(zval *gravity_zv)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(gravity_zv))) {
		case ZEND_ENUM_Gd_ScaleGravity_Northwest:
			return GD_SCALE_GRAVITY_NORTHWEST;
		case ZEND_ENUM_Gd_ScaleGravity_North:
			return GD_SCALE_GRAVITY_NORTH;
		case ZEND_ENUM_Gd_ScaleGravity_Northeast:
			return GD_SCALE_GRAVITY_NORTHEAST;
		case ZEND_ENUM_Gd_ScaleGravity_West:
			return GD_SCALE_GRAVITY_WEST;
		case ZEND_ENUM_Gd_ScaleGravity_East:
			return GD_SCALE_GRAVITY_EAST;
		case ZEND_ENUM_Gd_ScaleGravity_Southwest:
			return GD_SCALE_GRAVITY_SOUTHWEST;
		case ZEND_ENUM_Gd_ScaleGravity_South:
			return GD_SCALE_GRAVITY_SOUTH;
		case ZEND_ENUM_Gd_ScaleGravity_Southeast:
			return GD_SCALE_GRAVITY_SOUTHEAST;
		case ZEND_ENUM_Gd_ScaleGravity_Center:
			return GD_SCALE_GRAVITY_CENTER;
		default:
			zend_throw_error(NULL, "Unsupported Gd\\ScaleGravity");
			return GD_SCALE_GRAVITY_CENTER;
	}
}

static gdScaleStrategy php_gd_scale_strategy_from_zval(zval *strategy_zv)
{
	if (Z_TYPE_P(strategy_zv) == IS_NULL) {
		return GD_SCALE_STRATEGY_NONE;
	}

	switch (zend_enum_fetch_case_id(Z_OBJ_P(strategy_zv))) {
		case ZEND_ENUM_Gd_ScaleStrategy_Entropy:
			return GD_SCALE_STRATEGY_ENTROPY;
		case ZEND_ENUM_Gd_ScaleStrategy_Attention:
			return GD_SCALE_STRATEGY_ATTENTION;
		default:
			zend_throw_error(NULL, "Unsupported Gd\\ScaleStrategy");
			return GD_SCALE_STRATEGY_NONE;
	}
}

static int php_gd_interpolation_method_from_zval(zval *interpolation_zv, int null_value)
{
	if (Z_TYPE_P(interpolation_zv) == IS_NULL) {
		return null_value;
	}

	switch (zend_enum_fetch_case_id(Z_OBJ_P(interpolation_zv))) {
		case ZEND_ENUM_Gd_InterpolationMethod_Default:
			return GD_DEFAULT;
		case ZEND_ENUM_Gd_InterpolationMethod_Bell:
			return GD_BELL;
		case ZEND_ENUM_Gd_InterpolationMethod_Bessel:
			return GD_BESSEL;
		case ZEND_ENUM_Gd_InterpolationMethod_Linear:
			return GD_LINEAR;
		case ZEND_ENUM_Gd_InterpolationMethod_CubicSpline:
			return GD_CUBIC_SPLINE;
		case ZEND_ENUM_Gd_InterpolationMethod_Blackman:
			return GD_BLACKMAN;
		case ZEND_ENUM_Gd_InterpolationMethod_Box:
			return GD_BOX;
		case ZEND_ENUM_Gd_InterpolationMethod_BSpline:
			return GD_BSPLINE;
		case ZEND_ENUM_Gd_InterpolationMethod_CatmullRom:
			return GD_CATMULLROM;
		case ZEND_ENUM_Gd_InterpolationMethod_Gaussian:
			return GD_GAUSSIAN;
		case ZEND_ENUM_Gd_InterpolationMethod_GeneralizedCubic:
			return GD_GENERALIZED_CUBIC;
		case ZEND_ENUM_Gd_InterpolationMethod_Hermite:
			return GD_HERMITE;
		case ZEND_ENUM_Gd_InterpolationMethod_Hamming:
			return GD_HAMMING;
		case ZEND_ENUM_Gd_InterpolationMethod_Hanning:
			return GD_HANNING;
		case ZEND_ENUM_Gd_InterpolationMethod_Mitchell:
			return GD_MITCHELL;
		case ZEND_ENUM_Gd_InterpolationMethod_NearestNeighbour:
			return GD_NEAREST_NEIGHBOUR;
		case ZEND_ENUM_Gd_InterpolationMethod_Power:
			return GD_POWER;
		case ZEND_ENUM_Gd_InterpolationMethod_Quadratic:
			return GD_QUADRATIC;
		case ZEND_ENUM_Gd_InterpolationMethod_Sinc:
			return GD_SINC;
		case ZEND_ENUM_Gd_InterpolationMethod_Triangle:
			return GD_TRIANGLE;
		case ZEND_ENUM_Gd_InterpolationMethod_Weighted4:
			return GD_WEIGHTED4;
		case ZEND_ENUM_Gd_InterpolationMethod_Lanczos3:
			return GD_LANCZOS3;
		case ZEND_ENUM_Gd_InterpolationMethod_Lanczos8:
			return GD_LANCZOS8;
		case ZEND_ENUM_Gd_InterpolationMethod_BlackmanBessel:
			return GD_BLACKMAN_BESSEL;
		case ZEND_ENUM_Gd_InterpolationMethod_BlackmanSinc:
			return GD_BLACKMAN_SINC;
		case ZEND_ENUM_Gd_InterpolationMethod_QuadraticBSpline:
			return GD_QUADRATIC_BSPLINE;
		case ZEND_ENUM_Gd_InterpolationMethod_Cosine:
			return GD_COSINE;
		case ZEND_ENUM_Gd_InterpolationMethod_Welsh:
			return GD_WELSH;
		default:
			zend_throw_error(NULL, "Unsupported Gd\\InterpolationMethod");
			return null_value;
	}
}

static zend_long php_gd_scale_round_dimension(double value)
{
	if (value < 1.0) {
		return 1;
	}
	if (value > (double) INT_MAX) {
		return (zend_long) INT_MAX + 1;
	}

	return (zend_long) floor(value + 0.5);
}

static bool php_gd_scale_resolve_auto_dimensions(
	gdImagePtr src,
	zend_long *width,
	bool *width_is_null,
	zend_long *height,
	bool *height_is_null)
{
	if (*width_is_null && *height_is_null) {
		zend_value_error("GdImage::scale(): Argument #1 ($width) and argument #2 ($height) cannot both be null");
		return false;
	}

	if (!*width_is_null && !php_gd_validate_scale_dimension(*width, 1)) {
		return false;
	}
	if (!*height_is_null && !php_gd_validate_scale_dimension(*height, 2)) {
		return false;
	}

	if (*width_is_null) {
		*width = php_gd_scale_round_dimension((double) *height * (double) gdImageSX(src) / (double) gdImageSY(src));
		*width_is_null = false;
		if (!php_gd_validate_scale_dimension(*width, 1)) {
			return false;
		}
	}
	if (*height_is_null) {
		*height = php_gd_scale_round_dimension((double) *width * (double) gdImageSY(src) / (double) gdImageSX(src));
		*height_is_null = false;
		if (!php_gd_validate_scale_dimension(*height, 2)) {
			return false;
		}
	}

	return true;
}
#endif

#ifdef PHP_GD_HAVE_AUTOCROP_WITH_OPTIONS
static int php_gd_auto_crop_mode_from_zval(zval *mode_zv)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(mode_zv))) {
		case ZEND_ENUM_Gd_AutoCropMode_Default:
			return GD_CROP_DEFAULT;
		case ZEND_ENUM_Gd_AutoCropMode_Transparent:
			return GD_CROP_TRANSPARENT;
		case ZEND_ENUM_Gd_AutoCropMode_Black:
			return GD_CROP_BLACK;
		case ZEND_ENUM_Gd_AutoCropMode_White:
			return GD_CROP_WHITE;
		case ZEND_ENUM_Gd_AutoCropMode_Sides:
			return GD_CROP_SIDES;
		case ZEND_ENUM_Gd_AutoCropMode_Threshold:
			return GD_CROP_THRESHOLD;
		default:
			zend_throw_error(NULL, "Unsupported Gd\\AutoCropMode");
			return GD_CROP_DEFAULT;
	}
}
#endif

#ifdef HAVE_GD_BUNDLED
static bool php_gd_validate_affine_matrix(const double affine[6])
{
	double inverse[6];

	for (uint32_t i = 0; i < 6; i++) {
		if (!isfinite(affine[i])) {
			zend_argument_value_error(1, "must contain only finite values");
			return false;
		}
	}

	if (gdAffineInvert(inverse, affine) != GD_TRUE) {
		zend_argument_value_error(1, "must be invertible");
		return false;
	}

	return true;
}
#endif

PHP_METHOD(Gd_Image, __construct)
{
}

PHP_METHOD(Gd_Image, create)
{
	zend_long width, height;
	zval *color_model_zv = NULL;
	gdImagePtr im;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(color_model_zv, php_gd_color_model_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (width <= 0 || width >= INT_MAX) {
		zend_argument_value_error(1, "must be greater than 0");
		RETURN_THROWS();
	}

	if (height <= 0 || height >= INT_MAX) {
		zend_argument_value_error(2, "must be greater than 0");
		RETURN_THROWS();
	}

	if (color_model_zv == NULL) {
		im = gdImageCreateTrueColor(width, height);
	} else {
		switch (zend_enum_fetch_case_id(Z_OBJ_P(color_model_zv))) {
			case ZEND_ENUM_Gd_ColorModel_TrueColor:
				im = gdImageCreateTrueColor(width, height);
				break;
			case ZEND_ENUM_Gd_ColorModel_Palette:
				im = gdImageCreate(width, height);
				break;
			default:
				zend_throw_error(NULL, "Unsupported Gd\\ColorModel");
				RETURN_THROWS();
		}
	}

	if (im == NULL) {
		zend_throw_error(NULL, "Failed to create GdImage");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, im);
}

#ifdef PHP_GD_HAVE_IMAGE_READ_CTX_EX
static void php_gd_throw_load_exception(gdImageReadStatus status, const char *format_name)
{
	switch (status) {
		case gdImageReadStatusUnrecognized:
			zend_throw_exception(php_gd_codec_exception_ce, "Unrecognized image format", 0);
			return;
		case gdImageReadStatusUnsupportedFormat:
			zend_throw_exception(php_gd_codec_exception_ce, "Image format is not supported by Gd\\Image::loadFrom*()", 0);
			return;
		case gdImageReadStatusCodecUnavailable:
			if (format_name != NULL) {
				zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "%s codec is not available in this build", format_name);
			} else {
				zend_throw_exception(php_gd_codec_exception_ce, "Image codec is not available in this build", 0);
			}
			return;
		case gdImageReadStatusDecodeFailed:
			if (format_name != NULL) {
				zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "Failed to decode %s image", format_name);
			} else {
				zend_throw_exception(php_gd_codec_exception_ce, "Failed to decode image", 0);
			}
			return;
		case gdImageReadStatusOk:
			break;
	}

	zend_throw_exception(php_gd_codec_exception_ce, "Failed to load image", 0);
}
#endif

static void php_gd_image_load_from_bytes(zval *return_value, zend_string *bytes)
{
#ifdef PHP_GD_HAVE_IMAGE_READ_CTX_EX
	gdIOCtxPtr ctx;
	gdImagePtr im;
	gdImageReadStatus status;
	const char *format_name = NULL;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_throw_exception(php_gd_codec_exception_ce, "Image input is too large", 0);
		return;
	}

	ctx = gdNewDynamicCtxEx((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), 0);
	if (ctx == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to create image input context", 0);
		return;
	}

	im = gdImageReadCtxEx(ctx, GD_IMAGE_READ_RESTRICT_CODEC_API, &status, &format_name);
	ctx->gd_free(ctx);
	if (im == NULL) {
		php_gd_throw_load_exception(status, format_name);
		return;
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, im);
#else
	(void) bytes;
	zend_throw_exception(php_gd_codec_exception_ce, "Gd\\Image::loadFrom*(): Codec read API is not supported by this GD library", 0);
#endif
}

static zend_string *php_gd_read_stream_all(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

PHP_METHOD(Gd_Image, loadFromFile)
{
	zend_string *path;
	php_stream *stream;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(path)
	ZEND_PARSE_PARAMETERS_END();

	stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);
	if (stream == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to open image input", 0);
		RETURN_THROWS();
	}

	bytes = php_gd_read_stream_all(stream);
	php_stream_close(stream);
	if (bytes == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to read image input", 0);
		RETURN_THROWS();
	}

	php_gd_image_load_from_bytes(return_value, bytes);
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Image, loadFromStream)
{
	zval *stream_zv;
	php_stream *stream;
	zend_string *bytes;

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

	bytes = php_gd_read_stream_all(stream);
	if (bytes == NULL) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to read image input", 0);
		RETURN_THROWS();
	}

	php_gd_image_load_from_bytes(return_value, bytes);
	zend_string_release_ex(bytes, 0);
}

PHP_METHOD(Gd_Image, loadFromString)
{
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(bytes)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_image_load_from_bytes(return_value, bytes);
}

PHP_METHOD(Gd_PerceptualDiffOptions, __construct)
{
	zval *mode_zv = NULL;
	zval default_mode;
	zend_long highlight_color = 0xFF0000;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(mode_zv, php_gd_perceptual_diff_mode_ce)
		Z_PARAM_LONG(highlight_color)
	ZEND_PARSE_PARAMETERS_END();

	if (highlight_color < INT_MIN || highlight_color > INT_MAX) {
		zend_argument_value_error(2, "must be between %d and %d", INT_MIN, INT_MAX);
		RETURN_THROWS();
	}
	if (mode_zv == NULL) {
		ZVAL_OBJ(&default_mode, zend_enum_get_case_by_id(php_gd_perceptual_diff_mode_ce, ZEND_ENUM_Gd_PerceptualDiffMode_None));
		mode_zv = &default_mode;
	}
	zend_update_property(php_gd_perceptual_diff_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("mode"), mode_zv);
	zend_update_property_long(php_gd_perceptual_diff_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("highlightColor"), highlight_color);
}

PHP_METHOD(Gd_PerceptualDiffResult, __construct)
{
	zval *diff_image_zv = NULL;
	zend_long pixels_changed;
	double maximum_delta;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(pixels_changed)
		Z_PARAM_DOUBLE(maximum_delta)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(diff_image_zv, gd_image_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (pixels_changed < 0 || pixels_changed > UINT_MAX) {
		zend_argument_value_error(1, "must be between 0 and %u", UINT_MAX);
		RETURN_THROWS();
	}
	if (!(maximum_delta >= 0.0 && maximum_delta <= 1.0)) {
		zend_argument_value_error(2, "must be between 0.0 and 1.0");
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_perceptual_diff_result_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("pixelsChanged"), pixels_changed);
	zend_update_property_double(php_gd_perceptual_diff_result_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("maximumDelta"), maximum_delta);
	if (diff_image_zv != NULL) {
		zend_update_property(php_gd_perceptual_diff_result_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("diffImage"), diff_image_zv);
	} else {
		zend_update_property_null(php_gd_perceptual_diff_result_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("diffImage"));
	}
}

PHP_METHOD(Gd_RotateOptions, __construct)
{
	zend_long background_color = 0x7f000000;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(background_color)
	ZEND_PARSE_PARAMETERS_END();

	if (background_color < 0 || background_color > INT_MAX) {
		zend_argument_value_error(1, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_rotate_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("backgroundColor"), background_color);
}

PHP_METHOD(Gd_ScaleOptions, __construct)
{
	zend_long width = 0, height = 0;
	bool width_is_null = true, height_is_null = true;
	zval *fit_zv = NULL, *gravity_zv = NULL, *strategy_zv = NULL;
	zval *interpolation_zv = NULL;
	zval default_fit, default_gravity;
	zend_long background_color = 0x7f000000;

	ZEND_PARSE_PARAMETERS_START(0, 7)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG_OR_NULL(width, width_is_null)
		Z_PARAM_LONG_OR_NULL(height, height_is_null)
		Z_PARAM_OBJECT_OF_CLASS(fit_zv, php_gd_scale_fit_ce)
		Z_PARAM_OBJECT_OF_CLASS(gravity_zv, php_gd_scale_gravity_ce)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(strategy_zv, php_gd_scale_strategy_ce)
		Z_PARAM_LONG(background_color)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(interpolation_zv, php_gd_interpolation_method_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!width_is_null && !php_gd_validate_scale_dimension(width, 1)) {
		RETURN_THROWS();
	}
	if (!height_is_null && !php_gd_validate_scale_dimension(height, 2)) {
		RETURN_THROWS();
	}
	if (!php_gd_validate_truecolor_alpha(background_color, 6)) {
		RETURN_THROWS();
	}

	if (fit_zv == NULL) {
		ZVAL_OBJ(&default_fit, zend_enum_get_case_by_id(php_gd_scale_fit_ce, ZEND_ENUM_Gd_ScaleFit_Cover));
		fit_zv = &default_fit;
	}
	if (gravity_zv == NULL) {
		ZVAL_OBJ(&default_gravity, zend_enum_get_case_by_id(php_gd_scale_gravity_ce, ZEND_ENUM_Gd_ScaleGravity_Center));
		gravity_zv = &default_gravity;
	}

	if (width_is_null) {
		zend_update_property_null(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"));
	} else {
		zend_update_property_long(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	}
	if (height_is_null) {
		zend_update_property_null(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"));
	} else {
		zend_update_property_long(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	}
	zend_update_property(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("fit"), fit_zv);
	zend_update_property(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("gravity"), gravity_zv);
	if (strategy_zv == NULL) {
		zend_update_property_null(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("strategy"));
	} else {
		zend_update_property(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("strategy"), strategy_zv);
	}
	zend_update_property_long(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("backgroundColor"), background_color);
	if (interpolation_zv == NULL) {
		zend_update_property_null(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interpolation"));
	} else {
		zend_update_property(php_gd_scale_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interpolation"), interpolation_zv);
	}
}

#ifdef HAVE_GD_BUNDLED
PHP_METHOD(Gd_TransformOptions, __construct)
{
	zval *clip_zv = NULL, *interpolation_zv = NULL;
	zend_long background_color = 0x7f000000;

	ZEND_PARSE_PARAMETERS_START(0, 3)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(clip_zv, php_gd_get_rect_ce())
		Z_PARAM_LONG(background_color)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(interpolation_zv, php_gd_interpolation_method_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_validate_truecolor_alpha(background_color, 2)) {
		RETURN_THROWS();
	}

	if (clip_zv == NULL) {
		zend_update_property_null(php_gd_transform_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("clip"));
	} else {
		zend_update_property(php_gd_transform_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("clip"), clip_zv);
	}
	zend_update_property_long(php_gd_transform_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("backgroundColor"), background_color);
	if (interpolation_zv == NULL) {
		zend_update_property_null(php_gd_transform_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interpolation"));
	} else {
		zend_update_property(php_gd_transform_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("interpolation"), interpolation_zv);
	}
}
#endif

PHP_METHOD(Gd_AutoCropOptions, __construct)
{
	zval *mode_zv = NULL;
	zval default_mode;
	double threshold = 0.5;
	zend_long color = -1;

	ZEND_PARSE_PARAMETERS_START(0, 3)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(mode_zv, php_gd_auto_crop_mode_ce)
		Z_PARAM_DOUBLE(threshold)
		Z_PARAM_LONG(color)
	ZEND_PARSE_PARAMETERS_END();

	if (mode_zv == NULL) {
		ZVAL_OBJ(&default_mode, zend_enum_get_case_by_id(php_gd_auto_crop_mode_ce, ZEND_ENUM_Gd_AutoCropMode_Default));
		mode_zv = &default_mode;
	}

	zend_update_property(php_gd_auto_crop_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("mode"), mode_zv);
	zend_update_property_double(php_gd_auto_crop_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("threshold"), threshold);
	zend_update_property_long(php_gd_auto_crop_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("color"), color);
}

PHP_METHOD(GdImage, saveTo)
{
	zend_string *path;
	zval *format_zv = NULL;
	zval *options_zv = NULL;
	zval default_options;
	php_gd_codec_write_entry *entry;
	zend_class_entry *options_ce;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(format_zv, php_gd_codec_format_ce)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_codec_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (format_zv != NULL) {
		options_ce = php_gd_write_options_ce_from_format_zval(format_zv);
		if (options_ce == NULL) {
			RETURN_THROWS();
		}
	} else {
		options_ce = php_gd_infer_write_options_ce(path);
		if (options_ce == NULL) {
			RETURN_THROWS();
		}
	}

	if (!php_gd_prepare_write_options(options_zv, &default_options, options_ce)) {
		RETURN_THROWS();
	}
	if (options_zv == NULL) {
		options_zv = &default_options;
	}

	entry = php_gd_find_codec_write_entry(options_ce);
	if (entry == NULL) {
		if (!EG(exception)) {
			zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "No encoder registered for options class %s", ZSTR_VAL(options_ce->name));
		}
		if (!Z_ISUNDEF(default_options)) {
			zval_ptr_dtor(&default_options);
		}
		RETURN_THROWS();
	}

	php_gd_codec_write_to_file(entry, return_value, ZEND_THIS, path, options_zv);
	if (!Z_ISUNDEF(default_options)) {
		zval_ptr_dtor(&default_options);
	}
}

PHP_METHOD(GdImage, saveToStream)
{
	zval *stream_zv = NULL;
	zval *format_zv = NULL;
	zval *options_zv = NULL;
	zval output_stream_zv;
	zval default_options;
	php_gd_codec_write_entry *entry;
	zend_class_entry *options_ce;
	php_stream *output_stream = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 3)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(stream_zv)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(format_zv, php_gd_codec_format_ce)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_codec_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (format_zv == NULL) {
		zend_argument_value_error(2, "must be provided");
		RETURN_THROWS();
	}

	if (stream_zv == NULL || Z_TYPE_P(stream_zv) == IS_NULL) {
		output_stream = php_stream_open_wrapper("php://output", "wb", REPORT_ERRORS, NULL);
		if (output_stream == NULL) {
			zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "Failed to open output stream");
			RETURN_THROWS();
		}
		php_stream_to_zval(output_stream, &output_stream_zv);
		stream_zv = &output_stream_zv;
	} else if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}

	options_ce = php_gd_write_options_ce_from_format_zval(format_zv);
	if (options_ce == NULL) {
		if (output_stream != NULL) {
			zval_ptr_dtor(&output_stream_zv);
		}
		RETURN_THROWS();
	}
	if (!php_gd_prepare_write_options(options_zv, &default_options, options_ce)) {
		if (output_stream != NULL) {
			zval_ptr_dtor(&output_stream_zv);
		}
		RETURN_THROWS();
	}
	if (options_zv == NULL) {
		options_zv = &default_options;
	}

	entry = php_gd_find_codec_write_entry(options_ce);
	if (entry == NULL) {
		zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "No encoder registered for options class %s", ZSTR_VAL(options_ce->name));
		if (!Z_ISUNDEF(default_options)) {
			zval_ptr_dtor(&default_options);
		}
		if (output_stream != NULL) {
			zval_ptr_dtor(&output_stream_zv);
		}
		RETURN_THROWS();
	}

	php_gd_codec_write_to_stream(entry, return_value, ZEND_THIS, stream_zv, options_zv);
	if (!Z_ISUNDEF(default_options)) {
		zval_ptr_dtor(&default_options);
	}
	if (output_stream != NULL) {
		zval_ptr_dtor(&output_stream_zv);
	}
}

PHP_METHOD(GdImage, saveToString)
{
	zval *format_zv;
	zval *options_zv = NULL;
	zval default_options;
	php_gd_codec_write_entry *entry;
	zend_class_entry *options_ce;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(format_zv, php_gd_codec_format_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_codec_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	options_ce = php_gd_write_options_ce_from_format_zval(format_zv);
	if (options_ce == NULL) {
		RETURN_THROWS();
	}
	if (!php_gd_prepare_write_options(options_zv, &default_options, options_ce)) {
		RETURN_THROWS();
	}
	if (options_zv == NULL) {
		options_zv = &default_options;
	}

	entry = php_gd_find_codec_write_entry(options_ce);
	if (entry == NULL) {
		zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "No encoder registered for options class %s", ZSTR_VAL(options_ce->name));
		if (!Z_ISUNDEF(default_options)) {
			zval_ptr_dtor(&default_options);
		}
		RETURN_THROWS();
	}

	php_gd_codec_write_to_string(entry, return_value, ZEND_THIS, options_zv);
	if (!Z_ISUNDEF(default_options)) {
		zval_ptr_dtor(&default_options);
	}
}

PHP_METHOD(GdImage, rotate)
{
	zval *options_zv = NULL;
	zval rv, *background_color_zv;
	gdImagePtr src, rotate_src, dst;
	double degrees;
	int background_color = 0x7f000000;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_DOUBLE(degrees)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_rotate_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (degrees < (double)(INT_MIN / 100) || degrees > (double)(INT_MAX / 100)) {
		zend_argument_value_error(1, "must be between %d and %d", (INT_MIN / 100), (INT_MAX / 100));
		RETURN_THROWS();
	}

	if (options_zv != NULL) {
		background_color_zv = zend_read_property(php_gd_rotate_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("backgroundColor"), true, &rv);
		background_color = (int) Z_LVAL_P(background_color_zv);
	}

	src = php_gd_libgdimageptr_from_zval_p(ZEND_THIS);
	rotate_src = php_gd_prepare_transform_source(src, &background_color);
	if (rotate_src == NULL) {
		zend_throw_error(NULL, "Failed to rotate GdImage");
		RETURN_THROWS();
	}

	dst = gdImageRotateInterpolated(rotate_src, (const float)degrees, background_color);
	gdImageDestroy(rotate_src);

	if (dst == NULL) {
		zend_throw_error(NULL, "Failed to rotate GdImage");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, dst);
}

PHP_METHOD(GdImage, scale)
{
#ifndef PHP_GD_HAVE_SCALE_WITH_OPTIONS
	zend_throw_error(NULL, "GdImage::scale(): Options-based scale is not supported by this GD library");
	RETURN_THROWS();
#else
	zend_long width = 0, height = 0;
	bool width_is_null = true, height_is_null = true;
	zval *options_zv = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 3)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG_OR_NULL(width, width_is_null)
		Z_PARAM_LONG_OR_NULL(height, height_is_null)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_scale_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	zval rv, *width_zv, *height_zv, *fit_zv, *gravity_zv, *strategy_zv, *background_color_zv, *interpolation_zv;
	zval default_fit, default_gravity, default_strategy;
	gdImagePtr src, dst;
	int background_color = 0x7f000000;
	int interpolation = GD_SCALE_INTERPOLATION_AUTO;

	if (options_zv != NULL) {
		width_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("width"), true, &rv);
		if (Z_TYPE_P(width_zv) != IS_NULL) {
			width = Z_LVAL_P(width_zv);
			width_is_null = false;
		}
		height_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("height"), true, &rv);
		if (Z_TYPE_P(height_zv) != IS_NULL) {
			height = Z_LVAL_P(height_zv);
			height_is_null = false;
		}
		fit_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("fit"), true, &rv);
		gravity_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("gravity"), true, &rv);
		strategy_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("strategy"), true, &rv);
		background_color_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("backgroundColor"), true, &rv);
		background_color = (int) Z_LVAL_P(background_color_zv);
		interpolation_zv = zend_read_property(php_gd_scale_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("interpolation"), true, &rv);
	} else {
		ZVAL_OBJ(&default_fit, zend_enum_get_case_by_id(php_gd_scale_fit_ce, ZEND_ENUM_Gd_ScaleFit_Cover));
		ZVAL_OBJ(&default_gravity, zend_enum_get_case_by_id(php_gd_scale_gravity_ce, ZEND_ENUM_Gd_ScaleGravity_Center));
		ZVAL_NULL(&default_strategy);
		ZVAL_NULL(&rv);
		fit_zv = &default_fit;
		gravity_zv = &default_gravity;
		strategy_zv = &default_strategy;
		interpolation_zv = &rv;
	}

	if (!php_gd_validate_truecolor_alpha(background_color, 3)) {
		RETURN_THROWS();
	}

	src = php_gd_libgdimageptr_from_zval_p(ZEND_THIS);
	if (!php_gd_scale_resolve_auto_dimensions(src, &width, &width_is_null, &height, &height_is_null)) {
		RETURN_THROWS();
	}
	if (Z_TYPE_P(strategy_zv) != IS_NULL
			&& zend_enum_fetch_case_id(Z_OBJ_P(fit_zv)) != ZEND_ENUM_Gd_ScaleFit_Cover) {
		zend_value_error("GdImage::scale(): Gd\\ScaleStrategy can only be used with Gd\\ScaleFit::Cover");
		RETURN_THROWS();
	}

	{
		gdScaleOptions scale_options;

		scale_options.fit = php_gd_scale_fit_from_zval(fit_zv);
		if (EG(exception)) {
			RETURN_THROWS();
		}
		scale_options.gravity = php_gd_scale_gravity_from_zval(gravity_zv);
		if (EG(exception)) {
			RETURN_THROWS();
		}
		scale_options.strategy = php_gd_scale_strategy_from_zval(strategy_zv);
		if (EG(exception)) {
			RETURN_THROWS();
		}
		interpolation = php_gd_interpolation_method_from_zval(interpolation_zv, GD_SCALE_INTERPOLATION_AUTO);
		if (EG(exception)) {
			RETURN_THROWS();
		}
		scale_options.background_color = background_color;
		scale_options.interpolation = interpolation;

		dst = gdImageScaleWithOptions(src, (unsigned int) width, (unsigned int) height, &scale_options);
	}

	if (dst == NULL) {
		zend_throw_error(NULL, "Failed to scale GdImage");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, dst);
#endif
}

PHP_METHOD(GdImage, autoCrop)
{
#ifndef PHP_GD_HAVE_AUTOCROP_WITH_OPTIONS
	zend_throw_error(NULL, "GdImage::autoCrop(): Options-based autocrop is not supported by this GD library");
	RETURN_THROWS();
#else
	zval *options_zv = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_auto_crop_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	zval rv, *mode_zv, *threshold_zv, *color_zv;
	zval default_mode;
	gdImagePtr src, dst;
	int mode = GD_CROP_DEFAULT;
	double threshold = 0.5;
	zend_long color = -1;

	if (options_zv != NULL) {
		mode_zv = zend_read_property(php_gd_auto_crop_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("mode"), true, &rv);
		mode = php_gd_auto_crop_mode_from_zval(mode_zv);
		if (EG(exception)) {
			RETURN_THROWS();
		}
		threshold_zv = zend_read_property(php_gd_auto_crop_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("threshold"), true, &rv);
		threshold = Z_DVAL_P(threshold_zv);
		color_zv = zend_read_property(php_gd_auto_crop_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("color"), true, &rv);
		color = Z_LVAL_P(color_zv);
	} else {
		ZVAL_OBJ(&default_mode, zend_enum_get_case_by_id(php_gd_auto_crop_mode_ce, ZEND_ENUM_Gd_AutoCropMode_Default));
		mode = php_gd_auto_crop_mode_from_zval(&default_mode);
	}

	src = php_gd_libgdimageptr_from_zval_p(ZEND_THIS);

	if (mode == GD_CROP_THRESHOLD) {
		if (color < 0 || (!gdImageTrueColor(src) && color >= gdImageColorsTotal(src))) {
			zend_value_error("GdImage::autoCrop(): Gd\\AutoCropOptions::$color must be greater than or equal to 0 when using threshold mode");
			RETURN_THROWS();
		}
	}

	{
		gdAutoCropOptions autocrop_options;

		autocrop_options.mode = mode;
		autocrop_options.threshold = (float) threshold;
		autocrop_options.color = (int) color;

		dst = gdImageAutoCropWithOptions(src, &autocrop_options);
	}

	if (dst == NULL) {
		zend_throw_error(NULL, "Failed to auto-crop GdImage");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, dst);
#endif
}

#ifdef HAVE_GD_BUNDLED
PHP_METHOD(GdImage, transform)
{
	zval *matrix_zv;
	zval *options_zv = NULL;
	zval rv_clip, rv_background_color, rv_interpolation;
	zval *clip_zv = NULL, *background_color_zv = NULL, *interpolation_zv = NULL;
	gdImagePtr src, transform_src, dst;
	gdRect clip, area_full, bbox;
	gdRectPtr clip_ptr = NULL;
	double affine[6], translated_affine[6], translate[6];
	zend_long background_color = 0x7f000000;
	int interpolation = GD_SCALE_INTERPOLATION_AUTO;
	bool cloned_source = false;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(matrix_zv, php_gd_get_matrix_ce())
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_transform_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_matrix_to_affine(matrix_zv, affine);
	if (!php_gd_validate_affine_matrix(affine)) {
		RETURN_THROWS();
	}

	if (options_zv != NULL) {
		clip_zv = zend_read_property(php_gd_transform_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("clip"), true, &rv_clip);
		background_color_zv = zend_read_property(php_gd_transform_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("backgroundColor"), true, &rv_background_color);
		background_color = Z_LVAL_P(background_color_zv);
		interpolation_zv = zend_read_property(php_gd_transform_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("interpolation"), true, &rv_interpolation);
		interpolation = php_gd_interpolation_method_from_zval(interpolation_zv, GD_SCALE_INTERPOLATION_AUTO);
		if (EG(exception)) {
			RETURN_THROWS();
		}
	}

	if (clip_zv != NULL && Z_TYPE_P(clip_zv) != IS_NULL) {
		if (!php_gd_rect_to_gd_rect(clip_zv, &clip)) {
			RETURN_THROWS();
		}
		clip_ptr = &clip;
	}

	src = php_gd_libgdimageptr_from_zval_p(ZEND_THIS);
	if (clip_ptr == NULL) {
		area_full.x = 0;
		area_full.y = 0;
		area_full.width = gdImageSX(src);
		area_full.height = gdImageSY(src);
		clip_ptr = &area_full;
	}

	if (!gdImageTrueColor(src) || interpolation != GD_SCALE_INTERPOLATION_AUTO) {
		transform_src = gdImageClone(src);
		if (transform_src == NULL) {
			zend_throw_error(NULL, "Failed to transform GdImage");
			RETURN_THROWS();
		}
		cloned_source = true;
		if (!gdImageTrueColor(transform_src)) {
			gdImagePaletteToTrueColor(transform_src);
		}
	} else {
		transform_src = src;
	}

	if (interpolation != GD_SCALE_INTERPOLATION_AUTO
			&& !gdImageSetInterpolationMethod(transform_src, (gdInterpolationMethod) interpolation)) {
		if (cloned_source) {
			gdImageDestroy(transform_src);
		}
		zend_throw_error(NULL, "Failed to transform GdImage");
		RETURN_THROWS();
	}

	if (gdTransformAffineBoundingBox(clip_ptr, affine, &bbox) != GD_TRUE) {
		if (cloned_source) {
			gdImageDestroy(transform_src);
		}
		zend_throw_error(NULL, "Failed to transform GdImage");
		RETURN_THROWS();
	}

	dst = gdImageCreateTrueColor(bbox.width, bbox.height);
	if (dst == NULL) {
		if (cloned_source) {
			gdImageDestroy(transform_src);
		}
		zend_throw_error(NULL, "Failed to transform GdImage");
		RETURN_THROWS();
	}
	dst->saveAlphaFlag = 1;
	gdImageAlphaBlending(dst, 0);
	if ((background_color >> 24) >= gdAlphaTransparent) {
		gdImageFilledRectangle(dst, 0, 0, bbox.width - 1, bbox.height - 1, gdTrueColorAlpha(0, 0, 0, gdAlphaTransparent));
	} else {
		gdImageFilledRectangle(dst, 0, 0, bbox.width - 1, bbox.height - 1, (int) background_color);
		gdImageAlphaBlending(dst, 1);
	}

	gdAffineTranslate(translate, -bbox.x, -bbox.y);
	gdAffineConcat(translated_affine, affine, translate);
	if (gdTransformAffineCopy(dst, 0, 0, transform_src, clip_ptr, translated_affine) != GD_TRUE) {
		if (cloned_source) {
			gdImageDestroy(transform_src);
		}
		gdImageDestroy(dst);
		zend_throw_error(NULL, "Failed to transform GdImage");
		RETURN_THROWS();
	}

	if (cloned_source) {
		gdImageDestroy(transform_src);
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, dst);
}
#endif

PHP_METHOD(GdImage, perceptualDiff)
{
#ifdef PHP_GD_HAVE_PERCEPTUAL_DIFF
	zval *other_zv, *options_zv = NULL;
	zval rv, *mode_zv, *highlight_color_zv;
	gdImagePtr image1, image2, diff_image = NULL;
	gdImagePerceptualDiffOptions options;
	gdImagePerceptualDiffOptions *options_ptr = NULL;
	gdImagePerceptualDiffResult result;
	double threshold;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(other_zv, gd_image_ce)
		Z_PARAM_DOUBLE(threshold)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_perceptual_diff_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!(threshold >= 0.0 && threshold <= 1.0)) {
		zend_argument_value_error(2, "must be between 0.0 and 1.0");
		RETURN_THROWS();
	}

	image1 = php_gd_libgdimageptr_from_zval_p(ZEND_THIS);
	image2 = php_gd_libgdimageptr_from_zval_p(other_zv);
	if (image1->sx != image2->sx || image1->sy != image2->sy) {
		zend_argument_value_error(1, "must have the same dimensions as the receiver image");
		RETURN_THROWS();
	}

	if (options_zv != NULL) {
		mode_zv = zend_read_property(php_gd_perceptual_diff_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("mode"), true, &rv);
		highlight_color_zv = zend_read_property(php_gd_perceptual_diff_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("highlightColor"), true, &rv);
		options.mode = php_gd_perceptual_diff_mode_from_zval(mode_zv);
		if (EG(exception)) {
			RETURN_THROWS();
		}
		options.highlight_color = (int) Z_LVAL_P(highlight_color_zv);
		options_ptr = &options;
	}

	if (!gdImagePerceptualDiff(image1, image2, threshold, options_ptr,
			options_ptr != NULL && options_ptr->mode != GD_IMAGE_DIFF_NONE ? &diff_image : NULL, &result)) {
		if (diff_image != NULL) {
			gdImageDestroy(diff_image);
		}
		zend_throw_error(NULL, "Failed to compare GdImage objects");
		RETURN_THROWS();
	}

	php_gd_perceptual_diff_create_result(return_value, &result, diff_image);
#else
	zend_throw_error(NULL, "GdImage::perceptualDiff(): Perceptual diff is not supported by this GD library");
	RETURN_THROWS();
#endif
}

#ifndef HAVE_GD_BUNDLED
PHP_METHOD(GdImage, getContext)
{
	zend_throw_error(NULL, "GdImage::getContext(): 2D context is not supported by this GD library");
	RETURN_THROWS();
}
#endif

void php_gd_image_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();
	php_gd_codec_format_ce = register_class_Gd_Codec_Format();
	php_gd_codec_write_options_ce = register_class_Gd_Codec_WriteOptions();
	php_gd_codec_write_minit(php_gd_codec_write_options_ce, php_gd_codec_exception_ce);
	php_gd_color_model_ce = register_class_Gd_ColorModel();
	php_gd_rotate_options_ce = register_class_Gd_RotateOptions();
	php_gd_auto_crop_mode_ce = register_class_Gd_AutoCropMode();
	php_gd_auto_crop_options_ce = register_class_Gd_AutoCropOptions();
	php_gd_scale_fit_ce = register_class_Gd_ScaleFit();
	php_gd_scale_gravity_ce = register_class_Gd_ScaleGravity();
	php_gd_scale_strategy_ce = register_class_Gd_ScaleStrategy();
	php_gd_interpolation_method_ce = register_class_Gd_InterpolationMethod();
	php_gd_scale_options_ce = register_class_Gd_ScaleOptions();
#ifdef HAVE_GD_BUNDLED
	php_gd_transform_options_ce = register_class_Gd_TransformOptions();
#endif
	php_gd_perceptual_diff_mode_ce = register_class_Gd_PerceptualDiffMode();
	php_gd_perceptual_diff_options_ce = register_class_Gd_PerceptualDiffOptions();
	php_gd_perceptual_diff_result_ce = register_class_Gd_PerceptualDiffResult();
	register_class_Gd_Image();
}
