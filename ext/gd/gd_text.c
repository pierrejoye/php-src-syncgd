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
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>
#include <math.h>
#include <string.h>
#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gd_vector2d.h"
#else
# include <gd.h>
# include <gd_vector2d.h>
#endif

#include "gd_2d.h"
#include "gd_exception.h"
#include "gd_text_decl.h"
#include "gd_text_arginfo.h"

static zend_class_entry *php_gd_font_face_ce;
static zend_class_entry *php_gd_text_shaping_ce;
static zend_class_entry *php_gd_text_options_ce;
static zend_class_entry *php_gd_text_extents_ce;
static zend_class_entry *php_gd_text_exception_ce;
static zend_class_entry *php_gd_text_invalid_exception_ce;
static zend_class_entry *php_gd_text_unavailable_exception_ce;
static zend_class_entry *php_gd_text_font_exception_ce;
static zend_class_entry *php_gd_text_layout_exception_ce;
static zend_class_entry *php_gd_text_memory_exception_ce;
static zend_object_handlers php_gd_font_face_object_handlers;

typedef struct _php_gd_font_face_object {
	gdFontFacePtr face;
	unsigned char *data;
	size_t data_size;
	zend_object std;
} php_gd_font_face_object;

static php_gd_font_face_object *php_gd_font_face_object_from_zend_object(zend_object *zobj)
{
	return ZEND_CONTAINER_OF(zobj, php_gd_font_face_object, std);
}

static zend_object *php_gd_font_face_object_create(zend_class_entry *class_type)
{
	php_gd_font_face_object *intern = zend_object_alloc(sizeof(php_gd_font_face_object), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->face = NULL;
	intern->data = NULL;
	intern->data_size = 0;

	return &intern->std;
}

static void php_gd_font_face_object_free(zend_object *object)
{
	php_gd_font_face_object *intern = php_gd_font_face_object_from_zend_object(object);

	if (intern->face != NULL) {
		gdFontFaceDestroy(intern->face);
		intern->face = NULL;
	}
	if (intern->data != NULL) {
		efree(intern->data);
		intern->data = NULL;
		intern->data_size = 0;
	}
	zend_object_std_dtor(object);
}

static php_gd_font_face_object *php_gd_font_face_from_zval(zval *zv)
{
	php_gd_font_face_object *intern = php_gd_font_face_object_from_zend_object(Z_OBJ_P(zv));

	if (intern->face == NULL) {
		zend_throw_error(NULL, "Gd\\FontFace is not initialized");
		return NULL;
	}
	return intern;
}

static zend_class_entry *php_gd_text_exception_for_status(gdTextStatus status)
{
	switch (status) {
		case GD_TEXT_E_INVALID_ARGUMENT:
			return php_gd_text_invalid_exception_ce;
		case GD_TEXT_E_UNAVAILABLE:
			return php_gd_text_unavailable_exception_ce;
		case GD_TEXT_E_FONT:
			return php_gd_text_font_exception_ce;
		case GD_TEXT_E_LAYOUT:
			return php_gd_text_layout_exception_ce;
		case GD_TEXT_E_MEMORY:
			return php_gd_text_memory_exception_ce;
		case GD_TEXT_OK:
			return php_gd_text_exception_ce;
	}
	return php_gd_text_exception_ce;
}

static void php_gd_text_throw(gdTextStatus status, const gdTextError *err)
{
	const char *message = "Text operation failed";
	zend_long code = 0;

	if (err != NULL) {
		if (err->message[0] != '\0') {
			message = err->message;
		}
		if (err->provider_code != 0) {
			code = err->provider_code;
		}
	}
	zend_throw_exception_ex(php_gd_text_exception_for_status(status), code, "%s", message);
}

static gdTextShaping php_gd_text_shaping_from_zval(zval *zv)
{
	zend_string *name = Z_STR_P(zend_enum_fetch_case_name(Z_OBJ_P(zv)));

	if (zend_string_equals_literal(name, "None")) {
		return GD_TEXT_SHAPING_NONE;
	}
	if (zend_string_equals_literal(name, "Raqm")) {
		return GD_TEXT_SHAPING_RAQM;
	}

	ZEND_UNREACHABLE();
	return GD_TEXT_SHAPING_NONE;
}

static void php_gd_text_options_from_zval(gdTextOptions *options, zval *options_zv)
{
	zval rv;
	zval *value;

	gdTextOptionsInit(options);
	if (options_zv == NULL || Z_TYPE_P(options_zv) == IS_NULL) {
		return;
	}

	value = zend_read_property(php_gd_text_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("shaping"), false, &rv);
	options->shaping = php_gd_text_shaping_from_zval(value);
	value = zend_read_property(php_gd_text_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("lineSpacing"), false, &rv);
	options->line_spacing = zval_get_double(value);
}

static void php_gd_text_extents_create(zval *return_value, const gdTextExtents *extents)
{
	object_init_ex(return_value, php_gd_text_extents_ce);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(return_value), ZEND_STRL("xBearing"), extents->x_bearing);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(return_value), ZEND_STRL("yBearing"), extents->y_bearing);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(return_value), ZEND_STRL("width"), extents->width);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(return_value), ZEND_STRL("height"), extents->height);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(return_value), ZEND_STRL("xAdvance"), extents->x_advance);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(return_value), ZEND_STRL("yAdvance"), extents->y_advance);
}

PHP_METHOD(Gd_FontFace, __construct)
{
}

PHP_METHOD(Gd_FontFace, fromFile)
{
	zend_string *path;
	zend_long face_index = 0;
	gdTextError err;
	gdFontFacePtr face;
	php_gd_font_face_object *intern;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(face_index)
	ZEND_PARSE_PARAMETERS_END();

	if (ZSTR_LEN(path) == 0) {
		zend_argument_value_error(1, "must not be empty");
		RETURN_THROWS();
	}
	if (face_index < 0 || face_index > INT_MAX) {
		zend_argument_value_error(2, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (php_check_open_basedir(ZSTR_VAL(path))) {
		zend_argument_value_error(1, "must be a path inside open_basedir");
		RETURN_THROWS();
	}

	face = gdFontFaceCreateFromFile(ZSTR_VAL(path), (int) face_index, &err);
	if (face == NULL) {
		php_gd_text_throw(err.code, &err);
		RETURN_THROWS();
	}

	object_init_ex(return_value, php_gd_font_face_ce);
	intern = php_gd_font_face_object_from_zend_object(Z_OBJ_P(return_value));
	intern->face = face;
}

PHP_METHOD(Gd_FontFace, fromString)
{
	zend_string *bytes;
	zend_long face_index = 0;
	unsigned char *data;
	gdTextError err;
	gdFontFacePtr face;
	php_gd_font_face_object *intern;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(face_index)
	ZEND_PARSE_PARAMETERS_END();

	if (ZSTR_LEN(bytes) == 0) {
		zend_argument_value_error(1, "must not be empty");
		RETURN_THROWS();
	}
	if (face_index < 0 || face_index > INT_MAX) {
		zend_argument_value_error(2, "must be between 0 and %d", INT_MAX);
		RETURN_THROWS();
	}
	if (ZSTR_LEN(bytes) > ZEND_LONG_MAX) {
		zend_argument_value_error(1, "is too large");
		RETURN_THROWS();
	}

	data = emalloc(ZSTR_LEN(bytes));
	memcpy(data, ZSTR_VAL(bytes), ZSTR_LEN(bytes));
	face = gdFontFaceCreateFromData(data, ZSTR_LEN(bytes), (int) face_index, &err);
	if (face == NULL) {
		efree(data);
		php_gd_text_throw(err.code, &err);
		RETURN_THROWS();
	}

	object_init_ex(return_value, php_gd_font_face_ce);
	intern = php_gd_font_face_object_from_zend_object(Z_OBJ_P(return_value));
	intern->face = face;
	intern->data = data;
	intern->data_size = ZSTR_LEN(bytes);
}

PHP_METHOD(Gd_TextOptions, __construct)
{
	zval *shaping_zv = NULL;
	double line_spacing = 1.0;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(shaping_zv, php_gd_text_shaping_ce)
		Z_PARAM_DOUBLE(line_spacing)
	ZEND_PARSE_PARAMETERS_END();

	if (!isfinite(line_spacing) || line_spacing <= 0.0) {
		zend_argument_value_error(2, "must be greater than 0 and finite");
		RETURN_THROWS();
	}
	if (shaping_zv == NULL) {
		zval default_shaping_zv;

		ZVAL_OBJ(&default_shaping_zv, zend_enum_get_case_by_id(php_gd_text_shaping_ce, ZEND_ENUM_Gd_TextShaping_None));
		zend_update_property(php_gd_text_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("shaping"), &default_shaping_zv);
	} else {
		zend_update_property(php_gd_text_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("shaping"), shaping_zv);
	}
	zend_update_property_double(php_gd_text_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("lineSpacing"), line_spacing);
}

PHP_METHOD(Gd_TextExtents, __construct)
{
	double x_bearing, y_bearing, width, height, x_advance, y_advance;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_DOUBLE(x_bearing)
		Z_PARAM_DOUBLE(y_bearing)
		Z_PARAM_DOUBLE(width)
		Z_PARAM_DOUBLE(height)
		Z_PARAM_DOUBLE(x_advance)
		Z_PARAM_DOUBLE(y_advance)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xBearing"), x_bearing);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yBearing"), y_bearing);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xAdvance"), x_advance);
	zend_update_property_double(php_gd_text_extents_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yAdvance"), y_advance);
}

PHP_METHOD(Gd_Context, setFontFace)
{
	php_gd_context_object *context;
	php_gd_font_face_object *font;
	zval *font_zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(font_zv, php_gd_font_face_ce)
	ZEND_PARSE_PARAMETERS_END();

	context = php_gd_context_from_zval(ZEND_THIS);
	font = php_gd_font_face_from_zval(font_zv);
	if (context == NULL || font == NULL) {
		RETURN_THROWS();
	}

	gdContextSetFontFace(context->context, font->face);
	if (Z_ISUNDEF(context->font_faces)) {
		array_init(&context->font_faces);
	}
	Z_TRY_ADDREF_P(font_zv);
	zend_hash_next_index_insert(Z_ARRVAL(context->font_faces), font_zv);

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, setFontSize)
{
	php_gd_context_object *context;
	double size;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(size)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_finite_arg(size, 1)) {
		RETURN_THROWS();
	}
	if (size <= 0.0) {
		zend_argument_value_error(1, "must be greater than 0");
		RETURN_THROWS();
	}
	context = php_gd_context_from_zval(ZEND_THIS);
	if (context == NULL) {
		RETURN_THROWS();
	}

	gdContextSetFontSize(context->context, size);
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, textPath)
{
	php_gd_context_object *context;
	char *text;
	size_t text_len;
	double x, y;
	zval *options_zv = NULL;
	gdTextOptions options;
	gdTextError err;
	gdTextStatus status;

	ZEND_PARSE_PARAMETERS_START(3, 4)
		Z_PARAM_STRING(text, text_len)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_text_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_finite_arg(x, 2) ||
			!php_gd_2d_validate_finite_arg(y, 3)) {
		RETURN_THROWS();
	}
	context = php_gd_context_from_zval(ZEND_THIS);
	if (context == NULL) {
		RETURN_THROWS();
	}

	php_gd_text_options_from_zval(&options, options_zv);
	status = gdContextTextPath(context->context, text, x, y, &options, &err);
	if (status != GD_TEXT_OK) {
		php_gd_text_throw(status, &err);
		RETURN_THROWS();
	}
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, showText)
{
	php_gd_context_object *context;
	char *text;
	size_t text_len;
	double x, y;
	zval *options_zv = NULL;
	gdTextOptions options;
	gdTextError err;
	gdTextStatus status;

	ZEND_PARSE_PARAMETERS_START(3, 4)
		Z_PARAM_STRING(text, text_len)
		Z_PARAM_DOUBLE(x)
		Z_PARAM_DOUBLE(y)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_text_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_2d_validate_finite_arg(x, 2) ||
			!php_gd_2d_validate_finite_arg(y, 3)) {
		RETURN_THROWS();
	}
	context = php_gd_context_from_zval(ZEND_THIS);
	if (context == NULL) {
		RETURN_THROWS();
	}

	php_gd_text_options_from_zval(&options, options_zv);
	status = gdContextShowText(context->context, text, x, y, &options, &err);
	if (status != GD_TEXT_OK) {
		php_gd_text_throw(status, &err);
		RETURN_THROWS();
	}
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Context, textExtents)
{
	php_gd_context_object *context;
	char *text;
	size_t text_len;
	zval *options_zv = NULL;
	gdTextOptions options;
	gdTextExtents extents;
	gdTextError err;
	gdTextStatus status;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STRING(text, text_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zv, php_gd_text_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	context = php_gd_context_from_zval(ZEND_THIS);
	if (context == NULL) {
		RETURN_THROWS();
	}

	php_gd_text_options_from_zval(&options, options_zv);
	status = gdContextTextExtents(context->context, text, &options, &extents, &err);
	if (status != GD_TEXT_OK) {
		php_gd_text_throw(status, &err);
		RETURN_THROWS();
	}
	php_gd_text_extents_create(return_value, &extents);
}

void php_gd_text_minit(void)
{
	zend_class_entry *context_ce;

	php_gd_text_exception_ce = register_class_Gd_Text_TextException(php_gd_get_exception_ce());
	php_gd_text_invalid_exception_ce = register_class_Gd_Text_InvalidTextException(php_gd_text_exception_ce);
	php_gd_text_unavailable_exception_ce = register_class_Gd_Text_UnavailableException(php_gd_text_exception_ce);
	php_gd_text_font_exception_ce = register_class_Gd_Text_FontException(php_gd_text_exception_ce);
	php_gd_text_layout_exception_ce = register_class_Gd_Text_LayoutException(php_gd_text_exception_ce);
	php_gd_text_memory_exception_ce = register_class_Gd_Text_MemoryException(php_gd_text_exception_ce);

	php_gd_text_shaping_ce = register_class_Gd_TextShaping();
	php_gd_font_face_ce = register_class_Gd_FontFace();
	php_gd_text_options_ce = register_class_Gd_TextOptions();
	php_gd_text_extents_ce = register_class_Gd_TextExtents();

	php_gd_font_face_ce->create_object = php_gd_font_face_object_create;
	php_gd_font_face_ce->default_object_handlers = &php_gd_font_face_object_handlers;

	memcpy(&php_gd_font_face_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_font_face_object_handlers.clone_obj = NULL;
	php_gd_font_face_object_handlers.free_obj = php_gd_font_face_object_free;
	php_gd_font_face_object_handlers.compare = zend_objects_not_comparable;
	php_gd_font_face_object_handlers.offset = offsetof(php_gd_font_face_object, std);

	context_ce = php_gd_get_context_ce();
	ZEND_ASSERT(context_ce != NULL);
	zend_register_functions(context_ce, class_Gd_Context_methods, &context_ce->function_table, MODULE_PERSISTENT);
}
