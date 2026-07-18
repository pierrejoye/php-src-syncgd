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
#include "gd_metadata.h"
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
#ifdef HAVE_GD_BUNDLED
static zend_class_entry *php_gd_bmp_info_ce;
static zend_class_entry *php_gd_bmp_reader_ce;
static zend_object_handlers php_gd_bmp_reader_handlers;

typedef struct {
	zend_string *bytes;
	zval info;
	bool read;
	bool failed;
	zend_object std;
} php_gd_bmp_reader_object;

static php_gd_bmp_reader_object *php_gd_bmp_reader_from_object(zend_object *object)
{
	return (php_gd_bmp_reader_object *) ((char *) object - offsetof(php_gd_bmp_reader_object, std));
}

#define Z_GD_BMP_READER_P(zv) php_gd_bmp_reader_from_object(Z_OBJ_P((zv)))
#endif

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

#ifdef HAVE_GD_BUNDLED
static bool php_gd_bmp_create_info(zval *result, zend_string *bytes)
{
	gdBmpInfo info;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		php_gd_bmp_throw("BMP input is too large");
		return false;
	}
	gdBmpInfoInit(&info);
	if (!gdBmpGetInfoPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &info)) {
		php_gd_bmp_throw("Failed to read BMP info");
		return false;
	}
	object_init_ex(result, php_gd_bmp_info_ce);
#define BMP_INFO_LONG(name, value) zend_update_property_long(php_gd_bmp_info_ce, Z_OBJ_P(result), ZEND_STRL(name), (zend_long) (value))
	BMP_INFO_LONG("fileSize", info.file_size);
	BMP_INFO_LONG("pixelOffset", info.pixel_offset);
	BMP_INFO_LONG("headerSize", info.header_size);
	BMP_INFO_LONG("headerType", info.header_type);
	BMP_INFO_LONG("width", info.width);
	BMP_INFO_LONG("height", info.height);
	BMP_INFO_LONG("planes", info.planes);
	BMP_INFO_LONG("bitsPerPixel", info.bits_per_pixel);
	BMP_INFO_LONG("compressionTag", info.compression);
	BMP_INFO_LONG("imageSize", info.image_size);
	BMP_INFO_LONG("horizontalResolution", info.horizontal_resolution);
	BMP_INFO_LONG("verticalResolution", info.vertical_resolution);
	BMP_INFO_LONG("colorsUsed", info.colors_used);
	BMP_INFO_LONG("importantColors", info.important_colors);
	BMP_INFO_LONG("paletteType", info.palette_type);
	BMP_INFO_LONG("paletteEntries", info.palette_entries);
	BMP_INFO_LONG("redMask", info.red_mask);
	BMP_INFO_LONG("greenMask", info.green_mask);
	BMP_INFO_LONG("blueMask", info.blue_mask);
	BMP_INFO_LONG("alphaMask", info.alpha_mask);
	zend_update_property_bool(php_gd_bmp_info_ce, Z_OBJ_P(result), ZEND_STRL("topDown"), info.top_down);
#undef BMP_INFO_LONG
	return true;
}

static zend_object *php_gd_bmp_reader_create(zend_class_entry *class_entry)
{
	php_gd_bmp_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);
	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_bmp_reader_handlers;
	return &reader->std;
}

static void php_gd_bmp_reader_free(zend_object *object)
{
	php_gd_bmp_reader_object *reader = php_gd_bmp_reader_from_object(object);
	if (reader->bytes != NULL) zend_string_release(reader->bytes);
	if (!Z_ISUNDEF(reader->info)) zval_ptr_dtor(&reader->info);
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_bmp_initialize_reader(zval *result, zend_string *bytes)
{
	php_gd_bmp_reader_object *reader;
	object_init_ex(result, php_gd_bmp_reader_ce);
	reader = Z_GD_BMP_READER_P(result);
	reader->bytes = zend_string_copy(bytes);
	if (!php_gd_bmp_create_info(&reader->info, bytes)) {
		zval_ptr_dtor(result);
		return false;
	}
	return true;
}

static bool php_gd_bmp_reader_read_image(php_gd_bmp_reader_object *reader, zval *return_value)
{
	gdImagePtr image;
	if (reader->failed) {
		php_gd_bmp_throw("BMP reader is in a failed state");
		return false;
	}
	if (reader->read) {
		php_gd_bmp_throw("BMP image has already been read");
		return false;
	}
	image = php_gd_bmp_decode_bytes(reader->bytes);
	reader->read = true;
	if (image == NULL) {
		reader->failed = true;
		php_gd_bmp_throw("Failed to decode BMP image");
		return false;
	}
	php_gd_assign_libgdimageptr_as_extgdimage(return_value, image);
	return true;
}

static bool php_gd_bmp_decode_reader_bytes(zend_string *bytes, zval *return_value)
{
	zval reader_zv;
	bool result;
	if (!php_gd_bmp_initialize_reader(&reader_zv, bytes)) {
		return false;
	}
	result = php_gd_bmp_reader_read_image(Z_GD_BMP_READER_P(&reader_zv), return_value);
	zval_ptr_dtor(&reader_zv);
	return result;
}
#endif

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

#if defined(HAVE_GD_BUNDLED)
	gdBmpWriteOptions native_options;
	gdBmpWriteOptionsInit(&native_options);

	switch (options.compression) {
		case PHP_GD_BMP_COMPRESSION_AUTO:
			native_options.compression = -1;
			break;
		case PHP_GD_BMP_COMPRESSION_NONE:
			native_options.compression = GD_BMP_COMPRESS_NONE;
			break;
		case PHP_GD_BMP_COMPRESSION_RLE8:
			native_options.compression = GD_BMP_COMPRESS_RLE8;
			break;
		case PHP_GD_BMP_COMPRESSION_RLE4:
			native_options.compression = GD_BMP_COMPRESS_RLE4;
			break;
		default:
			php_gd_bmp_throw("Unsupported BMP compression option");
			return false;
	}

	native_options.bits_per_pixel = options.bits_per_pixel;
	if (options.quantize) {
		native_options.flags |= GD_BMP_FLAG_QUANTIZE;
	}
	if (options.force_v4_header) {
		native_options.flags |= GD_BMP_FLAG_FORCE_V4HDR;
	}
	if (options.rgb555) {
		native_options.flags |= GD_BMP_FLAG_RGB555;
	}
	data = gdImageBmpPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &native_options);
#elif defined(HAVE_GD_BMP_EXTENDED)
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
	if (options.quantize) flags |= GD_BMP_FLAG_QUANTIZE;
	if (options.force_v4_header) flags |= GD_BMP_FLAG_FORCE_V4HDR;
	if (options.rgb555) flags |= GD_BMP_FLAG_RGB555;
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
	zval *compression = NULL, *metadata = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 6)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(bits_per_pixel)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(compression, php_gd_bmp_compression_ce)
		Z_PARAM_BOOL(quantize)
		Z_PARAM_BOOL(force_v4_header)
		Z_PARAM_BOOL(rgb555)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
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
	if (metadata) zend_update_property(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata); else zend_update_property_null(php_gd_bmp_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
}

#ifdef HAVE_GD_BUNDLED
PHP_METHOD(Gd_Bmp_Info, __construct)
{
	zend_long file_size, pixel_offset, header_size, header_type, width, height;
	zend_bool top_down;
	zend_long planes, bits_per_pixel, compression_tag, image_size;
	zend_long horizontal_resolution, vertical_resolution, colors_used, important_colors;
	zend_long palette_type, palette_entries, red_mask, green_mask, blue_mask, alpha_mask;
	ZEND_PARSE_PARAMETERS_START(21, 21)
		Z_PARAM_LONG(file_size)
		Z_PARAM_LONG(pixel_offset)
		Z_PARAM_LONG(header_size)
		Z_PARAM_LONG(header_type)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_BOOL(top_down)
		Z_PARAM_LONG(planes)
		Z_PARAM_LONG(bits_per_pixel)
		Z_PARAM_LONG(compression_tag)
		Z_PARAM_LONG(image_size)
		Z_PARAM_LONG(horizontal_resolution)
		Z_PARAM_LONG(vertical_resolution)
		Z_PARAM_LONG(colors_used)
		Z_PARAM_LONG(important_colors)
		Z_PARAM_LONG(palette_type)
		Z_PARAM_LONG(palette_entries)
		Z_PARAM_LONG(red_mask)
		Z_PARAM_LONG(green_mask)
		Z_PARAM_LONG(blue_mask)
		Z_PARAM_LONG(alpha_mask)
	ZEND_PARSE_PARAMETERS_END();
#define BMP_INFO_SET(name, value) zend_update_property_long(php_gd_bmp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL(name), value)
	BMP_INFO_SET("fileSize", file_size); BMP_INFO_SET("pixelOffset", pixel_offset);
	BMP_INFO_SET("headerSize", header_size); BMP_INFO_SET("headerType", header_type);
	BMP_INFO_SET("width", width); BMP_INFO_SET("height", height);
	zend_update_property_bool(php_gd_bmp_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("topDown"), top_down);
	BMP_INFO_SET("planes", planes); BMP_INFO_SET("bitsPerPixel", bits_per_pixel);
	BMP_INFO_SET("compressionTag", compression_tag); BMP_INFO_SET("imageSize", image_size);
	BMP_INFO_SET("horizontalResolution", horizontal_resolution); BMP_INFO_SET("verticalResolution", vertical_resolution);
	BMP_INFO_SET("colorsUsed", colors_used); BMP_INFO_SET("importantColors", important_colors);
	BMP_INFO_SET("paletteType", palette_type); BMP_INFO_SET("paletteEntries", palette_entries);
	BMP_INFO_SET("redMask", red_mask); BMP_INFO_SET("greenMask", green_mask);
	BMP_INFO_SET("blueMask", blue_mask); BMP_INFO_SET("alphaMask", alpha_mask);
#undef BMP_INFO_SET
}

PHP_METHOD(Gd_Bmp_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Bmp_Reader, fromString)
{
	zend_string *bytes;
	zval *options = NULL;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_bmp_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_bmp_initialize_reader(return_value, bytes)) RETURN_THROWS();
}

PHP_METHOD(Gd_Bmp_Reader, fromFile)
{
	zend_string *path, *bytes;
	zval *options = NULL;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_bmp_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_bmp_read_file_bytes(path, &bytes)) RETURN_THROWS();
	if (!php_gd_bmp_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes); RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Bmp_Reader, fromStream)
{
	zval *stream_zv, *options = NULL;
	zend_string *bytes;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_bmp_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();
	if (!php_gd_bmp_read_stream_bytes(stream_zv, &bytes)) RETURN_THROWS();
	if (!php_gd_bmp_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes); RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Bmp_Reader, info)
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&Z_GD_BMP_READER_P(ZEND_THIS)->info);
}

PHP_METHOD(Gd_Bmp_Reader, read)
{
	ZEND_PARSE_PARAMETERS_NONE();
	if (!php_gd_bmp_reader_read_image(Z_GD_BMP_READER_P(ZEND_THIS), return_value)) RETURN_THROWS();
}
#endif

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

#ifdef HAVE_GD_BUNDLED
	if (!php_gd_bmp_decode_reader_bytes(bytes, return_value)) {
		RETURN_THROWS();
	}
	return;
#else
	image = php_gd_bmp_decode_bytes(bytes);
#endif
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
#ifdef HAVE_GD_BUNDLED
	if (!php_gd_bmp_decode_reader_bytes(bytes, return_value)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
	return;
#else
	image = php_gd_bmp_decode_bytes(bytes);
#endif
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
#ifdef HAVE_GD_BUNDLED
	if (!php_gd_bmp_decode_reader_bytes(bytes, return_value)) {
		zend_string_release_ex(bytes, 0);
		RETURN_THROWS();
	}
	zend_string_release_ex(bytes, 0);
	return;
#else
	image = php_gd_bmp_decode_bytes(bytes);
#endif
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
	#ifdef HAVE_GD_BUNDLED
	php_gd_bmp_info_ce = register_class_Gd_Bmp_Info();
	php_gd_bmp_reader_ce = register_class_Gd_Bmp_Reader();
	memcpy(&php_gd_bmp_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_bmp_reader_handlers.offset = offsetof(php_gd_bmp_reader_object, std);
	php_gd_bmp_reader_handlers.free_obj = php_gd_bmp_reader_free;
	php_gd_bmp_reader_ce->create_object = php_gd_bmp_reader_create;
	#endif
	codec_ce = register_class_Gd_Bmp_Codec();
	php_gd_register_codec_write(php_gd_bmp_write_options_ce, codec_ce);
	php_gd_register_codec_format("Bmp", php_gd_bmp_write_options_ce);
	php_gd_register_codec_extension("bmp", php_gd_bmp_write_options_ce);
}
