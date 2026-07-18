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
#include "gd_jpeg.h"
#include "gd_metadata.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>
#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_jpeg_arginfo.h"
#include "gd_jpeg_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#ifdef HAVE_GD_JPEG_CODEC
static zend_class_entry *php_gd_jpeg_color_space_ce;
static zend_class_entry *php_gd_jpeg_density_unit_ce;
static zend_class_entry *php_gd_jpeg_dct_method_ce;
static zend_class_entry *php_gd_jpeg_info_ce;
static zend_class_entry *php_gd_jpeg_read_options_ce;
static zend_class_entry *php_gd_jpeg_write_options_ce;
static zend_class_entry *php_gd_jpeg_reader_ce;
static zend_object_handlers php_gd_jpeg_reader_handlers;

typedef struct {
	zend_string *bytes;
	zval info;
	gdJpegReadOptions read_options;
	bool read;
	bool failed;
	zend_object std;
} php_gd_jpeg_reader_object;

static php_gd_jpeg_reader_object *php_gd_jpeg_reader_from_object(zend_object *object)
{
	return (php_gd_jpeg_reader_object *) ((char *) object - offsetof(php_gd_jpeg_reader_object, std));
}

#define Z_GD_JPEG_READER_P(zv) php_gd_jpeg_reader_from_object(Z_OBJ_P((zv)))

static void php_gd_jpeg_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static zend_string *php_gd_jpeg_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_jpeg_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_jpeg_throw("Failed to open JPEG input");
		return false;
	}
	*bytes = php_gd_jpeg_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_jpeg_throw("Failed to read JPEG input");
		return false;
	}
	return true;
}

static bool php_gd_jpeg_read_stream_bytes(zval *stream_zv, zend_string **bytes)
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
	*bytes = php_gd_jpeg_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_jpeg_throw("Failed to read JPEG input");
		return false;
	}
	return true;
}

static bool php_gd_jpeg_validate_bytes(zend_string *bytes, uint32_t arg_num)
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

static bool php_gd_jpeg_validate_input_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) == 0 || ZSTR_LEN(bytes) > INT_MAX) {
		php_gd_jpeg_throw("Failed to read JPEG input");
		return false;
	}
	return true;
}

static bool php_gd_jpeg_build_read_options(zval *options_zv, gdJpegReadOptions *options)
{
	zval rv;
	zval *value;
	unsigned int numerator, denominator, original_denominator, divisor;

	gdJpegReadOptionsInit(options);
	if (options_zv == NULL) {
		return true;
	}

	value = zend_read_property(php_gd_jpeg_read_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("ignoreWarnings"), true, &rv);
	options->ignore_warning = zend_is_true(value);

	value = zend_read_property(php_gd_jpeg_read_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("scaleNumerator"), true, &rv);
	numerator = (unsigned int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_jpeg_read_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("scaleDenominator"), true, &rv);
	denominator = (unsigned int) Z_LVAL_P(value);
	original_denominator = denominator;

	if (numerator == 0 || denominator == 0) {
		return false;
	}
	divisor = numerator;
	while (denominator != 0) {
		unsigned int t = denominator;
		denominator = divisor % denominator;
		divisor = t;
	}
	options->scale_num = numerator / divisor;
	options->scale_denom = original_denominator / divisor;

	value = zend_read_property(php_gd_jpeg_read_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("dctMethod"), true, &rv);
	options->dct_method = (int) Z_LVAL_P(zend_enum_fetch_case_value(Z_OBJ_P(value)));
	return true;
}

static gdImagePtr php_gd_jpeg_decode_bytes(zend_string *bytes, const gdJpegReadOptions *options)
{
	return gdImageCreateFromJpegPtrWithOptions((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), options);
}

static void php_gd_jpeg_color_space(zval *result, int color_space)
{
	zend_long case_id;

	switch (color_space) {
		case GD_JPEG_COLOR_SPACE_UNKNOWN: case_id = ZEND_ENUM_Gd_Jpeg_ColorSpace_Unknown; break;
		case GD_JPEG_COLOR_SPACE_GRAYSCALE: case_id = ZEND_ENUM_Gd_Jpeg_ColorSpace_Grayscale; break;
		case GD_JPEG_COLOR_SPACE_RGB: case_id = ZEND_ENUM_Gd_Jpeg_ColorSpace_Rgb; break;
		case GD_JPEG_COLOR_SPACE_YCBCR: case_id = ZEND_ENUM_Gd_Jpeg_ColorSpace_YCbCr; break;
		case GD_JPEG_COLOR_SPACE_CMYK: case_id = ZEND_ENUM_Gd_Jpeg_ColorSpace_Cmyk; break;
		case GD_JPEG_COLOR_SPACE_YCCK: case_id = ZEND_ENUM_Gd_Jpeg_ColorSpace_Ycck; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_jpeg_color_space_ce, case_id));
}

static void php_gd_jpeg_density_unit(zval *result, int density_unit)
{
	zend_long case_id;

	switch (density_unit) {
		case GD_JPEG_DENSITY_UNIT_NONE: case_id = ZEND_ENUM_Gd_Jpeg_DensityUnit_None; break;
		case GD_JPEG_DENSITY_UNIT_DPI: case_id = ZEND_ENUM_Gd_Jpeg_DensityUnit_Dpi; break;
		case GD_JPEG_DENSITY_UNIT_DPCM: case_id = ZEND_ENUM_Gd_Jpeg_DensityUnit_Dpcm; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_jpeg_density_unit_ce, case_id));
}

static void php_gd_jpeg_create_info(zval *result, const gdJpegInfo *info, gdImageMetadata *metadata)
{
	zval value;

	object_init_ex(result, php_gd_jpeg_info_ce);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("bitsPerSample"), info->bits_per_sample);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("components"), info->components);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("colorSpaceTag"), info->color_space);
	php_gd_jpeg_color_space(&value, info->color_space);
	zend_update_property(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("colorSpace"), &value);
	zend_update_property_bool(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("progressive"), info->progressive != 0);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("densityUnitTag"), info->density_unit);
	php_gd_jpeg_density_unit(&value, info->density_unit);
	zend_update_property(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("densityUnit"), &value);
	if (info->x_density >= 0) {
		zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("xDensity"), info->x_density);
	} else {
		zend_update_property_null(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("xDensity"));
	}
	if (info->y_density >= 0) {
		zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("yDensity"), info->y_density);
	} else {
		zend_update_property_null(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("yDensity"));
	}
	php_gd_metadata_create_zval(&value, metadata);
	zend_update_property(php_gd_jpeg_info_ce, Z_OBJ_P(result), ZEND_STRL("metadata"), &value);
	zval_ptr_dtor(&value);
}

static bool php_gd_jpeg_create_info_from_bytes(zval *info_zv, zend_string *bytes)
{
	gdJpegInfo info;
	gdImageMetadata *metadata;

	gdJpegInfoInit(&info);
	if (gdJpegGetInfoPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &info) != 0) {
		php_gd_jpeg_throw("Failed to read JPEG info");
		return false;
	}
	metadata = gdImageMetadataCreate();
	if (metadata == NULL) {
		php_gd_jpeg_throw("Failed to create JPEG metadata");
		return false;
	}
	if (gdJpegGetMetadataPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), metadata) != 0) {
		gdImageMetadataFree(metadata);
		php_gd_jpeg_throw("Failed to read JPEG metadata");
		return false;
	}
	php_gd_jpeg_create_info(info_zv, &info, metadata);
	return true;
}

static void php_gd_jpeg_build_options(zval *options_zv, gdJpegWriteOptions *options)
{
	zval rv;
	zval *value;

	gdJpegWriteOptionsInit(options);
	if (options_zv == NULL) {
		return;
	}

	value = zend_read_property(php_gd_jpeg_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("quality"), true, &rv);
	options->quality = (int) Z_LVAL_P(value);
	value = zend_read_property(php_gd_jpeg_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("progressive"), true, &rv);
	options->progressive = zend_is_true(value);
	value = zend_read_property(php_gd_jpeg_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("forceNoSubsampling"), true, &rv);
	options->force_no_subsampling = zend_is_true(value);
	value = zend_read_property(php_gd_jpeg_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
	if (Z_TYPE_P(value) == IS_OBJECT) {
		options->metadata = php_gd_metadata_from_zval(value);
	}
}

PHP_METHOD(Gd_Jpeg_ReadOptions, __construct)
{
	bool ignore_warnings = true;
	zend_long scale_numerator = 1, scale_denominator = 1;
	zval *dct_method = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(ignore_warnings)
		Z_PARAM_LONG(scale_numerator)
		Z_PARAM_LONG(scale_denominator)
		Z_PARAM_OBJECT_OF_CLASS(dct_method, php_gd_jpeg_dct_method_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (scale_numerator < 1 || scale_numerator > UINT_MAX) {
		zend_argument_value_error(2, "must be between 1 and %u", UINT_MAX);
		RETURN_THROWS();
	}
	if (scale_denominator < 1 || scale_denominator > UINT_MAX) {
		zend_argument_value_error(3, "must be between 1 and %u", UINT_MAX);
		RETURN_THROWS();
	}

	zend_update_property_bool(php_gd_jpeg_read_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("ignoreWarnings"), ignore_warnings);
	zend_update_property_long(php_gd_jpeg_read_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("scaleNumerator"), scale_numerator);
	zend_update_property_long(php_gd_jpeg_read_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("scaleDenominator"), scale_denominator);
	if (dct_method) {
		zend_update_property(php_gd_jpeg_read_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("dctMethod"), dct_method);
	} else {
		zval default_dct_method;
		ZVAL_OBJ(&default_dct_method, zend_enum_get_case_by_id(php_gd_jpeg_dct_method_ce, ZEND_ENUM_Gd_Jpeg_DctMethod_Default));
		zend_update_property(php_gd_jpeg_read_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("dctMethod"), &default_dct_method);
	}
}

PHP_METHOD(Gd_Jpeg_WriteOptions, __construct)
{
	zend_long quality = -1;
	bool progressive = false, force_no_subsampling = false;
	zval *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(quality)
		Z_PARAM_BOOL(progressive)
		Z_PARAM_BOOL(force_no_subsampling)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (quality < -1 || quality > 100) {
		zend_argument_value_error(1, "must be between -1 and 100");
		RETURN_THROWS();
	}

	zend_update_property_long(php_gd_jpeg_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("quality"), quality);
	zend_update_property_bool(php_gd_jpeg_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("progressive"), progressive);
	zend_update_property_bool(php_gd_jpeg_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("forceNoSubsampling"), force_no_subsampling);
	if (metadata) zend_update_property(php_gd_jpeg_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata); else zend_update_property_null(php_gd_jpeg_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
}

PHP_METHOD(Gd_Jpeg_Info, __construct)
{
	zend_long width, height, bits_per_sample, components, color_space_tag, density_unit_tag;
	zval *color_space = NULL, *density_unit = NULL;
	bool progressive;
	zend_long x_density = 0, y_density = 0;
	bool x_density_is_null = true, y_density_is_null = true;
	zval *metadata;

	ZEND_PARSE_PARAMETERS_START(12, 12)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(bits_per_sample)
		Z_PARAM_LONG(components)
		Z_PARAM_LONG(color_space_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(color_space, php_gd_jpeg_color_space_ce)
		Z_PARAM_BOOL(progressive)
		Z_PARAM_LONG(density_unit_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(density_unit, php_gd_jpeg_density_unit_ce)
		Z_PARAM_LONG_OR_NULL(x_density, x_density_is_null)
		Z_PARAM_LONG_OR_NULL(y_density, y_density_is_null)
		Z_PARAM_OBJECT_OF_CLASS(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("bitsPerSample"), bits_per_sample);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("components"), components);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorSpaceTag"), color_space_tag);
	if (color_space) zend_update_property(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorSpace"), color_space); else zend_update_property_null(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorSpace"));
	zend_update_property_bool(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("progressive"), progressive);
	zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("densityUnitTag"), density_unit_tag);
	if (density_unit) zend_update_property(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("densityUnit"), density_unit); else zend_update_property_null(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("densityUnit"));
	if (x_density_is_null) zend_update_property_null(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xDensity")); else zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xDensity"), x_density);
	if (y_density_is_null) zend_update_property_null(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yDensity")); else zend_update_property_long(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yDensity"), y_density);
	zend_update_property(php_gd_jpeg_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata);
}

PHP_METHOD(Gd_Jpeg_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

static zend_object *php_gd_jpeg_reader_create(zend_class_entry *class_entry)
{
	php_gd_jpeg_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	gdJpegReadOptionsInit(&reader->read_options);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_jpeg_reader_handlers;
	return &reader->std;
}

static void php_gd_jpeg_reader_free(zend_object *object)
{
	php_gd_jpeg_reader_object *reader = php_gd_jpeg_reader_from_object(object);

	if (reader->bytes != NULL) {
		zend_string_release(reader->bytes);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_jpeg_initialize_reader(zval *result, zend_string *bytes, const gdJpegReadOptions *options)
{
	php_gd_jpeg_reader_object *reader;

	object_init_ex(result, php_gd_jpeg_reader_ce);
	reader = Z_GD_JPEG_READER_P(result);
	reader->bytes = zend_string_copy(bytes);
	reader->read_options = *options;
	if (!php_gd_jpeg_create_info_from_bytes(&reader->info, bytes)) {
		return false;
	}
	return true;
}

PHP_METHOD(Gd_Jpeg_Reader, fromString)
{
	zend_string *bytes;
	zval *options_zv = NULL;
	gdJpegReadOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jpeg_validate_bytes(bytes, 1)) {
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_build_read_options(options_zv, &options)) {
		php_gd_jpeg_throw("Invalid JPEG read options");
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_initialize_reader(return_value, bytes, &options)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Jpeg_Reader, fromFile)
{
	zend_string *path, *bytes;
	zval *options_zv = NULL;
	gdJpegReadOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jpeg_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_build_read_options(options_zv, &options)) {
		zend_string_release(bytes);
		php_gd_jpeg_throw("Invalid JPEG read options");
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_initialize_reader(return_value, bytes, &options)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Jpeg_Reader, fromStream)
{
	zval *stream_zv;
	zval *options_zv = NULL;
	zend_string *bytes;
	gdJpegReadOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jpeg_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_build_read_options(options_zv, &options)) {
		zend_string_release(bytes);
		php_gd_jpeg_throw("Invalid JPEG read options");
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_initialize_reader(return_value, bytes, &options)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Jpeg_Reader, info)
{
	php_gd_jpeg_reader_object *reader = Z_GD_JPEG_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&reader->info);
}

PHP_METHOD(Gd_Jpeg_Reader, read)
{
	php_gd_jpeg_reader_object *reader = Z_GD_JPEG_READER_P(ZEND_THIS);
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_NONE();

	if (reader->failed) {
		php_gd_jpeg_throw("JPEG reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->read) {
		php_gd_jpeg_throw("JPEG image has already been read");
		RETURN_THROWS();
	}

	image = php_gd_jpeg_decode_bytes(reader->bytes, &reader->read_options);
	reader->read = true;
	if (image == NULL) {
		reader->failed = true;
		php_gd_jpeg_throw("Failed to decode JPEG image");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Jpeg_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Jpeg_Codec, fromString)
{
	zend_string *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;
	gdJpegReadOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jpeg_validate_bytes(bytes, 1)) {
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_build_read_options(options_zv, &options)) {
		php_gd_jpeg_throw("Invalid JPEG read options");
		RETURN_THROWS();
	}
	image = php_gd_jpeg_decode_bytes(bytes, &options);
	if (image == NULL) {
		php_gd_jpeg_throw("Failed to decode JPEG image");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Jpeg_Codec, fromFile)
{
	zend_string *path, *bytes;
	zval *options_zv = NULL;
	gdImagePtr image;
	gdJpegReadOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jpeg_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_build_read_options(options_zv, &options)) {
		zend_string_release(bytes);
		php_gd_jpeg_throw("Invalid JPEG read options");
		RETURN_THROWS();
	}
	image = php_gd_jpeg_decode_bytes(bytes, &options);
	zend_string_release(bytes);
	if (image == NULL) {
		php_gd_jpeg_throw("Failed to decode JPEG image");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Jpeg_Codec, fromStream)
{
	zval *stream_zv;
	zval *options_zv = NULL;
	zend_string *bytes;
	gdImagePtr image;
	gdJpegReadOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_jpeg_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_validate_input_bytes(bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	if (!php_gd_jpeg_build_read_options(options_zv, &options)) {
		zend_string_release(bytes);
		php_gd_jpeg_throw("Invalid JPEG read options");
		RETURN_THROWS();
	}
	image = php_gd_jpeg_decode_bytes(bytes, &options);
	zend_string_release(bytes);
	if (image == NULL) {
		php_gd_jpeg_throw("Failed to decode JPEG image");
		RETURN_THROWS();
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

static void php_gd_jpeg_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv;
	zval *destination;
	zval *options_zv = NULL;
	gdJpegWriteOptions options;
	gdIOCtx *ctx;
	int result;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ZEND_ASSERT(require_stream || Z_TYPE_P(destination) == IS_STRING);

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		php_gd_jpeg_throw("Failed to open JPEG output");
		RETURN_THROWS();
	}

	php_gd_jpeg_build_options(options_zv, &options);
	result = gdImageJpegCtxWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), ctx, &options);
	ctx->gd_free(ctx);

	if (result != 0) {
		php_gd_jpeg_throw("Failed to encode JPEG image");
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Jpeg_Codec, toFile)
{
	php_gd_jpeg_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Jpeg_Codec, toStream)
{
	php_gd_jpeg_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Jpeg_Codec, toString)
{
	zval *image_zv;
	zval *options_zv = NULL;
	gdJpegWriteOptions options;
	int size = 0;
	void *data;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_jpeg_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_jpeg_build_options(options_zv, &options);
	data = gdImageJpegPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &options);
	if (data == NULL) {
		php_gd_jpeg_throw("Failed to encode JPEG image");
		RETURN_THROWS();
	}

	RETVAL_STRINGL(data, size);
	gdFree(data);
}
#endif

void php_gd_jpeg_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_JPEG_CODEC
	zend_class_entry *codec_ce;

	php_gd_jpeg_color_space_ce = register_class_Gd_Jpeg_ColorSpace();
	php_gd_jpeg_density_unit_ce = register_class_Gd_Jpeg_DensityUnit();
	php_gd_jpeg_dct_method_ce = register_class_Gd_Jpeg_DctMethod();
	php_gd_jpeg_info_ce = register_class_Gd_Jpeg_Info();
	php_gd_jpeg_read_options_ce = register_class_Gd_Jpeg_ReadOptions();
	php_gd_jpeg_write_options_ce = register_class_Gd_Jpeg_WriteOptions(php_gd_get_codec_write_options_ce());
	php_gd_jpeg_reader_ce = register_class_Gd_Jpeg_Reader();
	php_gd_jpeg_reader_ce->create_object = php_gd_jpeg_reader_create;

	memcpy(&php_gd_jpeg_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_jpeg_reader_handlers.offset = offsetof(php_gd_jpeg_reader_object, std);
	php_gd_jpeg_reader_handlers.free_obj = php_gd_jpeg_reader_free;
	php_gd_jpeg_reader_handlers.clone_obj = NULL;

	codec_ce = register_class_Gd_Jpeg_Codec();
	php_gd_register_codec_write(php_gd_jpeg_write_options_ce, codec_ce);
	php_gd_register_codec_format("Jpeg", php_gd_jpeg_write_options_ce);
	php_gd_register_codec_extension("jpg", php_gd_jpeg_write_options_ce);
	php_gd_register_codec_extension("jpeg", php_gd_jpeg_write_options_ce);
#endif
}
