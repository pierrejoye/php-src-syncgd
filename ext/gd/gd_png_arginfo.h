/* This is a generated file, edit gd_png.stub.php instead.
 * Stub hash: 694cb227dc6b7c3ab0e6da2c9d593a114ceee25a
 * Has decl header: yes */

#if defined(HAVE_GD_PNG_CODEC)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Png_ReadOptions___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Png_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, compressionLevel, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, filters, IS_ARRAY, 0, "[]")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, compressionStrategy, Gd\\Png\\CompressionStrategy, 0, "Gd\\Png\\CompressionStrategy::Default")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, comments, IS_ARRAY, 0, "[]")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Png_Info___construct, 0, 0, 18)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, bitDepth, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, colorTypeTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, colorType, Gd\\Png\\ColorType, 1)
	ZEND_ARG_TYPE_INFO(0, hasAlpha, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, hasTransparency, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, paletteEntries, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, interlaceMethodTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, interlaceMethod, Gd\\Png\\InterlaceMethod, 1)
	ZEND_ARG_TYPE_INFO(0, xPixelsPerUnit, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, yPixelsPerUnit, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, resolutionX, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, resolutionY, IS_LONG, 1)
	ZEND_ARG_OBJ_INFO(0, physicalUnit, Gd\\Png\\PhysicalUnit, 1)
	ZEND_ARG_TYPE_INFO(0, comments, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, metadata, Gd\\Metadata, 0)
	ZEND_ARG_TYPE_INFO(0, decodedTrueColor, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Png_Reader___construct arginfo_class_Gd_Png_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Reader_fromFile, 0, 1, Gd\\Png\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\ReadOptions, 0, "new Gd\\Png\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Reader_fromString, 0, 1, Gd\\Png\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\ReadOptions, 0, "new Gd\\Png\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Reader_fromStream, 0, 1, Gd\\Png\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\ReadOptions, 0, "new Gd\\Png\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Reader_info, 0, 0, Gd\\Png\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Png_Codec___construct arginfo_class_Gd_Png_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\ReadOptions, 0, "new Gd\\Png\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\ReadOptions, 0, "new Gd\\Png\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Png_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\ReadOptions, 0, "new Gd\\Png\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Png_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\WriteOptions, 0, "new Gd\\Png\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Png_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\WriteOptions, 0, "new Gd\\Png\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Png_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Png\\WriteOptions, 0, "new Gd\\Png\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_PNG_CODEC)
ZEND_METHOD(Gd_Png_ReadOptions, __construct);
ZEND_METHOD(Gd_Png_WriteOptions, __construct);
ZEND_METHOD(Gd_Png_Info, __construct);
ZEND_METHOD(Gd_Png_Reader, __construct);
ZEND_METHOD(Gd_Png_Reader, fromFile);
ZEND_METHOD(Gd_Png_Reader, fromString);
ZEND_METHOD(Gd_Png_Reader, fromStream);
ZEND_METHOD(Gd_Png_Reader, info);
ZEND_METHOD(Gd_Png_Reader, read);
ZEND_METHOD(Gd_Png_Codec, __construct);
ZEND_METHOD(Gd_Png_Codec, fromFile);
ZEND_METHOD(Gd_Png_Codec, fromString);
ZEND_METHOD(Gd_Png_Codec, fromStream);
ZEND_METHOD(Gd_Png_Codec, toFile);
ZEND_METHOD(Gd_Png_Codec, toStream);
ZEND_METHOD(Gd_Png_Codec, toString);
#endif

#if defined(HAVE_GD_PNG_CODEC)
static const zend_function_entry class_Gd_Png_ReadOptions_methods[] = {
	ZEND_ME(Gd_Png_ReadOptions, __construct, arginfo_class_Gd_Png_ReadOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_PNG_CODEC)
static const zend_function_entry class_Gd_Png_WriteOptions_methods[] = {
	ZEND_ME(Gd_Png_WriteOptions, __construct, arginfo_class_Gd_Png_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_PNG_CODEC)
static const zend_function_entry class_Gd_Png_Info_methods[] = {
	ZEND_ME(Gd_Png_Info, __construct, arginfo_class_Gd_Png_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_PNG_CODEC)
static const zend_function_entry class_Gd_Png_Reader_methods[] = {
	ZEND_ME(Gd_Png_Reader, __construct, arginfo_class_Gd_Png_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Png_Reader, fromFile, arginfo_class_Gd_Png_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Reader, fromString, arginfo_class_Gd_Png_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Reader, fromStream, arginfo_class_Gd_Png_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Reader, info, arginfo_class_Gd_Png_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Png_Reader, read, arginfo_class_Gd_Png_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_PNG_CODEC)
static const zend_function_entry class_Gd_Png_Codec_methods[] = {
	ZEND_ME(Gd_Png_Codec, __construct, arginfo_class_Gd_Png_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Png_Codec, fromFile, arginfo_class_Gd_Png_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Codec, fromString, arginfo_class_Gd_Png_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Codec, fromStream, arginfo_class_Gd_Png_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Codec, toFile, arginfo_class_Gd_Png_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Codec, toStream, arginfo_class_Gd_Png_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Png_Codec, toString, arginfo_class_Gd_Png_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_ColorType(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Png\\ColorType", IS_LONG, NULL);

	zval enum_case_Grayscale_value;
	ZVAL_LONG(&enum_case_Grayscale_value, 0);
	zend_enum_add_case_cstr(class_entry, "Grayscale", &enum_case_Grayscale_value);

	zval enum_case_Rgb_value;
	ZVAL_LONG(&enum_case_Rgb_value, 2);
	zend_enum_add_case_cstr(class_entry, "Rgb", &enum_case_Rgb_value);

	zval enum_case_Palette_value;
	ZVAL_LONG(&enum_case_Palette_value, 3);
	zend_enum_add_case_cstr(class_entry, "Palette", &enum_case_Palette_value);

	zval enum_case_GrayscaleAlpha_value;
	ZVAL_LONG(&enum_case_GrayscaleAlpha_value, 4);
	zend_enum_add_case_cstr(class_entry, "GrayscaleAlpha", &enum_case_GrayscaleAlpha_value);

	zval enum_case_Rgba_value;
	ZVAL_LONG(&enum_case_Rgba_value, 6);
	zend_enum_add_case_cstr(class_entry, "Rgba", &enum_case_Rgba_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_InterlaceMethod(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Png\\InterlaceMethod", IS_LONG, NULL);

	zval enum_case_None_value;
	ZVAL_LONG(&enum_case_None_value, 0);
	zend_enum_add_case_cstr(class_entry, "None", &enum_case_None_value);

	zval enum_case_Adam7_value;
	ZVAL_LONG(&enum_case_Adam7_value, 1);
	zend_enum_add_case_cstr(class_entry, "Adam7", &enum_case_Adam7_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_PhysicalUnit(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Png\\PhysicalUnit", IS_LONG, NULL);

	zval enum_case_Unknown_value;
	ZVAL_LONG(&enum_case_Unknown_value, 0);
	zend_enum_add_case_cstr(class_entry, "Unknown", &enum_case_Unknown_value);

	zval enum_case_Meter_value;
	ZVAL_LONG(&enum_case_Meter_value, 1);
	zend_enum_add_case_cstr(class_entry, "Meter", &enum_case_Meter_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_Filter(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Png\\Filter", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "None", NULL);

	zend_enum_add_case_cstr(class_entry, "Sub", NULL);

	zend_enum_add_case_cstr(class_entry, "Up", NULL);

	zend_enum_add_case_cstr(class_entry, "Average", NULL);

	zend_enum_add_case_cstr(class_entry, "Paeth", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_CompressionStrategy(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Png\\CompressionStrategy", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Default", NULL);

	zend_enum_add_case_cstr(class_entry, "Filtered", NULL);

	zend_enum_add_case_cstr(class_entry, "HuffmanOnly", NULL);

	zend_enum_add_case_cstr(class_entry, "Rle", NULL);

	zend_enum_add_case_cstr(class_entry, "Fixed", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_ReadOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Png", "ReadOptions", class_Gd_Png_ReadOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Png", "WriteOptions", class_Gd_Png_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_compressionLevel_default_value;
	ZVAL_UNDEF(&property_compressionLevel_default_value);
	zend_string *property_compressionLevel_name = zend_string_init("compressionLevel", sizeof("compressionLevel") - 1, true);
	zend_declare_typed_property(class_entry, property_compressionLevel_name, &property_compressionLevel_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_compressionLevel_name, true);

	zval property_filters_default_value;
	ZVAL_UNDEF(&property_filters_default_value);
	zend_string *property_filters_name = zend_string_init("filters", sizeof("filters") - 1, true);
	zend_declare_typed_property(class_entry, property_filters_name, &property_filters_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release_ex(property_filters_name, true);

	zval property_compressionStrategy_default_value;
	ZVAL_UNDEF(&property_compressionStrategy_default_value);
	zend_string *property_compressionStrategy_name = zend_string_init("compressionStrategy", sizeof("compressionStrategy") - 1, true);
	zend_string *property_compressionStrategy_class_Gd_Png_CompressionStrategy = zend_string_init("Gd\\Png\\CompressionStrategy", sizeof("Gd\\Png\\CompressionStrategy")-1, 1);
	zend_declare_typed_property(class_entry, property_compressionStrategy_name, &property_compressionStrategy_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_compressionStrategy_class_Gd_Png_CompressionStrategy, 0, 0));
	zend_string_release_ex(property_compressionStrategy_name, true);

	zval property_comments_default_value;
	ZVAL_UNDEF(&property_comments_default_value);
	zend_string *property_comments_name = zend_string_init("comments", sizeof("comments") - 1, true);
	zend_declare_typed_property(class_entry, property_comments_name, &property_comments_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release_ex(property_comments_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Png", "Info", class_Gd_Png_Info_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

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

	zval property_bitDepth_default_value;
	ZVAL_UNDEF(&property_bitDepth_default_value);
	zend_string *property_bitDepth_name = zend_string_init("bitDepth", sizeof("bitDepth") - 1, true);
	zend_declare_typed_property(class_entry, property_bitDepth_name, &property_bitDepth_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_bitDepth_name, true);

	zval property_colorTypeTag_default_value;
	ZVAL_UNDEF(&property_colorTypeTag_default_value);
	zend_string *property_colorTypeTag_name = zend_string_init("colorTypeTag", sizeof("colorTypeTag") - 1, true);
	zend_declare_typed_property(class_entry, property_colorTypeTag_name, &property_colorTypeTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_colorTypeTag_name, true);

	zval property_colorType_default_value;
	ZVAL_UNDEF(&property_colorType_default_value);
	zend_string *property_colorType_name = zend_string_init("colorType", sizeof("colorType") - 1, true);
	zend_string *property_colorType_class_Gd_Png_ColorType = zend_string_init("Gd\\Png\\ColorType", sizeof("Gd\\Png\\ColorType")-1, 1);
	zend_declare_typed_property(class_entry, property_colorType_name, &property_colorType_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_colorType_class_Gd_Png_ColorType, 0, MAY_BE_NULL));
	zend_string_release_ex(property_colorType_name, true);

	zval property_hasAlpha_default_value;
	ZVAL_UNDEF(&property_hasAlpha_default_value);
	zend_string *property_hasAlpha_name = zend_string_init("hasAlpha", sizeof("hasAlpha") - 1, true);
	zend_declare_typed_property(class_entry, property_hasAlpha_name, &property_hasAlpha_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_hasAlpha_name, true);

	zval property_hasTransparency_default_value;
	ZVAL_UNDEF(&property_hasTransparency_default_value);
	zend_string *property_hasTransparency_name = zend_string_init("hasTransparency", sizeof("hasTransparency") - 1, true);
	zend_declare_typed_property(class_entry, property_hasTransparency_name, &property_hasTransparency_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_hasTransparency_name, true);

	zval property_paletteEntries_default_value;
	ZVAL_UNDEF(&property_paletteEntries_default_value);
	zend_string *property_paletteEntries_name = zend_string_init("paletteEntries", sizeof("paletteEntries") - 1, true);
	zend_declare_typed_property(class_entry, property_paletteEntries_name, &property_paletteEntries_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_paletteEntries_name, true);

	zval property_interlaceMethodTag_default_value;
	ZVAL_UNDEF(&property_interlaceMethodTag_default_value);
	zend_string *property_interlaceMethodTag_name = zend_string_init("interlaceMethodTag", sizeof("interlaceMethodTag") - 1, true);
	zend_declare_typed_property(class_entry, property_interlaceMethodTag_name, &property_interlaceMethodTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_interlaceMethodTag_name, true);

	zval property_interlaceMethod_default_value;
	ZVAL_UNDEF(&property_interlaceMethod_default_value);
	zend_string *property_interlaceMethod_name = zend_string_init("interlaceMethod", sizeof("interlaceMethod") - 1, true);
	zend_string *property_interlaceMethod_class_Gd_Png_InterlaceMethod = zend_string_init("Gd\\Png\\InterlaceMethod", sizeof("Gd\\Png\\InterlaceMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_interlaceMethod_name, &property_interlaceMethod_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_interlaceMethod_class_Gd_Png_InterlaceMethod, 0, MAY_BE_NULL));
	zend_string_release_ex(property_interlaceMethod_name, true);

	zval property_xPixelsPerUnit_default_value;
	ZVAL_UNDEF(&property_xPixelsPerUnit_default_value);
	zend_string *property_xPixelsPerUnit_name = zend_string_init("xPixelsPerUnit", sizeof("xPixelsPerUnit") - 1, true);
	zend_declare_typed_property(class_entry, property_xPixelsPerUnit_name, &property_xPixelsPerUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_xPixelsPerUnit_name, true);

	zval property_yPixelsPerUnit_default_value;
	ZVAL_UNDEF(&property_yPixelsPerUnit_default_value);
	zend_string *property_yPixelsPerUnit_name = zend_string_init("yPixelsPerUnit", sizeof("yPixelsPerUnit") - 1, true);
	zend_declare_typed_property(class_entry, property_yPixelsPerUnit_name, &property_yPixelsPerUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_yPixelsPerUnit_name, true);

	zval property_resolutionX_default_value;
	ZVAL_UNDEF(&property_resolutionX_default_value);
	zend_string *property_resolutionX_name = zend_string_init("resolutionX", sizeof("resolutionX") - 1, true);
	zend_declare_typed_property(class_entry, property_resolutionX_name, &property_resolutionX_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_resolutionX_name, true);

	zval property_resolutionY_default_value;
	ZVAL_UNDEF(&property_resolutionY_default_value);
	zend_string *property_resolutionY_name = zend_string_init("resolutionY", sizeof("resolutionY") - 1, true);
	zend_declare_typed_property(class_entry, property_resolutionY_name, &property_resolutionY_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_resolutionY_name, true);

	zval property_physicalUnit_default_value;
	ZVAL_UNDEF(&property_physicalUnit_default_value);
	zend_string *property_physicalUnit_name = zend_string_init("physicalUnit", sizeof("physicalUnit") - 1, true);
	zend_string *property_physicalUnit_class_Gd_Png_PhysicalUnit = zend_string_init("Gd\\Png\\PhysicalUnit", sizeof("Gd\\Png\\PhysicalUnit")-1, 1);
	zend_declare_typed_property(class_entry, property_physicalUnit_name, &property_physicalUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_physicalUnit_class_Gd_Png_PhysicalUnit, 0, MAY_BE_NULL));
	zend_string_release_ex(property_physicalUnit_name, true);

	zval property_comments_default_value;
	ZVAL_UNDEF(&property_comments_default_value);
	zend_string *property_comments_name = zend_string_init("comments", sizeof("comments") - 1, true);
	zend_declare_typed_property(class_entry, property_comments_name, &property_comments_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release_ex(property_comments_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, 0));
	zend_string_release_ex(property_metadata_name, true);

	zval property_decodedTrueColor_default_value;
	ZVAL_UNDEF(&property_decodedTrueColor_default_value);
	zend_string *property_decodedTrueColor_name = zend_string_init("decodedTrueColor", sizeof("decodedTrueColor") - 1, true);
	zend_declare_typed_property(class_entry, property_decodedTrueColor_name, &property_decodedTrueColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_decodedTrueColor_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Png", "Reader", class_Gd_Png_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_PNG_CODEC)
static zend_class_entry *register_class_Gd_Png_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Png", "Codec", class_Gd_Png_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif
