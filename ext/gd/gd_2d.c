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
#include "php_gd.h"
#include "zend_enum.h"
#include <math.h>
#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gd_vector2d.h"
#else
# include <gd.h>
#endif

#include "gd_2d.h"
#include "gd_2d_arginfo.h"

static zend_class_entry *php_gd_context_ce;
static zend_class_entry *php_gd_matrix_ce;
static zend_class_entry *php_gd_rect_ce;
static zend_class_entry *php_gd_path_ce;
static zend_class_entry *php_gd_paint_ce;
static zend_class_entry *php_gd_gradient_ce;
static zend_class_entry *php_gd_linear_gradient_ce;
static zend_class_entry *php_gd_radial_gradient_ce;
static zend_class_entry *php_gd_pattern_ce;
static zend_class_entry *php_gd_extend_mode_ce;
static zend_class_entry *php_gd_line_cap_ce;
static zend_class_entry *php_gd_line_join_ce;
static zend_class_entry *php_gd_fill_rule_ce;
static zend_class_entry *php_gd_composite_operator_ce;
static zend_object_handlers php_gd_context_object_handlers;
static zend_object_handlers php_gd_matrix_object_handlers;
static zend_object_handlers php_gd_path_object_handlers;
static zend_object_handlers php_gd_paint_object_handlers;

typedef enum {
	PHP_GD_PAINT_KIND_NONE,
	PHP_GD_PAINT_KIND_PATTERN,
	PHP_GD_PAINT_KIND_GRADIENT,
} php_gd_paint_kind;

typedef struct _php_gd_paint_object {
	gdPaintPtr paint;
	gdPathPatternPtr pattern;
	php_gd_paint_kind kind;
	zend_object std;
} php_gd_paint_object;

typedef struct _php_gd_matrix_object {
	gdPathMatrix matrix;
	zend_object std;
} php_gd_matrix_object;

typedef struct _php_gd_path_object {
	gdPathPtr path;
	zend_object std;
} php_gd_path_object;

php_gd_context_object *php_gd_context_object_from_zend_object(zend_object *zobj)
{
	return ZEND_CONTAINER_OF(zobj, php_gd_context_object, std);
}

static php_gd_matrix_object *php_gd_matrix_object_from_zend_object(zend_object *zobj)
{
	return ZEND_CONTAINER_OF(zobj, php_gd_matrix_object, std);
}

static php_gd_paint_object *php_gd_paint_object_from_zend_object(zend_object *zobj)
{
	return ZEND_CONTAINER_OF(zobj, php_gd_paint_object, std);
}

static php_gd_path_object *php_gd_path_object_from_zend_object(zend_object *zobj)
{
	return ZEND_CONTAINER_OF(zobj, php_gd_path_object, std);
}

static zend_object *php_gd_context_object_create(zend_class_entry *class_type)
{
	php_gd_context_object *intern = zend_object_alloc(sizeof(php_gd_context_object), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->context = NULL;
	ZVAL_UNDEF(&intern->image);
	ZVAL_UNDEF(&intern->font_faces);

	return &intern->std;
}

static zend_object *php_gd_matrix_object_create(zend_class_entry *class_type)
{
	php_gd_matrix_object *intern = zend_object_alloc(sizeof(php_gd_matrix_object), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	gdPathMatrixInitIdentity(&intern->matrix);

	return &intern->std;
}

static zend_object *php_gd_paint_object_create(zend_class_entry *class_type)
{
	php_gd_paint_object *intern = zend_object_alloc(sizeof(php_gd_paint_object), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->paint = NULL;
	intern->pattern = NULL;
	intern->kind = PHP_GD_PAINT_KIND_NONE;

	return &intern->std;
}

static zend_object *php_gd_path_object_create(zend_class_entry *class_type)
{
	php_gd_path_object *intern = zend_object_alloc(sizeof(php_gd_path_object), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->path = NULL;

	return &intern->std;
}

static zend_function *php_gd_context_get_constructor(zend_object *object)
{
	zend_throw_error(NULL, "You cannot initialize a Gd\\Context object except through helper functions");
	return NULL;
}

static gdExtendMode php_gd_extend_mode_from_zval(zval *zv)
{
	zend_string *name = Z_STR_P(zend_enum_fetch_case_name(Z_OBJ_P(zv)));

	if (zend_string_equals_literal(name, "None")) {
		return GD_EXTEND_NONE;
	}
	if (zend_string_equals_literal(name, "Repeat")) {
		return GD_EXTEND_REPEAT;
	}
	if (zend_string_equals_literal(name, "Reflect")) {
		return GD_EXTEND_REFLECT;
	}
	if (zend_string_equals_literal(name, "Pad")) {
		return GD_EXTEND_PAD;
	}

	ZEND_UNREACHABLE();
	return GD_EXTEND_NONE;
}

static gdLineCap php_gd_line_cap_from_zval(zval *zv)
{
	zend_string *name = Z_STR_P(zend_enum_fetch_case_name(Z_OBJ_P(zv)));

	if (zend_string_equals_literal(name, "Butt")) {
		return gdLineCapButt;
	}
	if (zend_string_equals_literal(name, "Round")) {
		return gdLineCapRound;
	}
	if (zend_string_equals_literal(name, "Square")) {
		return gdLineCapSquare;
	}

	ZEND_UNREACHABLE();
	return gdLineCapButt;
}

static gdLineJoin php_gd_line_join_from_zval(zval *zv)
{
	zend_string *name = Z_STR_P(zend_enum_fetch_case_name(Z_OBJ_P(zv)));

	if (zend_string_equals_literal(name, "Miter")) {
		return gdLineJoinMiter;
	}
	if (zend_string_equals_literal(name, "Round")) {
		return gdLineJoinRound;
	}
	if (zend_string_equals_literal(name, "Bevel")) {
		return gdLineJoinBevel;
	}

	ZEND_UNREACHABLE();
	return gdLineJoinMiter;
}

static gdFillRule php_gd_fill_rule_from_zval(zval *zv)
{
	zend_string *name = Z_STR_P(zend_enum_fetch_case_name(Z_OBJ_P(zv)));

	if (zend_string_equals_literal(name, "NonZero")) {
		return gdFillRuleNonZero;
	}
	if (zend_string_equals_literal(name, "EvenOdd")) {
		return gdFillRuleEvenOdd;
	}

	ZEND_UNREACHABLE();
	return gdFillRuleNonZero;
}

static gdCompositeOperator php_gd_composite_operator_from_zval(zval *zv)
{
	zend_string *name = Z_STR_P(zend_enum_fetch_case_name(Z_OBJ_P(zv)));

	if (zend_string_equals_literal(name, "Clear")) {
		return GD_OP_CLEAR;
	}
	if (zend_string_equals_literal(name, "Source")) {
		return GD_OP_SOURCE;
	}
	if (zend_string_equals_literal(name, "Over")) {
		return GD_OP_OVER;
	}
	if (zend_string_equals_literal(name, "In")) {
		return GD_OP_IN;
	}
	if (zend_string_equals_literal(name, "Out")) {
		return GD_OP_OUT;
	}
	if (zend_string_equals_literal(name, "Atop")) {
		return GD_OP_ATOP;
	}
	if (zend_string_equals_literal(name, "Dest")) {
		return GD_OP_DEST;
	}
	if (zend_string_equals_literal(name, "DestOver")) {
		return GD_OP_DEST_OVER;
	}
	if (zend_string_equals_literal(name, "DestIn")) {
		return GD_OP_DEST_IN;
	}
	if (zend_string_equals_literal(name, "DestOut")) {
		return GD_OP_DEST_OUT;
	}
	if (zend_string_equals_literal(name, "DestAtop")) {
		return GD_OP_DEST_ATOP;
	}
	if (zend_string_equals_literal(name, "Xor")) {
		return GD_OP_XOR;
	}
	if (zend_string_equals_literal(name, "Add")) {
		return GD_OP_ADD;
	}
	if (zend_string_equals_literal(name, "Saturate")) {
		return GD_OP_SATURATE;
	}
	if (zend_string_equals_literal(name, "Multiply")) {
		return GD_OP_MULTIPLY;
	}
	if (zend_string_equals_literal(name, "Screen")) {
		return GD_OP_SCREEN;
	}
	if (zend_string_equals_literal(name, "Overlay")) {
		return GD_OP_OVERLAY;
	}
	if (zend_string_equals_literal(name, "Darken")) {
		return GD_OP_DARKEN;
	}
	if (zend_string_equals_literal(name, "Lighten")) {
		return GD_OP_LIGHTEN;
	}
	if (zend_string_equals_literal(name, "ColorDodge")) {
		return GD_OP_COLOR_DODGE;
	}
	if (zend_string_equals_literal(name, "ColorBurn")) {
		return GD_OP_COLOR_BURN;
	}
	if (zend_string_equals_literal(name, "HardLight")) {
		return GD_OP_HARD_LIGHT;
	}
	if (zend_string_equals_literal(name, "SoftLight")) {
		return GD_OP_SOFT_LIGHT;
	}
	if (zend_string_equals_literal(name, "Difference")) {
		return GD_OP_DIFFERENCE;
	}
	if (zend_string_equals_literal(name, "Exclusion")) {
		return GD_OP_EXCLUSION;
	}
	if (zend_string_equals_literal(name, "HslHue")) {
		return GD_OP_HSL_HUE;
	}
	if (zend_string_equals_literal(name, "HslSaturation")) {
		return GD_OP_HSL_SATURATION;
	}
	if (zend_string_equals_literal(name, "HslColor")) {
		return GD_OP_HSL_COLOR;
	}
	if (zend_string_equals_literal(name, "HslLuminosity")) {
		return GD_OP_HSL_LUMINOSITY;
	}

	ZEND_UNREACHABLE();
	return GD_OP_OVER;
}

static void php_gd_context_release(php_gd_context_object *intern, bool flush)
{
	if (intern->context != NULL) {
		if (flush) {
			gdContextDestroy(intern->context);
		} else {
			gdContextDestroyNoFlush(intern->context);
		}
		intern->context = NULL;
	}
	if (!Z_ISUNDEF(intern->image)) {
		zval_ptr_dtor(&intern->image);
		ZVAL_UNDEF(&intern->image);
	}
	if (!Z_ISUNDEF(intern->font_faces)) {
		zval_ptr_dtor(&intern->font_faces);
		ZVAL_UNDEF(&intern->font_faces);
	}
}

static void php_gd_context_object_free(zend_object *object)
{
	php_gd_context_object *intern = php_gd_context_object_from_zend_object(object);

	php_gd_context_release(intern, false);
	zend_object_std_dtor(object);
}

static void php_gd_paint_object_free(zend_object *object)
{
	php_gd_paint_object *intern = php_gd_paint_object_from_zend_object(object);

	if (intern->paint != NULL) {
		gdPaintDestroy(intern->paint);
		intern->paint = NULL;
		intern->pattern = NULL;
		intern->kind = PHP_GD_PAINT_KIND_NONE;
	}
	zend_object_std_dtor(object);
}

static void php_gd_path_object_free(zend_object *object)
{
	php_gd_path_object *intern = php_gd_path_object_from_zend_object(object);

	if (intern->path != NULL) {
		gdPathDestroy(intern->path);
		intern->path = NULL;
	}
	zend_object_std_dtor(object);
}

php_gd_context_object *php_gd_context_from_zval(zval *zv)
{
	php_gd_context_object *intern = php_gd_context_object_from_zend_object(Z_OBJ_P(zv));

	if (intern->context == NULL) {
		zend_throw_error(NULL, "Gd\\Context has already been destroyed");
		return NULL;
	}
	return intern;
}

static php_gd_paint_object *php_gd_paint_from_zval(zval *zv)
{
	php_gd_paint_object *intern = php_gd_paint_object_from_zend_object(Z_OBJ_P(zv));

	if (intern->paint == NULL) {
		zend_throw_error(NULL, "Gd\\Paint is not initialized");
		return NULL;
	}
	return intern;
}

static php_gd_paint_object *php_gd_pattern_from_zval(zval *zv)
{
	php_gd_paint_object *intern = php_gd_paint_from_zval(zv);

	if (intern == NULL) {
		return NULL;
	}
	if (intern->pattern == NULL) {
		zend_throw_error(NULL, "Gd\\Pattern is not initialized");
		return NULL;
	}
	return intern;
}

static php_gd_paint_object *php_gd_gradient_from_zval(zval *zv)
{
	php_gd_paint_object *intern = php_gd_paint_from_zval(zv);

	if (intern == NULL) {
		return NULL;
	}
	if (intern->kind != PHP_GD_PAINT_KIND_GRADIENT) {
		zend_throw_error(NULL, "Gd\\Gradient is not initialized");
		return NULL;
	}
	return intern;
}

static php_gd_matrix_object *php_gd_matrix_from_zval(zval *zv)
{
	return php_gd_matrix_object_from_zend_object(Z_OBJ_P(zv));
}

static php_gd_path_object *php_gd_path_from_zval(zval *zv)
{
	php_gd_path_object *intern = php_gd_path_object_from_zend_object(Z_OBJ_P(zv));

	if (intern->path == NULL) {
		zend_throw_error(NULL, "Gd\\Path is not initialized");
		return NULL;
	}
	return intern;
}

static void php_gd_matrix_create(zval *return_value, const gdPathMatrixPtr matrix)
{
	php_gd_matrix_object *intern;

	object_init_ex(return_value, php_gd_matrix_ce);
	intern = php_gd_matrix_object_from_zend_object(Z_OBJ_P(return_value));
	memcpy(&intern->matrix, matrix, sizeof(gdPathMatrix));
}

static void php_gd_matrix_create_transformed(zval *return_value, zval *matrix_zv, void (*transform)(gdPathMatrixPtr, double, double), double x, double y)
{
	gdPathMatrix matrix;

	memcpy(&matrix, &php_gd_matrix_from_zval(matrix_zv)->matrix, sizeof(gdPathMatrix));
	transform(&matrix, x, y);
	php_gd_matrix_create(return_value, &matrix);
}

static void php_gd_rect_create(zval *return_value, const gdRectFPtr rect)
{
	object_init_ex(return_value, php_gd_rect_ce);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(return_value), ZEND_STRL("x"), rect->x);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(return_value), ZEND_STRL("y"), rect->y);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(return_value), ZEND_STRL("width"), rect->w);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(return_value), ZEND_STRL("height"), rect->h);
}

static double php_gd_rect_read_double(zval *rect_zv, const char *name, size_t name_len)
{
	zval rv;
	zval *prop = zend_read_property(php_gd_rect_ce, Z_OBJ_P(rect_zv), name, name_len, false, &rv);

	return zval_get_double(prop);
}

static gdImagePtr php_gd_context_get_image(php_gd_context_object *intern)
{
	if (Z_ISUNDEF(intern->image)) {
		zend_throw_error(NULL, "Gd\\Context is not backed by a GdImage");
		return NULL;
	}
	return php_gd_libgdimageptr_from_zval_p(&intern->image);
}

static void php_gd_context_create_from_image(zval *return_value, zval *image_zv)
{
	gdImagePtr image = php_gd_libgdimageptr_from_zval_p(image_zv);
	gdContextPtr context;
	php_gd_context_object *intern;

	if (!image->trueColor) {
		zend_throw_error(NULL, "Gd\\Context requires a truecolor GdImage");
		RETURN_THROWS();
	}

	context = gdContextCreateForImage(image);
	if (context == NULL) {
		zend_throw_error(NULL, "Failed to create Gd\\Context");
		RETURN_THROWS();
	}

	object_init_ex(return_value, php_gd_context_ce);
	intern = php_gd_context_object_from_zend_object(Z_OBJ_P(return_value));
	intern->context = context;
	ZVAL_COPY(&intern->image, image_zv);
}

static void php_gd_pattern_create_from_image(zval *return_value, zval *image_zv)
{
	gdImagePtr image = php_gd_libgdimageptr_from_zval_p(image_zv);
	gdPathPatternPtr pattern = gdPathPatternCreateForImage(image);
	gdPaintPtr paint;
	php_gd_paint_object *intern;

	if (pattern == NULL) {
		zend_throw_error(NULL, "Failed to create Gd\\Pattern");
		RETURN_THROWS();
	}

	paint = gdPaintCreateFromPattern(pattern);
	if (paint == NULL) {
		gdPathPatternDestroy(pattern);
		zend_throw_error(NULL, "Failed to create Gd\\Pattern");
		RETURN_THROWS();
	}

	object_init_ex(return_value, php_gd_pattern_ce);
	intern = php_gd_paint_object_from_zend_object(Z_OBJ_P(return_value));
	intern->paint = paint;
	intern->pattern = pattern;
	intern->kind = PHP_GD_PAINT_KIND_PATTERN;
	gdPathPatternDestroy(pattern);
}

bool php_gd_2d_validate_finite_arg(double value, uint32_t arg_num)
{
	if (!isfinite(value)) {
		zend_argument_value_error(arg_num, "must be finite");
		return false;
	}
	return true;
}

static bool php_gd_2d_validate_unit_arg(double value, uint32_t arg_num)
{
	if (!isfinite(value) || value < 0 || value > 1) {
		zend_argument_value_error(arg_num, "must be between 0.0 and 1.0");
		return false;
	}
	return true;
}

PHP_METHOD(Gd_Paint, __construct)
{
}

PHP_METHOD(Gd_Matrix, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Matrix, translation)
{
	gdPathMatrix matrix;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixInitTranslate(&matrix, x, y);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, scaling)
{
	gdPathMatrix matrix;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixInitScale(&matrix, x, y);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, shearing)
{
	gdPathMatrix matrix;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixInitShear(&matrix, x, y);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, rotation)
{
	gdPathMatrix matrix;
	double radians;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(radians)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixInitRotate(&matrix, radians);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, rotationAround)
{
	gdPathMatrix matrix;
	double radians, x, y;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_DOUBLE(radians)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixInitRotateTranslate(&matrix, radians, x, y);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, fromCoefficients)
{
	gdPathMatrix matrix;
	double m00, m10, m01, m11, m02, m12;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(m00)
		Z_PARAM_DOUBLE(m10)
		Z_PARAM_DOUBLE(m01)
		Z_PARAM_DOUBLE(m11)
		Z_PARAM_DOUBLE(m02)
		Z_PARAM_DOUBLE(m12)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixInit(&matrix, m00, m10, m01, m11, m02, m12);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, translate)
{
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_matrix_create_transformed(return_value, ZEND_THIS, gdPathMatrixTranslate, x, y);
}

PHP_METHOD(Gd_Matrix, scale)
{
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_matrix_create_transformed(return_value, ZEND_THIS, gdPathMatrixScale, x, y);
}

PHP_METHOD(Gd_Matrix, shear)
{
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_matrix_create_transformed(return_value, ZEND_THIS, gdPathMatrixShear, x, y);
}

PHP_METHOD(Gd_Matrix, rotate)
{
	gdPathMatrix matrix;
	double radians;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(radians)
	ZEND_PARSE_PARAMETERS_END();

	memcpy(&matrix, &php_gd_matrix_from_zval(ZEND_THIS)->matrix, sizeof(gdPathMatrix));
	gdPathMatrixRotate(&matrix, radians);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, rotateAround)
{
	gdPathMatrix matrix;
	double radians, x, y;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_DOUBLE(radians)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	memcpy(&matrix, &php_gd_matrix_from_zval(ZEND_THIS)->matrix, sizeof(gdPathMatrix));
	gdPathMatrixRotateTranslate(&matrix, radians, x, y);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, multiply)
{
	php_gd_matrix_object *intern;
	php_gd_matrix_object *other;
	gdPathMatrix matrix;
	zval *other_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(other_zv, php_gd_matrix_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_matrix_from_zval(ZEND_THIS);
	other = php_gd_matrix_from_zval(other_zv);
	gdPathMatrixMultiply(&matrix, &intern->matrix, &other->matrix);
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, inverted)
{
	gdPathMatrix matrix;

	ZEND_PARSE_PARAMETERS_NONE();

	memcpy(&matrix, &php_gd_matrix_from_zval(ZEND_THIS)->matrix, sizeof(gdPathMatrix));
	if (!gdPathMatrixInvert(&matrix)) {
		RETURN_NULL();
	}
	php_gd_matrix_create(return_value, &matrix);
}

PHP_METHOD(Gd_Matrix, mapPoint)
{
	double x, y, result_x, result_y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	gdPathMatrixMap(&php_gd_matrix_from_zval(ZEND_THIS)->matrix, x, y, &result_x, &result_y);
	array_init_size(return_value, 2);
	add_next_index_double(return_value, result_x);
	add_next_index_double(return_value, result_y);
}

PHP_METHOD(Gd_Matrix, mapRect)
{
	gdRectF src, dst;
	zval *rect_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(rect_zv, php_gd_rect_ce)
	ZEND_PARSE_PARAMETERS_END();

	src.x = php_gd_rect_read_double(rect_zv, ZEND_STRL("x"));
	src.y = php_gd_rect_read_double(rect_zv, ZEND_STRL("y"));
	src.w = php_gd_rect_read_double(rect_zv, ZEND_STRL("width"));
	src.h = php_gd_rect_read_double(rect_zv, ZEND_STRL("height"));
	gdPathMatrixMapRect(&php_gd_matrix_from_zval(ZEND_THIS)->matrix, &src, &dst);
	php_gd_rect_create(return_value, &dst);
}

PHP_METHOD(Gd_Rect, __construct)
{
	double x, y, width, height;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
		Z_PARAM_DOUBLE(width)
		Z_PARAM_DOUBLE(height)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("x"), x);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("y"), y);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_double(php_gd_rect_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
}

PHP_METHOD(Gd_Path, __construct)
{
	php_gd_path_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_path_object_from_zend_object(Z_OBJ_P(ZEND_THIS));
	if (intern->path != NULL) {
		gdPathDestroy(intern->path);
	}
	intern->path = gdPathCreate();
	if (intern->path == NULL) {
		zend_throw_error(NULL, "Failed to create Gd\\Path");
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Path, moveTo)
{
	php_gd_path_object *intern;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathMoveTo(intern->path, x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, relMoveTo)
{
	php_gd_path_object *intern;
	double dx, dy;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(dx)
		Z_PARAM_DOUBLE(dy)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathRelMoveTo(intern->path, dx, dy);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, lineTo)
{
	php_gd_path_object *intern;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathLineTo(intern->path, x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, relLineTo)
{
	php_gd_path_object *intern;
	double dx, dy;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(dx)
		Z_PARAM_DOUBLE(dy)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathRelLineTo(intern->path, dx, dy);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, curveTo)
{
	php_gd_path_object *intern;
	double x1, y1, x2, y2, x3, y3;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(x1)
		Z_PARAM_DOUBLE(y1)
		Z_PARAM_DOUBLE(x2)
		Z_PARAM_DOUBLE(y2)
		Z_PARAM_DOUBLE(x3)
		Z_PARAM_DOUBLE(y3)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathCurveTo(intern->path, x1, y1, x2, y2, x3, y3);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, relCurveTo)
{
	php_gd_path_object *intern;
	double dx1, dy1, dx2, dy2, dx3, dy3;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(dx1)
		Z_PARAM_DOUBLE(dy1)
		Z_PARAM_DOUBLE(dx2)
		Z_PARAM_DOUBLE(dy2)
		Z_PARAM_DOUBLE(dx3)
		Z_PARAM_DOUBLE(dy3)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathRelCurveTo(intern->path, dx1, dy1, dx2, dy2, dx3, dy3);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, quadTo)
{
	php_gd_path_object *intern;
	double x1, y1, x2, y2;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(x1)
		Z_PARAM_DOUBLE(y1)
		Z_PARAM_DOUBLE(x2)
		Z_PARAM_DOUBLE(y2)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathQuadTo(intern->path, x1, y1, x2, y2);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, relQuadTo)
{
	php_gd_path_object *intern;
	double dx1, dy1, dx2, dy2;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(dx1)
		Z_PARAM_DOUBLE(dy1)
		Z_PARAM_DOUBLE(dx2)
		Z_PARAM_DOUBLE(dy2)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathRelQuadTo(intern->path, dx1, dy1, dx2, dy2);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, arc)
{
	php_gd_path_object *intern;
	double cx, cy, radius, a0, a1;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_DOUBLE(cx)
		Z_PARAM_DOUBLE(cy)
		Z_PARAM_DOUBLE(radius)
		Z_PARAM_DOUBLE(a0)
		Z_PARAM_DOUBLE(a1)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathArc(intern->path, cx, cy, radius, a0, a1);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, negativeArc)
{
	php_gd_path_object *intern;
	double cx, cy, radius, a0, a1;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_DOUBLE(cx)
		Z_PARAM_DOUBLE(cy)
		Z_PARAM_DOUBLE(radius)
		Z_PARAM_DOUBLE(a0)
		Z_PARAM_DOUBLE(a1)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathNegativeArc(intern->path, cx, cy, radius, a0, a1);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, rect)
{
	php_gd_path_object *intern;
	double x, y, width, height;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
		Z_PARAM_DOUBLE(width)
		Z_PARAM_DOUBLE(height)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathRectangle(intern->path, x, y, width, height);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, close)
{
	php_gd_path_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathClose(intern->path);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, appendPath)
{
	php_gd_path_object *intern;
	php_gd_path_object *other;
	zval *other_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(other_zv, php_gd_path_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	other = php_gd_path_from_zval(other_zv);
	if (intern == NULL || other == NULL) {
		RETURN_THROWS();
	}

	gdPathAppendPath(intern->path, other->path);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Path, transform)
{
	php_gd_path_object *intern;
	php_gd_matrix_object *matrix;
	zval *matrix_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(matrix_zv, php_gd_matrix_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_path_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	matrix = php_gd_matrix_from_zval(matrix_zv);

	gdPathTransform(intern->path, &matrix->matrix);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Pattern, __construct)
{
}

PHP_METHOD(Gd_Gradient, __construct)
{
}

PHP_METHOD(Gd_Gradient, addColorStopRgb)
{
	php_gd_paint_object *intern;
	double offset, r, g, b;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(offset)
		Z_PARAM_DOUBLE(r)
		Z_PARAM_DOUBLE(g)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_unit_arg(offset, 1) ||
			!php_gd_2d_validate_unit_arg(r, 2) ||
			!php_gd_2d_validate_unit_arg(g, 3) ||
			!php_gd_2d_validate_unit_arg(b, 4)) {
		RETURN_THROWS();
	}

	intern = php_gd_gradient_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	if (!gdPaintAddColorStopRgb(intern->paint, offset, r, g, b)) {
		zend_throw_error(NULL, "Failed to add color stop to Gd\\Gradient");
		RETURN_THROWS();
	}
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Gradient, addColorStopRgba)
{
	php_gd_paint_object *intern;
	double offset, r, g, b, a;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_DOUBLE(offset)
		Z_PARAM_DOUBLE(r)
		Z_PARAM_DOUBLE(g)
		Z_PARAM_DOUBLE(b)
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_unit_arg(offset, 1) ||
			!php_gd_2d_validate_unit_arg(r, 2) ||
			!php_gd_2d_validate_unit_arg(g, 3) ||
			!php_gd_2d_validate_unit_arg(b, 4) ||
			!php_gd_2d_validate_unit_arg(a, 5)) {
		RETURN_THROWS();
	}

	intern = php_gd_gradient_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	if (!gdPaintAddColorStopRgba(intern->paint, offset, r, g, b, a)) {
		zend_throw_error(NULL, "Failed to add color stop to Gd\\Gradient");
		RETURN_THROWS();
	}
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Gradient, setExtend)
{
	php_gd_paint_object *intern;
	zval *extend_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(extend_zv, php_gd_extend_mode_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_gradient_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	if (!gdPaintSetExtend(intern->paint, php_gd_extend_mode_from_zval(extend_zv))) {
		zend_throw_error(NULL, "Failed to set Gd\\Gradient extend mode");
		RETURN_THROWS();
	}
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Gradient, setMatrix)
{
	php_gd_paint_object *intern;
	php_gd_matrix_object *matrix;
	zval *matrix_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(matrix_zv, php_gd_matrix_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_gradient_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	matrix = php_gd_matrix_from_zval(matrix_zv);

	if (!gdPaintSetMatrix(intern->paint, &matrix->matrix)) {
		zend_argument_value_error(1, "must be invertible");
		RETURN_THROWS();
	}
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_LinearGradient, __construct)
{
	php_gd_paint_object *intern;
	gdPaintPtr paint;
	double x0, y0, x1, y1;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(x0)
		Z_PARAM_DOUBLE(y0)
		Z_PARAM_DOUBLE(x1)
		Z_PARAM_DOUBLE(y1)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_finite_arg(x0, 1) ||
			!php_gd_2d_validate_finite_arg(y0, 2) ||
			!php_gd_2d_validate_finite_arg(x1, 3) ||
			!php_gd_2d_validate_finite_arg(y1, 4)) {
		RETURN_THROWS();
	}

	intern = php_gd_paint_object_from_zend_object(Z_OBJ_P(ZEND_THIS));
	if (intern->paint != NULL) {
		zend_throw_error(NULL, "Gd\\LinearGradient is already initialized");
		RETURN_THROWS();
	}

	paint = gdPaintCreateLinear(x0, y0, x1, y1);
	if (paint == NULL) {
		zend_throw_error(NULL, "Failed to create Gd\\LinearGradient");
		RETURN_THROWS();
	}

	intern->paint = paint;
	intern->kind = PHP_GD_PAINT_KIND_GRADIENT;
}

PHP_METHOD(Gd_RadialGradient, __construct)
{
	php_gd_paint_object *intern;
	gdPaintPtr paint;
	double x0, y0, r0, x1, y1, r1;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(x0)
		Z_PARAM_DOUBLE(y0)
		Z_PARAM_DOUBLE(r0)
		Z_PARAM_DOUBLE(x1)
		Z_PARAM_DOUBLE(y1)
		Z_PARAM_DOUBLE(r1)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_finite_arg(x0, 1) ||
			!php_gd_2d_validate_finite_arg(y0, 2) ||
			!php_gd_2d_validate_finite_arg(r0, 3) ||
			!php_gd_2d_validate_finite_arg(x1, 4) ||
			!php_gd_2d_validate_finite_arg(y1, 5) ||
			!php_gd_2d_validate_finite_arg(r1, 6)) {
		RETURN_THROWS();
	}
	if (r0 < 0) {
		zend_argument_value_error(3, "must be greater than or equal to 0");
		RETURN_THROWS();
	}
	if (r1 < 0) {
		zend_argument_value_error(6, "must be greater than or equal to 0");
		RETURN_THROWS();
	}

	intern = php_gd_paint_object_from_zend_object(Z_OBJ_P(ZEND_THIS));
	if (intern->paint != NULL) {
		zend_throw_error(NULL, "Gd\\RadialGradient is already initialized");
		RETURN_THROWS();
	}

	paint = gdPaintCreateRadial(x0, y0, r0, x1, y1, r1);
	if (paint == NULL) {
		zend_throw_error(NULL, "Failed to create Gd\\RadialGradient");
		RETURN_THROWS();
	}

	intern->paint = paint;
	intern->kind = PHP_GD_PAINT_KIND_GRADIENT;
}

PHP_METHOD(Gd_Pattern, setExtend)
{
	php_gd_paint_object *intern;
	zval *extend_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(extend_zv, php_gd_extend_mode_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_pattern_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathPatternSetExtend(intern->pattern, php_gd_extend_mode_from_zval(extend_zv));
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Pattern, setMatrix)
{
	php_gd_paint_object *intern;
	php_gd_matrix_object *matrix;
	zval *matrix_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(matrix_zv, php_gd_matrix_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_pattern_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	matrix = php_gd_matrix_from_zval(matrix_zv);

	gdPathPatternSetMatrix(intern->pattern, &matrix->matrix);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Pattern, setOpacity)
{
	php_gd_paint_object *intern;
	double opacity;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(opacity)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_pattern_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdPathPatternSetOpacity(intern->pattern, opacity);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, __construct)
{
}

PHP_METHOD(Gd_Context, createForImage)
{
	zval *image_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_context_create_from_image(return_value, image_zv);
}

PHP_METHOD(Gd_Context, getImage)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	if (Z_ISUNDEF(intern->image)) {
		zend_throw_error(NULL, "Gd\\Context is not backed by a GdImage");
		RETURN_THROWS();
	}

	ZVAL_COPY(return_value, &intern->image);
}

PHP_METHOD(Gd_Context, flushImage)
{
	php_gd_context_object *intern;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	image = php_gd_context_get_image(intern);
	if (image == NULL) {
		RETURN_THROWS();
	}
	if (!image->trueColor) {
		zend_throw_error(NULL, "Cannot flush Gd\\Context to a non-truecolor GdImage");
		RETURN_THROWS();
	}
	gdContextFlushImage(intern->context);
}

PHP_METHOD(Gd_Context, reloadImage)
{
	php_gd_context_object *intern;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	image = php_gd_context_get_image(intern);
	if (image == NULL) {
		RETURN_THROWS();
	}
	if (!image->trueColor) {
		zend_throw_error(NULL, "Cannot reload Gd\\Context from a non-truecolor GdImage");
		RETURN_THROWS();
	}
	if (!gdContextReloadImage(intern->context)) {
		zend_throw_error(NULL, "Failed to reload Gd\\Context from GdImage");
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Context, destroy)
{
	php_gd_context_object *intern;
	bool flush = true;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(flush)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_object_from_zend_object(Z_OBJ_P(ZEND_THIS));
	if (intern->context == NULL) {
		return;
	}
	if (flush && !Z_ISUNDEF(intern->image)) {
		gdImagePtr image = php_gd_libgdimageptr_from_zval_p(&intern->image);
		if (!image->trueColor) {
			zend_throw_error(NULL, "Cannot destroy and flush Gd\\Context to a non-truecolor GdImage");
			RETURN_THROWS();
		}
	}

	php_gd_context_release(intern, flush);
}

PHP_METHOD(Gd_Context, createPattern)
{
	php_gd_context_object *intern;
	zval *image_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	php_gd_pattern_create_from_image(return_value, image_zv);
}

PHP_METHOD(Gd_Context, appendPath)
{
	php_gd_context_object *intern;
	php_gd_path_object *path;
	zval *path_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(path_zv, php_gd_path_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	path = php_gd_path_from_zval(path_zv);
	if (path == NULL) {
		RETURN_THROWS();
	}

	gdContextAppendPath(intern->context, path->path);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, save)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	if (!gdContextSave(intern->context)) {
		zend_throw_error(NULL, "Failed to save Gd\\Context state");
		RETURN_THROWS();
	}

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, restore)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	if (!gdContextRestore(intern->context)) {
		zend_throw_error(NULL, "Failed to restore Gd\\Context state");
		RETURN_THROWS();
	}

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, clip)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	if (!gdContextClip(intern->context)) {
		zend_throw_error(NULL, "Failed to clip Gd\\Context");
		RETURN_THROWS();
	}

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, clipPreserve)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	if (!gdContextClipPreserve(intern->context)) {
		zend_throw_error(NULL, "Failed to clip Gd\\Context");
		RETURN_THROWS();
	}

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setSource)
{
	php_gd_context_object *intern;
	php_gd_paint_object *paint;
	zval *source_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(source_zv, php_gd_paint_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	paint = php_gd_paint_from_zval(source_zv);
	if (paint == NULL) {
		RETURN_THROWS();
	}

	gdContextSetSource(intern->context, paint->paint);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setSourceRgb)
{
	php_gd_context_object *intern;
	double r, g, b;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_DOUBLE(r)
		Z_PARAM_DOUBLE(g)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetSourceRgb(intern->context, r, g, b);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setSourceRgba)
{
	php_gd_context_object *intern;
	double r, g, b, a;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(r)
		Z_PARAM_DOUBLE(g)
		Z_PARAM_DOUBLE(b)
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetSourceRgba(intern->context, r, g, b, a);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setSourceImage)
{
	php_gd_context_object *intern;
	zval *image_zv;
	double x = 0.0, y = 0.0;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_finite_arg(x, 2) ||
			!php_gd_2d_validate_finite_arg(y, 3)) {
		RETURN_THROWS();
	}

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetSourceImage(intern->context, php_gd_libgdimageptr_from_zval_p(image_zv), x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setOperator)
{
	php_gd_context_object *intern;
	zval *operator_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(operator_zv, php_gd_composite_operator_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetOperator(intern->context, php_gd_composite_operator_from_zval(operator_zv));
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setOpacity)
{
	php_gd_context_object *intern;
	double opacity;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(opacity)
	ZEND_PARSE_PARAMETERS_END();

	if (!isfinite(opacity)) {
		zend_argument_value_error(1, "must be finite");
		RETURN_THROWS();
	}

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetOpacity(intern->context, opacity);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setLineWidth)
{
	php_gd_context_object *intern;
	double width;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(width)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetLineWidth(intern->context, width);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setLineCap)
{
	php_gd_context_object *intern;
	zval *cap_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(cap_zv, php_gd_line_cap_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetLineCap(intern->context, php_gd_line_cap_from_zval(cap_zv));
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setLineJoin)
{
	php_gd_context_object *intern;
	zval *join_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(join_zv, php_gd_line_join_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetLineJoin(intern->context, php_gd_line_join_from_zval(join_zv));
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setDash)
{
	php_gd_context_object *intern;
	HashTable *pattern;
	zval *value;
	double offset;
	double *data;
	uint32_t i = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(offset)
		Z_PARAM_ARRAY_HT(pattern)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_hash_num_elements(pattern) == 0) {
		zend_argument_value_error(2, "must not be empty");
		RETURN_THROWS();
	}
	if (zend_hash_num_elements(pattern) > INT_MAX) {
		zend_argument_value_error(2, "is too large");
		RETURN_THROWS();
	}

	data = safe_emalloc(zend_hash_num_elements(pattern), sizeof(double), 0);
	ZEND_HASH_FOREACH_VAL(pattern, value) {
		data[i] = zval_get_double(value);
		if (data[i] <= 0) {
			efree(data);
			zend_argument_value_error(2, "must contain only positive lengths");
			RETURN_THROWS();
		}
		i++;
	} ZEND_HASH_FOREACH_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		efree(data);
		RETURN_THROWS();
	}

	gdContextSetDash(intern->context, offset, data, (int) i);
	efree(data);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setFillRule)
{
	php_gd_context_object *intern;
	zval *rule_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(rule_zv, php_gd_fill_rule_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextSetFillRule(intern->context, php_gd_fill_rule_from_zval(rule_zv));
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, beginPath)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextNewPath(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, scale)
{
	php_gd_context_object *intern;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextScale(intern->context, x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, translate)
{
	php_gd_context_object *intern;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextTranslate(intern->context, x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, rotate)
{
	php_gd_context_object *intern;
	double radians;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(radians)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextRotate(intern->context, radians);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, transform)
{
	php_gd_context_object *intern;
	php_gd_matrix_object *matrix;
	zval *matrix_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(matrix_zv, php_gd_matrix_ce)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}
	matrix = php_gd_matrix_from_zval(matrix_zv);

	gdContextTransform(intern->context, &matrix->matrix);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, moveTo)
{
	php_gd_context_object *intern;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextMoveTo(intern->context, x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, relMoveTo)
{
	php_gd_context_object *intern;
	double dx, dy;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(dx)
		Z_PARAM_DOUBLE(dy)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextRelMoveTo(intern->context, dx, dy);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, lineTo)
{
	php_gd_context_object *intern;
	double x, y;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextLineTo(intern->context, x, y);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, relLineTo)
{
	php_gd_context_object *intern;
	double dx, dy;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(dx)
		Z_PARAM_DOUBLE(dy)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextRelLineTo(intern->context, dx, dy);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, curveTo)
{
	php_gd_context_object *intern;
	double x1, y1, x2, y2, x3, y3;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(x1)
		Z_PARAM_DOUBLE(y1)
		Z_PARAM_DOUBLE(x2)
		Z_PARAM_DOUBLE(y2)
		Z_PARAM_DOUBLE(x3)
		Z_PARAM_DOUBLE(y3)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextCurveTo(intern->context, x1, y1, x2, y2, x3, y3);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, relCurveTo)
{
	php_gd_context_object *intern;
	double dx1, dy1, dx2, dy2, dx3, dy3;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(dx1)
		Z_PARAM_DOUBLE(dy1)
		Z_PARAM_DOUBLE(dx2)
		Z_PARAM_DOUBLE(dy2)
		Z_PARAM_DOUBLE(dx3)
		Z_PARAM_DOUBLE(dy3)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextRelCurveTo(intern->context, dx1, dy1, dx2, dy2, dx3, dy3);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, quadTo)
{
	php_gd_context_object *intern;
	double x1, y1, x2, y2;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(x1)
		Z_PARAM_DOUBLE(y1)
		Z_PARAM_DOUBLE(x2)
		Z_PARAM_DOUBLE(y2)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextQuadTo(intern->context, x1, y1, x2, y2);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, relQuadTo)
{
	php_gd_context_object *intern;
	double dx1, dy1, dx2, dy2;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(dx1)
		Z_PARAM_DOUBLE(dy1)
		Z_PARAM_DOUBLE(dx2)
		Z_PARAM_DOUBLE(dy2)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextRelQuadTo(intern->context, dx1, dy1, dx2, dy2);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, arc)
{
	php_gd_context_object *intern;
	double cx, cy, radius, a0, a1;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_DOUBLE(cx)
		Z_PARAM_DOUBLE(cy)
		Z_PARAM_DOUBLE(radius)
		Z_PARAM_DOUBLE(a0)
		Z_PARAM_DOUBLE(a1)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextArc(intern->context, cx, cy, radius, a0, a1);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, negativeArc)
{
	php_gd_context_object *intern;
	double cx, cy, radius, a0, a1;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_DOUBLE(cx)
		Z_PARAM_DOUBLE(cy)
		Z_PARAM_DOUBLE(radius)
		Z_PARAM_DOUBLE(a0)
		Z_PARAM_DOUBLE(a1)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextNegativeArc(intern->context, cx, cy, radius, a0, a1);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, rect)
{
	php_gd_context_object *intern;
	double x, y, width, height;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
		Z_PARAM_DOUBLE(width)
		Z_PARAM_DOUBLE(height)
	ZEND_PARSE_PARAMETERS_END();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextRectangle(intern->context, x, y, width, height);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, closePath)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextClosePath(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, fill)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextFill(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, fillPreserve)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextFillPreserve(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, stroke)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextStroke(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, strokePreserve)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextStrokePreserve(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, paint)
{
	php_gd_context_object *intern;

	ZEND_PARSE_PARAMETERS_NONE();

	intern = php_gd_context_from_zval(ZEND_THIS);
	if (intern == NULL) {
		RETURN_THROWS();
	}

	gdContextPaint(intern->context);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(GdImage, getContext)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_gd_context_create_from_image(return_value, ZEND_THIS);
}

zend_class_entry *php_gd_get_context_ce(void)
{
	return php_gd_context_ce;
}

void php_gd_2d_minit(void)
{
	php_gd_extend_mode_ce = register_class_Gd_ExtendMode();
	php_gd_line_cap_ce = register_class_Gd_LineCap();
	php_gd_line_join_ce = register_class_Gd_LineJoin();
	php_gd_fill_rule_ce = register_class_Gd_FillRule();
	php_gd_composite_operator_ce = register_class_Gd_CompositeOperator();
	php_gd_matrix_ce = register_class_Gd_Matrix();
	php_gd_rect_ce = register_class_Gd_Rect();
	php_gd_path_ce = register_class_Gd_Path();
	php_gd_paint_ce = register_class_Gd_Paint();
	php_gd_gradient_ce = register_class_Gd_Gradient(php_gd_paint_ce);
	php_gd_linear_gradient_ce = register_class_Gd_LinearGradient(php_gd_gradient_ce);
	php_gd_radial_gradient_ce = register_class_Gd_RadialGradient(php_gd_gradient_ce);
	php_gd_pattern_ce = register_class_Gd_Pattern(php_gd_paint_ce);
	php_gd_context_ce = register_class_Gd_Context();

	php_gd_matrix_ce->create_object = php_gd_matrix_object_create;
	php_gd_matrix_ce->default_object_handlers = &php_gd_matrix_object_handlers;

	memcpy(&php_gd_matrix_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_matrix_object_handlers.clone_obj = NULL;
	php_gd_matrix_object_handlers.compare = zend_objects_not_comparable;
	php_gd_matrix_object_handlers.offset = offsetof(php_gd_matrix_object, std);

	php_gd_path_ce->create_object = php_gd_path_object_create;
	php_gd_path_ce->default_object_handlers = &php_gd_path_object_handlers;

	memcpy(&php_gd_path_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_path_object_handlers.clone_obj = NULL;
	php_gd_path_object_handlers.free_obj = php_gd_path_object_free;
	php_gd_path_object_handlers.compare = zend_objects_not_comparable;
	php_gd_path_object_handlers.offset = offsetof(php_gd_path_object, std);

	php_gd_paint_ce->create_object = php_gd_paint_object_create;
	php_gd_paint_ce->default_object_handlers = &php_gd_paint_object_handlers;
	php_gd_gradient_ce->create_object = php_gd_paint_object_create;
	php_gd_gradient_ce->default_object_handlers = &php_gd_paint_object_handlers;
	php_gd_linear_gradient_ce->create_object = php_gd_paint_object_create;
	php_gd_linear_gradient_ce->default_object_handlers = &php_gd_paint_object_handlers;
	php_gd_radial_gradient_ce->create_object = php_gd_paint_object_create;
	php_gd_radial_gradient_ce->default_object_handlers = &php_gd_paint_object_handlers;
	php_gd_pattern_ce->create_object = php_gd_paint_object_create;
	php_gd_pattern_ce->default_object_handlers = &php_gd_paint_object_handlers;

	memcpy(&php_gd_paint_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_paint_object_handlers.clone_obj = NULL;
	php_gd_paint_object_handlers.free_obj = php_gd_paint_object_free;
	php_gd_paint_object_handlers.compare = zend_objects_not_comparable;
	php_gd_paint_object_handlers.offset = offsetof(php_gd_paint_object, std);

	php_gd_context_ce->create_object = php_gd_context_object_create;
	php_gd_context_ce->default_object_handlers = &php_gd_context_object_handlers;

	memcpy(&php_gd_context_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_context_object_handlers.clone_obj = NULL;
	php_gd_context_object_handlers.free_obj = php_gd_context_object_free;
	php_gd_context_object_handlers.get_constructor = php_gd_context_get_constructor;
	php_gd_context_object_handlers.compare = zend_objects_not_comparable;
	php_gd_context_object_handlers.offset = offsetof(php_gd_context_object, std);
}
