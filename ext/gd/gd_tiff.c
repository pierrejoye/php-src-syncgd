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
#include "gd_tiff.h"
#include "gd_metadata.h"
#include "ext/spl/spl_exceptions.h"

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_tiff_arginfo.h"
#include "gd_tiff_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *php_gd_tiff_color_space_ce;
static zend_class_entry *php_gd_tiff_compression_ce;
static zend_class_entry *php_gd_tiff_resolution_unit_ce;

static void php_gd_tiff_throw(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}
#endif

#ifdef HAVE_GD_TIFF_READ_API
static zend_class_entry *php_gd_tiff_info_ce;
static zend_class_entry *php_gd_tiff_page_ce;
static zend_class_entry *php_gd_tiff_reader_ce;
static zend_class_entry *php_gd_tiff_photometric_ce;
static zend_class_entry *php_gd_tiff_planar_configuration_ce;
static zend_object_handlers php_gd_tiff_reader_handlers;

typedef struct {
	gdTiffReadPtr reader;
	zval info;
	bool exhausted;
	bool failed;
	zend_object std;
} php_gd_tiff_reader_object;

static php_gd_tiff_reader_object *php_gd_tiff_reader_from_object(zend_object *object)
{
	return (php_gd_tiff_reader_object *) ((char *) object - offsetof(php_gd_tiff_reader_object, std));
}

#define Z_GD_TIFF_READER_P(zv) php_gd_tiff_reader_from_object(Z_OBJ_P((zv)))

static void php_gd_tiff_read_compression(zval *result, int compression)
{
	zend_long case_id;

	switch (compression) {
		case GD_TIFF_COMPRESSION_NONE: case_id = ZEND_ENUM_Gd_Tiff_Compression_None; break;
		case GD_TIFF_COMPRESSION_CCITT_RLE: case_id = ZEND_ENUM_Gd_Tiff_Compression_CcittRle; break;
		case GD_TIFF_COMPRESSION_CCITT_FAX3: case_id = ZEND_ENUM_Gd_Tiff_Compression_CcittFax3; break;
		case GD_TIFF_COMPRESSION_CCITT_FAX4: case_id = ZEND_ENUM_Gd_Tiff_Compression_CcittFax4; break;
		case GD_TIFF_COMPRESSION_LZW: case_id = ZEND_ENUM_Gd_Tiff_Compression_Lzw; break;
		case GD_TIFF_COMPRESSION_JPEG: case_id = ZEND_ENUM_Gd_Tiff_Compression_Jpeg; break;
		case GD_TIFF_COMPRESSION_ADOBE_DEFLATE: case_id = ZEND_ENUM_Gd_Tiff_Compression_AdobeDeflate; break;
		case GD_TIFF_COMPRESSION_DEFLATE: case_id = ZEND_ENUM_Gd_Tiff_Compression_Deflate; break;
		case GD_TIFF_COMPRESSION_PACKBITS: case_id = ZEND_ENUM_Gd_Tiff_Compression_PackBits; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_tiff_compression_ce, case_id));
}

static void php_gd_tiff_read_photometric(zval *result, int photometric)
{
	zend_long case_id;

	switch (photometric) {
		case GD_TIFF_PHOTOMETRIC_MINISWHITE: case_id = ZEND_ENUM_Gd_Tiff_Photometric_MinIsWhite; break;
		case GD_TIFF_PHOTOMETRIC_MINISBLACK: case_id = ZEND_ENUM_Gd_Tiff_Photometric_MinIsBlack; break;
		case GD_TIFF_PHOTOMETRIC_RGB: case_id = ZEND_ENUM_Gd_Tiff_Photometric_Rgb; break;
		case GD_TIFF_PHOTOMETRIC_PALETTE: case_id = ZEND_ENUM_Gd_Tiff_Photometric_Palette; break;
		case GD_TIFF_PHOTOMETRIC_TRANSPARENCY_MASK: case_id = ZEND_ENUM_Gd_Tiff_Photometric_TransparencyMask; break;
		case GD_TIFF_PHOTOMETRIC_SEPARATED: case_id = ZEND_ENUM_Gd_Tiff_Photometric_Separated; break;
		case GD_TIFF_PHOTOMETRIC_YCBCR: case_id = ZEND_ENUM_Gd_Tiff_Photometric_YCbCr; break;
		case GD_TIFF_PHOTOMETRIC_CIELAB: case_id = ZEND_ENUM_Gd_Tiff_Photometric_CieLab; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_tiff_photometric_ce, case_id));
}

static void php_gd_tiff_read_planar_configuration(zval *result, int planar)
{
	zend_long case_id;

	switch (planar) {
		case GD_TIFF_PLANARCONFIG_CONTIG: case_id = ZEND_ENUM_Gd_Tiff_PlanarConfiguration_Contiguous; break;
		case GD_TIFF_PLANARCONFIG_SEPARATE: case_id = ZEND_ENUM_Gd_Tiff_PlanarConfiguration_Separate; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_tiff_planar_configuration_ce, case_id));
}

static void php_gd_tiff_read_resolution_unit(zval *result, int unit)
{
	zend_long case_id;

	switch (unit) {
		case GD_TIFF_RESUNIT_NONE: case_id = ZEND_ENUM_Gd_Tiff_ResolutionUnit_None; break;
		case GD_TIFF_RESUNIT_INCH: case_id = ZEND_ENUM_Gd_Tiff_ResolutionUnit_Inch; break;
		case GD_TIFF_RESUNIT_CENTIMETER: case_id = ZEND_ENUM_Gd_Tiff_ResolutionUnit_Centimeter; break;
		default: ZVAL_NULL(result); return;
	}
	ZVAL_OBJ(result, zend_enum_get_case_by_id(php_gd_tiff_resolution_unit_ce, case_id));
}

static void php_gd_tiff_update_nullable_double(zend_class_entry *ce, zend_object *object,
		const char *name, size_t name_len, double value)
{
	if (value > 0 && zend_finite(value)) {
		zend_update_property_double(ce, object, name, name_len, value);
	} else {
		zend_update_property_null(ce, object, name, name_len);
	}
}

static void php_gd_tiff_create_info(zval *result, const gdTiffInfo *info, gdImageMetadata *metadata)
{
	zval value;

	object_init_ex(result, php_gd_tiff_info_ce);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("pageCount"), info->pageCount);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("bitsPerSample"), info->bitsPerSample);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("samplesPerPixel"), info->samplesPerPixel);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("compressionTag"), info->compression);
	php_gd_tiff_read_compression(&value, info->compression);
	zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("compression"), &value);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("photometricTag"), info->photometric);
	php_gd_tiff_read_photometric(&value, info->photometric);
	zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("photometric"), &value);
	zend_update_property_bool(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("minIsWhite"), info->photometric == GD_TIFF_PHOTOMETRIC_MINISWHITE);
	php_gd_tiff_update_nullable_double(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("xResolution"), info->xResolution);
	php_gd_tiff_update_nullable_double(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("yResolution"), info->yResolution);
	php_gd_tiff_read_resolution_unit(&value, info->resolutionUnit);
	zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("resolutionUnit"), &value);
	php_gd_metadata_create_zval(&value, metadata);
	zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(result), ZEND_STRL("metadata"), &value);
	zval_ptr_dtor(&value);
}

static void php_gd_tiff_create_page(zval *result, gdImagePtr image, const gdTiffPageInfo *info)
{
	zval value;

	object_init_ex(result, php_gd_tiff_page_ce);
	php_gd_assign_libgdimageptr_as_extgdimage(&value, image);
	zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("image"), &value);
	zval_ptr_dtor(&value);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("pageIndex"), info->pageIndex);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("width"), info->width);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("height"), info->height);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("bitsPerSample"), info->bitsPerSample);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("samplesPerPixel"), info->samplesPerPixel);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("compressionTag"), info->compression);
	php_gd_tiff_read_compression(&value, info->compression);
	zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("compression"), &value);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("photometricTag"), info->photometric);
	php_gd_tiff_read_photometric(&value, info->photometric);
	zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("photometric"), &value);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("planarConfigurationTag"), info->planar);
	php_gd_tiff_read_planar_configuration(&value, info->planar);
	zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("planarConfiguration"), &value);
	zend_update_property_bool(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("minIsWhite"), info->photometric == GD_TIFF_PHOTOMETRIC_MINISWHITE);
	zend_update_property_bool(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("hasAlpha"), info->hasAlpha != 0);
	zend_update_property_bool(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("isTiled"), info->isTiled != 0);
	php_gd_tiff_update_nullable_double(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("xResolution"), info->xResolution);
	php_gd_tiff_update_nullable_double(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("yResolution"), info->yResolution);
	php_gd_tiff_read_resolution_unit(&value, info->resolutionUnit);
	zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(result), ZEND_STRL("resolutionUnit"), &value);
}

static zend_object *php_gd_tiff_reader_create(zend_class_entry *class_entry)
{
	php_gd_tiff_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->reader = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->exhausted = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_tiff_reader_handlers;
	return &reader->std;
}

static void php_gd_tiff_reader_free(zend_object *object)
{
	php_gd_tiff_reader_object *reader = php_gd_tiff_reader_from_object(object);

	if (reader->reader != NULL) {
		gdTiffReadClose(reader->reader);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_tiff_initialize_reader(zval *result, gdTiffReadPtr tiff)
{
	gdTiffInfo info;
	gdImageMetadata *metadata;
	php_gd_tiff_reader_object *reader;

	if (tiff == NULL || !gdTiffReadGetInfo(tiff, &info)) {
		if (tiff != NULL) {
			gdTiffReadClose(tiff);
		}
		php_gd_tiff_throw("Failed to open TIFF input");
		return false;
	}
	metadata = gdImageMetadataCreate();
	if (metadata == NULL || !gdTiffReadGetMetadata(tiff, metadata)) {
		gdImageMetadataFree(metadata);
		gdTiffReadClose(tiff);
		php_gd_tiff_throw("Failed to read TIFF metadata");
		return false;
	}
	object_init_ex(result, php_gd_tiff_reader_ce);
	reader = Z_GD_TIFF_READER_P(result);
	reader->reader = tiff;
	php_gd_tiff_create_info(&reader->info, &info, metadata);
	return true;
}

PHP_METHOD(Gd_Tiff_Info, __construct)
{
	zend_long width, height, page_count, bits_per_sample, samples_per_pixel;
	zend_long compression_tag, photometric_tag;
	zval *compression = NULL, *photometric = NULL, *resolution_unit = NULL, *metadata = NULL;
	bool min_is_white = false;
	double x_resolution = 0, y_resolution = 0;
	bool x_is_null = true, y_is_null = true;

	ZEND_PARSE_PARAMETERS_START(14, 14)
		Z_PARAM_LONG(width) Z_PARAM_LONG(height) Z_PARAM_LONG(page_count)
		Z_PARAM_LONG(bits_per_sample) Z_PARAM_LONG(samples_per_pixel)
		Z_PARAM_LONG(compression_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(compression, php_gd_tiff_compression_ce)
		Z_PARAM_LONG(photometric_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(photometric, php_gd_tiff_photometric_ce)
		Z_PARAM_BOOL(min_is_white)
		Z_PARAM_DOUBLE_OR_NULL(x_resolution, x_is_null)
		Z_PARAM_DOUBLE_OR_NULL(y_resolution, y_is_null)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(resolution_unit, php_gd_tiff_resolution_unit_ce)
		Z_PARAM_OBJECT_OF_CLASS(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("pageCount"), page_count);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("bitsPerSample"), bits_per_sample);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("samplesPerPixel"), samples_per_pixel);
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compressionTag"), compression_tag);
	if (compression) zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"), compression); else zend_update_property_null(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"));
	zend_update_property_long(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("photometricTag"), photometric_tag);
	if (photometric) zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("photometric"), photometric); else zend_update_property_null(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("photometric"));
	zend_update_property_bool(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("minIsWhite"), min_is_white);
	if (x_is_null) zend_update_property_null(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xResolution")); else zend_update_property_double(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xResolution"), x_resolution);
	if (y_is_null) zend_update_property_null(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yResolution")); else zend_update_property_double(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yResolution"), y_resolution);
	if (resolution_unit) zend_update_property(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionUnit"), resolution_unit); else zend_update_property_null(php_gd_tiff_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionUnit"));
}

PHP_METHOD(Gd_Tiff_Page, __construct)
{
	zval *image, *compression = NULL, *photometric = NULL, *planar_configuration = NULL, *resolution_unit = NULL;
	zend_long page_index, width, height, bits_per_sample, samples_per_pixel;
	zend_long compression_tag, photometric_tag, planar_configuration_tag;
	bool min_is_white = false, has_alpha = false, is_tiled = false, x_is_null = true, y_is_null = true;
	double x_resolution = 0, y_resolution = 0;

	ZEND_PARSE_PARAMETERS_START(18, 18)
		Z_PARAM_OBJECT_OF_CLASS(image, gd_image_ce)
		Z_PARAM_LONG(page_index) Z_PARAM_LONG(width) Z_PARAM_LONG(height)
		Z_PARAM_LONG(bits_per_sample) Z_PARAM_LONG(samples_per_pixel)
		Z_PARAM_LONG(compression_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(compression, php_gd_tiff_compression_ce)
		Z_PARAM_LONG(photometric_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(photometric, php_gd_tiff_photometric_ce)
		Z_PARAM_LONG(planar_configuration_tag)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(planar_configuration, php_gd_tiff_planar_configuration_ce)
		Z_PARAM_BOOL(min_is_white) Z_PARAM_BOOL(has_alpha) Z_PARAM_BOOL(is_tiled)
		Z_PARAM_DOUBLE_OR_NULL(x_resolution, x_is_null)
		Z_PARAM_DOUBLE_OR_NULL(y_resolution, y_is_null)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(resolution_unit, php_gd_tiff_resolution_unit_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("image"), image);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("pageIndex"), page_index);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("bitsPerSample"), bits_per_sample);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("samplesPerPixel"), samples_per_pixel);
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compressionTag"), compression_tag);
	if (compression) zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"), compression); else zend_update_property_null(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"));
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("photometricTag"), photometric_tag);
	if (photometric) zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("photometric"), photometric); else zend_update_property_null(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("photometric"));
	zend_update_property_long(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("planarConfigurationTag"), planar_configuration_tag);
	if (planar_configuration) zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("planarConfiguration"), planar_configuration); else zend_update_property_null(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("planarConfiguration"));
	zend_update_property_bool(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("minIsWhite"), min_is_white);
	zend_update_property_bool(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("hasAlpha"), has_alpha);
	zend_update_property_bool(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("isTiled"), is_tiled);
	if (x_is_null) zend_update_property_null(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xResolution")); else zend_update_property_double(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xResolution"), x_resolution);
	if (y_is_null) zend_update_property_null(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yResolution")); else zend_update_property_double(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yResolution"), y_resolution);
	if (resolution_unit) zend_update_property(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionUnit"), resolution_unit); else zend_update_property_null(php_gd_tiff_page_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionUnit"));
}

PHP_METHOD(Gd_Tiff_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Tiff_Reader, fromString)
{
	zend_string *bytes;
	gdTiffReadPtr tiff;

	ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_STR(bytes) ZEND_PARSE_PARAMETERS_END();
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		RETURN_THROWS();
	}
	tiff = gdTiffReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), NULL);
	if (!php_gd_tiff_initialize_reader(return_value, tiff)) RETURN_THROWS();
}

static zend_string *php_gd_tiff_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

PHP_METHOD(Gd_Tiff_Reader, fromFile)
{
	char *path;
	size_t path_len;
	php_stream *stream;
	zend_string *bytes;
	gdTiffReadPtr tiff;

	ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_PATH(path, path_len) ZEND_PARSE_PARAMETERS_END();
	stream = php_stream_open_wrapper(path, "rb", REPORT_ERRORS, NULL);
	if (stream == NULL) {
		php_gd_tiff_throw("Failed to open TIFF input");
		RETURN_THROWS();
	}
	bytes = php_gd_tiff_read_stream(stream);
	php_stream_close(stream);
	if (bytes == NULL || ZSTR_LEN(bytes) > INT_MAX) {
		if (bytes) zend_string_release(bytes);
		php_gd_tiff_throw("Failed to read TIFF input");
		RETURN_THROWS();
	}
	tiff = gdTiffReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), NULL);
	zend_string_release(bytes);
	if (!php_gd_tiff_initialize_reader(return_value, tiff)) RETURN_THROWS();
}

PHP_METHOD(Gd_Tiff_Reader, fromStream)
{
	zval *stream_zv;
	php_stream *stream;
	zend_string *bytes;
	gdTiffReadPtr tiff;

	ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_RESOURCE(stream_zv) ZEND_PARSE_PARAMETERS_END();
	php_stream_from_zval_no_verify(stream, stream_zv);
	if (stream == NULL) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}
	bytes = php_gd_tiff_read_stream(stream);
	if (bytes == NULL || ZSTR_LEN(bytes) > INT_MAX) {
		if (bytes) zend_string_release(bytes);
		php_gd_tiff_throw("Failed to read TIFF input");
		RETURN_THROWS();
	}
	tiff = gdTiffReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), NULL);
	zend_string_release(bytes);
	if (!php_gd_tiff_initialize_reader(return_value, tiff)) RETURN_THROWS();
}

PHP_METHOD(Gd_Tiff_Reader, info)
{
	php_gd_tiff_reader_object *reader = Z_GD_TIFF_READER_P(ZEND_THIS);
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&reader->info);
}

PHP_METHOD(Gd_Tiff_Reader, next)
{
	php_gd_tiff_reader_object *reader = Z_GD_TIFF_READER_P(ZEND_THIS);
	gdTiffPageInfo info;
	gdImagePtr image = NULL;
	int status;

	ZEND_PARSE_PARAMETERS_NONE();
	if (reader->failed) {
		php_gd_tiff_throw("TIFF reader is in a failed state");
		RETURN_THROWS();
	}
	if (reader->exhausted) RETURN_NULL();
	status = gdTiffReadNextImage(reader->reader, &info, &image);
	if (status == 0) {
		reader->exhausted = true;
		RETURN_NULL();
	}
	if (status < 0 || image == NULL) {
		reader->failed = true;
		if (image != NULL) gdImageDestroy(image);
		php_gd_tiff_throw("Failed to decode TIFF page");
		RETURN_THROWS();
	}
	php_gd_tiff_create_page(return_value, image, &info);
}
#endif

#ifdef HAVE_GD_TIFF_WRITE_API
static zend_class_entry *php_gd_tiff_write_options_ce;
static zend_class_entry *php_gd_tiff_writer_ce;
static zend_object_handlers php_gd_tiff_writer_handlers;

typedef enum {
	PHP_GD_TIFF_DESTINATION_CONTEXT,
	PHP_GD_TIFF_DESTINATION_MEMORY
} php_gd_tiff_destination;

typedef struct {
	gdTiffWritePtr writer;
	gdIOCtx *ctx;
	php_gd_tiff_destination destination;
	int compression;
	int color_space;
	int min_is_white;
	int resolution_unit;
	double x_resolution;
	double y_resolution;
	bool has_x_resolution;
	bool has_y_resolution;
	zval metadata;
	zend_long page_count;
	bool finalized;
	zend_object std;
} php_gd_tiff_writer_object;

static php_gd_tiff_writer_object *php_gd_tiff_writer_from_object(zend_object *object)
{
	return (php_gd_tiff_writer_object *) ((char *) object - offsetof(php_gd_tiff_writer_object, std));
}

#define Z_GD_TIFF_WRITER_P(zv) php_gd_tiff_writer_from_object(Z_OBJ_P((zv)))

static int php_gd_tiff_compression(zval *value)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Tiff_Compression_None:
			return GD_TIFF_COMPRESSION_NONE;
		case ZEND_ENUM_Gd_Tiff_Compression_CcittRle:
			return GD_TIFF_COMPRESSION_CCITT_RLE;
		case ZEND_ENUM_Gd_Tiff_Compression_CcittFax3:
			return GD_TIFF_COMPRESSION_CCITT_FAX3;
		case ZEND_ENUM_Gd_Tiff_Compression_CcittFax4:
			return GD_TIFF_COMPRESSION_CCITT_FAX4;
		case ZEND_ENUM_Gd_Tiff_Compression_Lzw:
			return GD_TIFF_COMPRESSION_LZW;
		case ZEND_ENUM_Gd_Tiff_Compression_Jpeg:
			return GD_TIFF_COMPRESSION_JPEG;
		case ZEND_ENUM_Gd_Tiff_Compression_AdobeDeflate:
			return GD_TIFF_COMPRESSION_ADOBE_DEFLATE;
		case ZEND_ENUM_Gd_Tiff_Compression_Deflate:
			return GD_TIFF_COMPRESSION_DEFLATE;
		case ZEND_ENUM_Gd_Tiff_Compression_PackBits:
			return GD_TIFF_COMPRESSION_PACKBITS;
		default:
			return -1;
	}
}

static int php_gd_tiff_color_space(zval *value)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Tiff_ColorSpace_Rgb:
			return GD_TIFF_RGB;
		case ZEND_ENUM_Gd_Tiff_ColorSpace_Gray:
			return GD_TIFF_GRAY;
		case ZEND_ENUM_Gd_Tiff_ColorSpace_Rgba:
		default:
			return GD_TIFF_RGBA;
	}
}

static int php_gd_tiff_resolution_unit(zval *value)
{
	switch (zend_enum_fetch_case_id(Z_OBJ_P(value))) {
		case ZEND_ENUM_Gd_Tiff_ResolutionUnit_None:
			return GD_TIFF_RESUNIT_NONE;
		case ZEND_ENUM_Gd_Tiff_ResolutionUnit_Centimeter:
			return GD_TIFF_RESUNIT_CENTIMETER;
		case ZEND_ENUM_Gd_Tiff_ResolutionUnit_Inch:
		default:
			return GD_TIFF_RESUNIT_INCH;
	}
}

static void php_gd_tiff_writer_close(php_gd_tiff_writer_object *writer)
{
	if (writer->writer != NULL) {
		gdTiffWriteClose(writer->writer);
		writer->writer = NULL;
	}
	if (writer->ctx != NULL) {
		writer->ctx->gd_free(writer->ctx);
		writer->ctx = NULL;
	}
}

static zend_object *php_gd_tiff_writer_create(zend_class_entry *class_entry)
{
	php_gd_tiff_writer_object *writer = zend_object_alloc(sizeof(*writer), class_entry);

	writer->writer = NULL;
	writer->ctx = NULL;
	writer->destination = PHP_GD_TIFF_DESTINATION_CONTEXT;
	writer->compression = GD_TIFF_COMPRESSION_DEFLATE;
	writer->color_space = GD_TIFF_RGBA;
	writer->min_is_white = 0;
	writer->resolution_unit = GD_TIFF_RESUNIT_INCH;
	writer->x_resolution = 0;
	writer->y_resolution = 0;
	writer->has_x_resolution = false;
	writer->has_y_resolution = false;
	ZVAL_UNDEF(&writer->metadata);
	writer->page_count = 0;
	writer->finalized = false;

	zend_object_std_init(&writer->std, class_entry);
	object_properties_init(&writer->std, class_entry);
	writer->std.handlers = &php_gd_tiff_writer_handlers;

	return &writer->std;
}

static void php_gd_tiff_writer_free(zend_object *object)
{
	php_gd_tiff_writer_object *writer = php_gd_tiff_writer_from_object(object);

	php_gd_tiff_writer_close(writer);
	if (!Z_ISUNDEF(writer->metadata)) {
		zval_ptr_dtor(&writer->metadata);
	}
	zend_object_std_dtor(&writer->std);
}

static void php_gd_tiff_configure_writer(php_gd_tiff_writer_object *writer, zval *options_zv)
{
	zval rv;
	zval *value;

	if (options_zv == NULL) {
		return;
	}

	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("compression"), true, &rv);
	writer->compression = php_gd_tiff_compression(value);
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("colorSpace"), true, &rv);
	writer->color_space = php_gd_tiff_color_space(value);
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("minIsWhite"), true, &rv);
	writer->min_is_white = Z_TYPE_P(value) == IS_TRUE;
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("resolutionUnit"), true, &rv);
	writer->resolution_unit = php_gd_tiff_resolution_unit(value);
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("xResolution"), true, &rv);
	if (Z_TYPE_P(value) != IS_NULL) {
		writer->x_resolution = Z_DVAL_P(value);
		writer->has_x_resolution = true;
	}
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("yResolution"), true, &rv);
	if (Z_TYPE_P(value) != IS_NULL) {
		writer->y_resolution = Z_DVAL_P(value);
		writer->has_y_resolution = true;
	}
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
	if (Z_TYPE_P(value) == IS_OBJECT) {
		ZVAL_COPY(&writer->metadata, value);
	}
}

static void php_gd_tiff_create_writer(zval *return_value, gdIOCtx *ctx,
		php_gd_tiff_destination destination, zval *options_zv)
{
	php_gd_tiff_writer_object *writer;

	object_init_ex(return_value, php_gd_tiff_writer_ce);
	writer = Z_GD_TIFF_WRITER_P(return_value);
	writer->ctx = ctx;
	writer->destination = destination;
	php_gd_tiff_configure_writer(writer, options_zv);
}

PHP_METHOD(Gd_Tiff_WriteOptions, __construct)
{
	zval *compression = NULL;
	zval *color_space = NULL;
	bool min_is_white = false;
	zval *resolution_unit = NULL;
	double x_resolution = 0;
	double y_resolution = 0;
	bool x_resolution_is_null = true;
	bool y_resolution_is_null = true;
	zval default_value;
	zval *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 7)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(compression, php_gd_tiff_compression_ce)
		Z_PARAM_OBJECT_OF_CLASS(color_space, php_gd_tiff_color_space_ce)
		Z_PARAM_BOOL(min_is_white)
		Z_PARAM_OBJECT_OF_CLASS(resolution_unit, php_gd_tiff_resolution_unit_ce)
		Z_PARAM_DOUBLE_OR_NULL(x_resolution, x_resolution_is_null)
		Z_PARAM_DOUBLE_OR_NULL(y_resolution, y_resolution_is_null)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!x_resolution_is_null && (x_resolution <= 0 || !zend_finite(x_resolution))) {
		zend_argument_value_error(5, "must be finite and greater than 0");
		RETURN_THROWS();
	}
	if (!y_resolution_is_null && (y_resolution <= 0 || !zend_finite(y_resolution))) {
		zend_argument_value_error(6, "must be finite and greater than 0");
		RETURN_THROWS();
	}

	if (compression == NULL) {
		ZVAL_OBJ(&default_value, zend_enum_get_case_by_id(php_gd_tiff_compression_ce, ZEND_ENUM_Gd_Tiff_Compression_Deflate));
		compression = &default_value;
	}
	zend_update_property(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("compression"), compression);
	if (color_space == NULL) {
		ZVAL_OBJ(&default_value, zend_enum_get_case_by_id(php_gd_tiff_color_space_ce, ZEND_ENUM_Gd_Tiff_ColorSpace_Rgba));
		color_space = &default_value;
	}
	zend_update_property(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorSpace"), color_space);
	zend_update_property_bool(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("minIsWhite"), min_is_white);
	if (resolution_unit == NULL) {
		ZVAL_OBJ(&default_value, zend_enum_get_case_by_id(php_gd_tiff_resolution_unit_ce, ZEND_ENUM_Gd_Tiff_ResolutionUnit_Inch));
		resolution_unit = &default_value;
	}
	zend_update_property(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("resolutionUnit"), resolution_unit);
	if (x_resolution_is_null) {
		zend_update_property_null(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xResolution"));
	} else {
		zend_update_property_double(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("xResolution"), x_resolution);
	}
	if (y_resolution_is_null) {
		zend_update_property_null(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yResolution"));
	} else {
		zend_update_property_double(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("yResolution"), y_resolution);
	}
	if (metadata) zend_update_property(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata); else zend_update_property_null(php_gd_tiff_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
}

PHP_METHOD(Gd_Tiff_Writer, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Tiff_Writer, toFile)
{
	zval *path_zv;
	zval *options_zv = NULL;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(path_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_tiff_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	ZEND_ASSERT(Z_TYPE_P(path_zv) == IS_STRING);
	ctx = php_gd_create_output_context(path_zv, 1);
	if (ctx == NULL) {
		php_gd_tiff_throw("Failed to open TIFF output");
		RETURN_THROWS();
	}
	php_gd_tiff_create_writer(return_value, ctx, PHP_GD_TIFF_DESTINATION_CONTEXT, options_zv);
}

PHP_METHOD(Gd_Tiff_Writer, toStream)
{
	zval *stream_zv;
	zval *options_zv = NULL;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_tiff_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ctx = php_gd_create_output_context(stream_zv, 1);
	if (ctx == NULL) {
		php_gd_tiff_throw("Failed to open TIFF output");
		RETURN_THROWS();
	}
	php_gd_tiff_create_writer(return_value, ctx, PHP_GD_TIFF_DESTINATION_CONTEXT, options_zv);
}

PHP_METHOD(Gd_Tiff_Writer, toMemory)
{
	zval *options_zv = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_tiff_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	php_gd_tiff_create_writer(return_value, NULL, PHP_GD_TIFF_DESTINATION_MEMORY, options_zv);
}

PHP_METHOD(Gd_Tiff_Writer, addPage)
{
	zval *image_zv;
	php_gd_tiff_writer_object *writer = Z_GD_TIFF_WRITER_P(ZEND_THIS);
	gdImagePtr image;
		gdTiffWriteOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (writer->finalized) {
		php_gd_tiff_throw("TIFF writer has already been finalized");
		RETURN_THROWS();
	}

	image = php_gd_libgdimageptr_from_zval_p(image_zv);
	if (!image->trueColor) {
		php_gd_tiff_throw("TIFF writer only supports truecolor images");
		RETURN_THROWS();
	}

	if (writer->writer == NULL) {
		gdTiffWriteOptionsInit(&options);
		options.colorspace = writer->color_space;
		options.compression = writer->compression;
		options.minIsWhite = writer->min_is_white;
		options.resolutionUnit = writer->resolution_unit;
		options.xResolution = writer->has_x_resolution ? (float) writer->x_resolution : (float) image->res_x;
		options.yResolution = writer->has_y_resolution ? (float) writer->y_resolution : (float) image->res_y;
		options.metadata = Z_ISUNDEF(writer->metadata) ? NULL : php_gd_metadata_from_zval(&writer->metadata);

		if (writer->destination == PHP_GD_TIFF_DESTINATION_MEMORY) {
			writer->writer = gdTiffWriteOpenPtr(&options);
		} else {
			writer->writer = gdTiffWriteOpenCtx(writer->ctx, &options);
		}
		if (writer->writer == NULL) {
			php_gd_tiff_throw("Failed to initialize TIFF writer");
			RETURN_THROWS();
		}
	}

	if (!gdTiffWriteAddImage(writer->writer, image)) {
		writer->finalized = true;
		php_gd_tiff_writer_close(writer);
		php_gd_tiff_throw("Failed to write TIFF page");
		RETURN_THROWS();
	}

	writer->page_count++;
	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

PHP_METHOD(Gd_Tiff_Writer, finish)
{
	php_gd_tiff_writer_object *writer = Z_GD_TIFF_WRITER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();

	if (writer->finalized) {
		php_gd_tiff_throw("TIFF writer has already been finalized");
		RETURN_THROWS();
	}
	if (writer->page_count == 0) {
		php_gd_tiff_throw("Cannot finalize a TIFF writer without pages");
		RETURN_THROWS();
	}

	writer->finalized = true;
	if (writer->destination == PHP_GD_TIFF_DESTINATION_MEMORY) {
		int size = 0;
		void *data = gdTiffWritePtrFinish(writer->writer, &size);
		writer->writer = NULL;
		if (data == NULL || size < 0) {
			gdFree(data);
			php_gd_tiff_throw("Failed to finalize TIFF output");
			RETURN_THROWS();
		}
		RETVAL_STRINGL(data, size);
		gdFree(data);
		return;
	}

	php_gd_tiff_writer_close(writer);
	RETURN_NULL();
}

#ifdef HAVE_GD_TIFF_READ_API
static void php_gd_tiff_build_single_write_options(gdImagePtr image, zval *options_zv, gdTiffWriteOptions *options)
{
	zval rv;
	zval *value;

	gdTiffWriteOptionsInit(options);
	options->compression = GD_TIFF_COMPRESSION_DEFLATE;
	options->xResolution = (float) image->res_x;
	options->yResolution = (float) image->res_y;

	if (options_zv == NULL) {
		return;
	}

	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("compression"), true, &rv);
	options->compression = php_gd_tiff_compression(value);
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("colorSpace"), true, &rv);
	options->colorspace = php_gd_tiff_color_space(value);
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("minIsWhite"), true, &rv);
	options->minIsWhite = Z_TYPE_P(value) == IS_TRUE;
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("resolutionUnit"), true, &rv);
	options->resolutionUnit = php_gd_tiff_resolution_unit(value);
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("xResolution"), true, &rv);
	if (Z_TYPE_P(value) != IS_NULL) {
		options->xResolution = (float) Z_DVAL_P(value);
	}
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("yResolution"), true, &rv);
	if (Z_TYPE_P(value) != IS_NULL) {
		options->yResolution = (float) Z_DVAL_P(value);
	}
	value = zend_read_property(php_gd_tiff_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("metadata"), true, &rv);
	if (Z_TYPE_P(value) == IS_OBJECT) {
		options->metadata = php_gd_metadata_from_zval(value);
	}
}

static gdImagePtr php_gd_tiff_decode_single_page(gdTiffReadPtr tiff)
{
	gdTiffInfo info;
	gdTiffPageInfo page_info;
	gdImagePtr image = NULL;
	int status;

	if (tiff == NULL || !gdTiffReadGetInfo(tiff, &info)) {
		if (tiff != NULL) {
			gdTiffReadClose(tiff);
		}
		php_gd_tiff_throw("Failed to open TIFF input");
		return NULL;
	}
	if (info.pageCount != 1) {
		gdTiffReadClose(tiff);
		php_gd_tiff_throw("TIFF input contains multiple pages; use Gd\\Tiff\\Reader");
		return NULL;
	}
	status = gdTiffReadNextImage(tiff, &page_info, &image);
	gdTiffReadClose(tiff);
	if (status != 1 || image == NULL) {
		if (image != NULL) {
			gdImageDestroy(image);
		}
		php_gd_tiff_throw("Failed to decode TIFF page");
		return NULL;
	}
	return image;
}

PHP_METHOD(Gd_Tiff_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Tiff_Codec, fromString)
{
	zend_string *bytes;
	gdTiffReadPtr tiff;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_STR(bytes) ZEND_PARSE_PARAMETERS_END();
	if (ZSTR_LEN(bytes) > INT_MAX) {
		zend_argument_value_error(1, "must not exceed %d bytes", INT_MAX);
		RETURN_THROWS();
	}
	tiff = gdTiffReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), NULL);
	image = php_gd_tiff_decode_single_page(tiff);
	if (image == NULL) {
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Tiff_Codec, fromFile)
{
	char *path;
	size_t path_len;
	php_stream *stream;
	zend_string *bytes;
	gdTiffReadPtr tiff;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_PATH(path, path_len) ZEND_PARSE_PARAMETERS_END();
	stream = php_stream_open_wrapper(path, "rb", REPORT_ERRORS, NULL);
	if (stream == NULL) {
		php_gd_tiff_throw("Failed to open TIFF input");
		RETURN_THROWS();
	}
	bytes = php_gd_tiff_read_stream(stream);
	php_stream_close(stream);
	if (bytes == NULL || ZSTR_LEN(bytes) > INT_MAX) {
		if (bytes) zend_string_release(bytes);
		php_gd_tiff_throw("Failed to read TIFF input");
		RETURN_THROWS();
	}
	tiff = gdTiffReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), NULL);
	zend_string_release(bytes);
	image = php_gd_tiff_decode_single_page(tiff);
	if (image == NULL) {
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

PHP_METHOD(Gd_Tiff_Codec, fromStream)
{
	zval *stream_zv;
	php_stream *stream;
	zend_string *bytes;
	gdTiffReadPtr tiff;
	gdImagePtr image;

	ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_RESOURCE(stream_zv) ZEND_PARSE_PARAMETERS_END();
	php_stream_from_zval_no_verify(stream, stream_zv);
	if (stream == NULL) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}
	bytes = php_gd_tiff_read_stream(stream);
	if (bytes == NULL || ZSTR_LEN(bytes) > INT_MAX) {
		if (bytes) zend_string_release(bytes);
		php_gd_tiff_throw("Failed to read TIFF input");
		RETURN_THROWS();
	}
	tiff = gdTiffReadOpenPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), NULL);
	zend_string_release(bytes);
	image = php_gd_tiff_decode_single_page(tiff);
	if (image == NULL) {
		RETURN_THROWS();
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
}

static void php_gd_tiff_codec_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv, *destination, *options_zv = NULL;
	gdImagePtr image;
	gdTiffWriteOptions options;
	gdIOCtx *ctx;
	gdTiffWritePtr writer;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_tiff_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}

	image = php_gd_libgdimageptr_from_zval_p(image_zv);
	if (!image->trueColor) {
		php_gd_tiff_throw("TIFF writer only supports truecolor images");
		RETURN_THROWS();
	}
	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		php_gd_tiff_throw("Failed to open TIFF output");
		RETURN_THROWS();
	}
	php_gd_tiff_build_single_write_options(image, options_zv, &options);
	writer = gdTiffWriteOpenCtx(ctx, &options);
	if (writer == NULL) {
		ctx->gd_free(ctx);
		php_gd_tiff_throw("Failed to initialize TIFF writer");
		RETURN_THROWS();
	}
	if (!gdTiffWriteAddImage(writer, image)) {
		gdTiffWriteClose(writer);
		ctx->gd_free(ctx);
		php_gd_tiff_throw("Failed to write TIFF page");
		RETURN_THROWS();
	}
	gdTiffWriteClose(writer);
	ctx->gd_free(ctx);
}

PHP_METHOD(Gd_Tiff_Codec, toFile)
{
	php_gd_tiff_codec_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Tiff_Codec, toStream)
{
	php_gd_tiff_codec_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Tiff_Codec, toString)
{
	zval *image_zv, *options_zv = NULL;
	gdImagePtr image;
	gdTiffWriteOptions options;
	gdTiffWritePtr writer;
	int size = 0;
	void *data;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_tiff_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	image = php_gd_libgdimageptr_from_zval_p(image_zv);
	if (!image->trueColor) {
		php_gd_tiff_throw("TIFF writer only supports truecolor images");
		RETURN_THROWS();
	}
	php_gd_tiff_build_single_write_options(image, options_zv, &options);
	writer = gdTiffWriteOpenPtr(&options);
	if (writer == NULL) {
		php_gd_tiff_throw("Failed to initialize TIFF writer");
		RETURN_THROWS();
	}
	if (!gdTiffWriteAddImage(writer, image)) {
		gdTiffWriteClose(writer);
		php_gd_tiff_throw("Failed to write TIFF page");
		RETURN_THROWS();
	}
	data = gdTiffWritePtrFinish(writer, &size);
	if (data == NULL || size < 0) {
		gdFree(data);
		php_gd_tiff_throw("Failed to finalize TIFF output");
		RETURN_THROWS();
	}
	RETVAL_STRINGL(data, size);
	gdFree(data);
}
#endif
#endif

void php_gd_tiff_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
	php_gd_tiff_color_space_ce = register_class_Gd_Tiff_ColorSpace();
	php_gd_tiff_compression_ce = register_class_Gd_Tiff_Compression();
	php_gd_tiff_resolution_unit_ce = register_class_Gd_Tiff_ResolutionUnit();
#endif

#ifdef HAVE_GD_TIFF_READ_API
	php_gd_tiff_photometric_ce = register_class_Gd_Tiff_Photometric();
	php_gd_tiff_planar_configuration_ce = register_class_Gd_Tiff_PlanarConfiguration();
	php_gd_tiff_info_ce = register_class_Gd_Tiff_Info();
	php_gd_tiff_page_ce = register_class_Gd_Tiff_Page();
	php_gd_tiff_reader_ce = register_class_Gd_Tiff_Reader();
	php_gd_tiff_reader_ce->create_object = php_gd_tiff_reader_create;

	memcpy(&php_gd_tiff_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_tiff_reader_handlers.offset = offsetof(php_gd_tiff_reader_object, std);
	php_gd_tiff_reader_handlers.free_obj = php_gd_tiff_reader_free;
	php_gd_tiff_reader_handlers.clone_obj = NULL;
#endif

#ifdef HAVE_GD_TIFF_WRITE_API
	php_gd_tiff_write_options_ce = register_class_Gd_Tiff_WriteOptions(php_gd_get_codec_write_options_ce());
	php_gd_tiff_writer_ce = register_class_Gd_Tiff_Writer();
	php_gd_tiff_writer_ce->create_object = php_gd_tiff_writer_create;

	memcpy(&php_gd_tiff_writer_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_tiff_writer_handlers.offset = offsetof(php_gd_tiff_writer_object, std);
	php_gd_tiff_writer_handlers.free_obj = php_gd_tiff_writer_free;
	php_gd_tiff_writer_handlers.clone_obj = NULL;
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) && defined(HAVE_GD_TIFF_READ_API)
	zend_class_entry *codec_ce;

	codec_ce = register_class_Gd_Tiff_Codec();
	php_gd_register_codec_write(php_gd_tiff_write_options_ce, codec_ce);
	php_gd_register_codec_format("Tiff", php_gd_tiff_write_options_ce);
	php_gd_register_codec_extension("tif", php_gd_tiff_write_options_ce);
	php_gd_register_codec_extension("tiff", php_gd_tiff_write_options_ce);
#endif
}
