/* This is a generated file, edit gd_bmp.stub.php instead.
 * Stub hash: 81b8a484ca8975bb218883a8bec6cfe4eda7582c
 * Has decl header: yes */

#if defined(HAVE_GD_BUNDLED)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Bmp_Info___construct, 0, 0, 18)
	ZEND_ARG_TYPE_INFO(0, headerType, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, topDown, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, planes, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, bitsPerPixel, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, compressionTag, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, imageSize, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, horizontalResolution, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, verticalResolution, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, colorsUsed, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, importantColors, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, paletteType, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, paletteEntries, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, redMask, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, greenMask, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, blueMask, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, alphaMask, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Bmp_Reader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Reader_fromFile, 0, 1, Gd\\Bmp\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\ReadOptions, 0, "new Gd\\Bmp\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Reader_fromStream, 0, 1, Gd\\Bmp\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\ReadOptions, 0, "new Gd\\Bmp\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Reader_fromString, 0, 1, Gd\\Bmp\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\ReadOptions, 0, "new Gd\\Bmp\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Reader_info, 0, 0, Gd\\Bmp\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Bmp_ReadOptions___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Bmp_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, bitsPerPixel, IS_LONG, 0, "0")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, compression, Gd\\Bmp\\Compression, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quantize, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, forceV4Header, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, rgb555, _IS_BOOL, 0, "false")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Bmp_Codec___construct arginfo_class_Gd_Bmp_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\ReadOptions, 0, "new Gd\\Bmp\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\ReadOptions, 0, "new Gd\\Bmp\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Bmp_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\ReadOptions, 0, "new Gd\\Bmp\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Bmp_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\WriteOptions, 0, "new Gd\\Bmp\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Bmp_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\WriteOptions, 0, "new Gd\\Bmp\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Bmp_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Bmp\\WriteOptions, 0, "new Gd\\Bmp\\WriteOptions()")
ZEND_END_ARG_INFO()

#if defined(HAVE_GD_BUNDLED)
ZEND_METHOD(Gd_Bmp_Info, __construct);
ZEND_METHOD(Gd_Bmp_Reader, __construct);
ZEND_METHOD(Gd_Bmp_Reader, fromFile);
ZEND_METHOD(Gd_Bmp_Reader, fromStream);
ZEND_METHOD(Gd_Bmp_Reader, fromString);
ZEND_METHOD(Gd_Bmp_Reader, info);
ZEND_METHOD(Gd_Bmp_Reader, read);
#endif
ZEND_METHOD(Gd_Bmp_ReadOptions, __construct);
ZEND_METHOD(Gd_Bmp_WriteOptions, __construct);
ZEND_METHOD(Gd_Bmp_Codec, __construct);
ZEND_METHOD(Gd_Bmp_Codec, fromFile);
ZEND_METHOD(Gd_Bmp_Codec, fromString);
ZEND_METHOD(Gd_Bmp_Codec, fromStream);
ZEND_METHOD(Gd_Bmp_Codec, toFile);
ZEND_METHOD(Gd_Bmp_Codec, toStream);
ZEND_METHOD(Gd_Bmp_Codec, toString);

#if defined(HAVE_GD_BUNDLED)
static const zend_function_entry class_Gd_Bmp_Info_methods[] = {
	ZEND_ME(Gd_Bmp_Info, __construct, arginfo_class_Gd_Bmp_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_BUNDLED)
static const zend_function_entry class_Gd_Bmp_Reader_methods[] = {
	ZEND_ME(Gd_Bmp_Reader, __construct, arginfo_class_Gd_Bmp_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Bmp_Reader, fromFile, arginfo_class_Gd_Bmp_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Reader, fromStream, arginfo_class_Gd_Bmp_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Reader, fromString, arginfo_class_Gd_Bmp_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Reader, info, arginfo_class_Gd_Bmp_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Bmp_Reader, read, arginfo_class_Gd_Bmp_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

static const zend_function_entry class_Gd_Bmp_ReadOptions_methods[] = {
	ZEND_ME(Gd_Bmp_ReadOptions, __construct, arginfo_class_Gd_Bmp_ReadOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Bmp_WriteOptions_methods[] = {
	ZEND_ME(Gd_Bmp_WriteOptions, __construct, arginfo_class_Gd_Bmp_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Bmp_Codec_methods[] = {
	ZEND_ME(Gd_Bmp_Codec, __construct, arginfo_class_Gd_Bmp_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Bmp_Codec, fromFile, arginfo_class_Gd_Bmp_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Codec, fromString, arginfo_class_Gd_Bmp_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Codec, fromStream, arginfo_class_Gd_Bmp_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Codec, toFile, arginfo_class_Gd_Bmp_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Codec, toStream, arginfo_class_Gd_Bmp_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Bmp_Codec, toString, arginfo_class_Gd_Bmp_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

#if defined(HAVE_GD_BUNDLED)
static zend_class_entry *register_class_Gd_Bmp_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Bmp", "Info", class_Gd_Bmp_Info_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_headerType_default_value;
	ZVAL_UNDEF(&property_headerType_default_value);
	zend_string *property_headerType_name = zend_string_init("headerType", sizeof("headerType") - 1, true);
	zend_declare_typed_property(class_entry, property_headerType_name, &property_headerType_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_headerType_name, true);

	zval property_width_default_value;
	ZVAL_UNDEF(&property_width_default_value);
	zend_string *property_width_name = zend_string_init("width", sizeof("width") - 1, true);
	zend_declare_typed_property(class_entry, property_width_name, &property_width_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_width_name, true);

	zval property_height_default_value;
	ZVAL_UNDEF(&property_height_default_value);
	zend_string *property_height_name = zend_string_init("height", sizeof("height") - 1, true);
	zend_declare_typed_property(class_entry, property_height_name, &property_height_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_height_name, true);

	zval property_topDown_default_value;
	ZVAL_UNDEF(&property_topDown_default_value);
	zend_string *property_topDown_name = zend_string_init("topDown", sizeof("topDown") - 1, true);
	zend_declare_typed_property(class_entry, property_topDown_name, &property_topDown_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_topDown_name, true);

	zval property_planes_default_value;
	ZVAL_UNDEF(&property_planes_default_value);
	zend_string *property_planes_name = zend_string_init("planes", sizeof("planes") - 1, true);
	zend_declare_typed_property(class_entry, property_planes_name, &property_planes_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_planes_name, true);

	zval property_bitsPerPixel_default_value;
	ZVAL_UNDEF(&property_bitsPerPixel_default_value);
	zend_string *property_bitsPerPixel_name = zend_string_init("bitsPerPixel", sizeof("bitsPerPixel") - 1, true);
	zend_declare_typed_property(class_entry, property_bitsPerPixel_name, &property_bitsPerPixel_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_bitsPerPixel_name, true);

	zval property_compressionTag_default_value;
	ZVAL_UNDEF(&property_compressionTag_default_value);
	zend_string *property_compressionTag_name = zend_string_init("compressionTag", sizeof("compressionTag") - 1, true);
	zend_declare_typed_property(class_entry, property_compressionTag_name, &property_compressionTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_compressionTag_name, true);

	zval property_imageSize_default_value;
	ZVAL_UNDEF(&property_imageSize_default_value);
	zend_string *property_imageSize_name = zend_string_init("imageSize", sizeof("imageSize") - 1, true);
	zend_declare_typed_property(class_entry, property_imageSize_name, &property_imageSize_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_imageSize_name, true);

	zval property_horizontalResolution_default_value;
	ZVAL_UNDEF(&property_horizontalResolution_default_value);
	zend_string *property_horizontalResolution_name = zend_string_init("horizontalResolution", sizeof("horizontalResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_horizontalResolution_name, &property_horizontalResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_horizontalResolution_name, true);

	zval property_verticalResolution_default_value;
	ZVAL_UNDEF(&property_verticalResolution_default_value);
	zend_string *property_verticalResolution_name = zend_string_init("verticalResolution", sizeof("verticalResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_verticalResolution_name, &property_verticalResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_verticalResolution_name, true);

	zval property_colorsUsed_default_value;
	ZVAL_UNDEF(&property_colorsUsed_default_value);
	zend_string *property_colorsUsed_name = zend_string_init("colorsUsed", sizeof("colorsUsed") - 1, true);
	zend_declare_typed_property(class_entry, property_colorsUsed_name, &property_colorsUsed_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_colorsUsed_name, true);

	zval property_importantColors_default_value;
	ZVAL_UNDEF(&property_importantColors_default_value);
	zend_string *property_importantColors_name = zend_string_init("importantColors", sizeof("importantColors") - 1, true);
	zend_declare_typed_property(class_entry, property_importantColors_name, &property_importantColors_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_importantColors_name, true);

	zval property_paletteType_default_value;
	ZVAL_UNDEF(&property_paletteType_default_value);
	zend_string *property_paletteType_name = zend_string_init("paletteType", sizeof("paletteType") - 1, true);
	zend_declare_typed_property(class_entry, property_paletteType_name, &property_paletteType_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_paletteType_name, true);

	zval property_paletteEntries_default_value;
	ZVAL_UNDEF(&property_paletteEntries_default_value);
	zend_string *property_paletteEntries_name = zend_string_init("paletteEntries", sizeof("paletteEntries") - 1, true);
	zend_declare_typed_property(class_entry, property_paletteEntries_name, &property_paletteEntries_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_paletteEntries_name, true);

	zval property_redMask_default_value;
	ZVAL_UNDEF(&property_redMask_default_value);
	zend_string *property_redMask_name = zend_string_init("redMask", sizeof("redMask") - 1, true);
	zend_declare_typed_property(class_entry, property_redMask_name, &property_redMask_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_redMask_name, true);

	zval property_greenMask_default_value;
	ZVAL_UNDEF(&property_greenMask_default_value);
	zend_string *property_greenMask_name = zend_string_init("greenMask", sizeof("greenMask") - 1, true);
	zend_declare_typed_property(class_entry, property_greenMask_name, &property_greenMask_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_greenMask_name, true);

	zval property_blueMask_default_value;
	ZVAL_UNDEF(&property_blueMask_default_value);
	zend_string *property_blueMask_name = zend_string_init("blueMask", sizeof("blueMask") - 1, true);
	zend_declare_typed_property(class_entry, property_blueMask_name, &property_blueMask_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_blueMask_name, true);

	zval property_alphaMask_default_value;
	ZVAL_UNDEF(&property_alphaMask_default_value);
	zend_string *property_alphaMask_name = zend_string_init("alphaMask", sizeof("alphaMask") - 1, true);
	zend_declare_typed_property(class_entry, property_alphaMask_name, &property_alphaMask_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_alphaMask_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_BUNDLED)
static zend_class_entry *register_class_Gd_Bmp_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Bmp", "Reader", class_Gd_Bmp_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

static zend_class_entry *register_class_Gd_Bmp_Compression(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Bmp\\Compression", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "None", NULL);

	zend_enum_add_case_cstr(class_entry, "Rle4", NULL);

	zend_enum_add_case_cstr(class_entry, "Rle8", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Bmp_ReadOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Bmp", "ReadOptions", class_Gd_Bmp_ReadOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Bmp_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Bmp", "WriteOptions", class_Gd_Bmp_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_bitsPerPixel_default_value;
	ZVAL_UNDEF(&property_bitsPerPixel_default_value);
	zend_string *property_bitsPerPixel_name = zend_string_init("bitsPerPixel", sizeof("bitsPerPixel") - 1, true);
	zend_declare_typed_property(class_entry, property_bitsPerPixel_name, &property_bitsPerPixel_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_bitsPerPixel_name, true);

	zval property_compression_default_value;
	ZVAL_UNDEF(&property_compression_default_value);
	zend_string *property_compression_name = zend_string_init("compression", sizeof("compression") - 1, true);
	zend_string *property_compression_class_Gd_Bmp_Compression = zend_string_init("Gd\\Bmp\\Compression", sizeof("Gd\\Bmp\\Compression")-1, 1);
	zend_declare_typed_property(class_entry, property_compression_name, &property_compression_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_compression_class_Gd_Bmp_Compression, 0, MAY_BE_NULL));
	zend_string_release_ex(property_compression_name, true);

	zval property_quantize_default_value;
	ZVAL_UNDEF(&property_quantize_default_value);
	zend_string *property_quantize_name = zend_string_init("quantize", sizeof("quantize") - 1, true);
	zend_declare_typed_property(class_entry, property_quantize_name, &property_quantize_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_quantize_name, true);

	zval property_forceV4Header_default_value;
	ZVAL_UNDEF(&property_forceV4Header_default_value);
	zend_string *property_forceV4Header_name = zend_string_init("forceV4Header", sizeof("forceV4Header") - 1, true);
	zend_declare_typed_property(class_entry, property_forceV4Header_name, &property_forceV4Header_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_forceV4Header_name, true);

	zval property_rgb555_default_value;
	ZVAL_UNDEF(&property_rgb555_default_value);
	zend_string *property_rgb555_name = zend_string_init("rgb555", sizeof("rgb555") - 1, true);
	zend_declare_typed_property(class_entry, property_rgb555_name, &property_rgb555_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_rgb555_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Bmp_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Bmp", "Codec", class_Gd_Bmp_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
