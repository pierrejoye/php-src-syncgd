/* This is a generated file, edit gd_jpeg.stub.php instead.
 * Stub hash: 779e5e7c0a852d49ac864aa89361cbbc02f15252
 * Has decl header: yes */

#if defined(HAVE_GD_JPEG_CODEC)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jpeg_Info___construct, 0, 0, 12)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, bitsPerSample, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, components, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, colorSpaceTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, colorSpace, Gd\\Jpeg\\ColorSpace, 1)
	ZEND_ARG_TYPE_INFO(0, progressive, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, densityUnitTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, densityUnit, Gd\\Jpeg\\DensityUnit, 1)
	ZEND_ARG_TYPE_INFO(0, xDensity, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, yDensity, IS_LONG, 1)
	ZEND_ARG_OBJ_INFO(0, metadata, Gd\\Metadata, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jpeg_ReadOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, ignoreWarnings, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, scaleNumerator, IS_LONG, 0, "1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, scaleDenominator, IS_LONG, 0, "1")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, dctMethod, Gd\\Jpeg\\DctMethod, 0, "Gd\\Jpeg\\DctMethod::Default")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jpeg_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quality, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, progressive, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, forceNoSubsampling, _IS_BOOL, 0, "false")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jpeg_Reader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Reader_fromFile, 0, 1, Gd\\Jpeg\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\ReadOptions, 0, "new Gd\\Jpeg\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Reader_fromString, 0, 1, Gd\\Jpeg\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\ReadOptions, 0, "new Gd\\Jpeg\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Reader_fromStream, 0, 1, Gd\\Jpeg\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\ReadOptions, 0, "new Gd\\Jpeg\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Reader_info, 0, 0, Gd\\Jpeg\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Jpeg_Codec___construct arginfo_class_Gd_Jpeg_Reader___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\ReadOptions, 0, "new Gd\\Jpeg\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\ReadOptions, 0, "new Gd\\Jpeg\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jpeg_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\ReadOptions, 0, "new Gd\\Jpeg\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jpeg_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\WriteOptions, 0, "new Gd\\Jpeg\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jpeg_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\WriteOptions, 0, "new Gd\\Jpeg\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jpeg_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jpeg\\WriteOptions, 0, "new Gd\\Jpeg\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_JPEG_CODEC)
ZEND_METHOD(Gd_Jpeg_Info, __construct);
ZEND_METHOD(Gd_Jpeg_ReadOptions, __construct);
ZEND_METHOD(Gd_Jpeg_WriteOptions, __construct);
ZEND_METHOD(Gd_Jpeg_Reader, __construct);
ZEND_METHOD(Gd_Jpeg_Reader, fromFile);
ZEND_METHOD(Gd_Jpeg_Reader, fromString);
ZEND_METHOD(Gd_Jpeg_Reader, fromStream);
ZEND_METHOD(Gd_Jpeg_Reader, info);
ZEND_METHOD(Gd_Jpeg_Reader, read);
ZEND_METHOD(Gd_Jpeg_Codec, __construct);
ZEND_METHOD(Gd_Jpeg_Codec, fromFile);
ZEND_METHOD(Gd_Jpeg_Codec, fromString);
ZEND_METHOD(Gd_Jpeg_Codec, fromStream);
ZEND_METHOD(Gd_Jpeg_Codec, toFile);
ZEND_METHOD(Gd_Jpeg_Codec, toStream);
ZEND_METHOD(Gd_Jpeg_Codec, toString);
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static const zend_function_entry class_Gd_Jpeg_Info_methods[] = {
	ZEND_ME(Gd_Jpeg_Info, __construct, arginfo_class_Gd_Jpeg_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static const zend_function_entry class_Gd_Jpeg_ReadOptions_methods[] = {
	ZEND_ME(Gd_Jpeg_ReadOptions, __construct, arginfo_class_Gd_Jpeg_ReadOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static const zend_function_entry class_Gd_Jpeg_WriteOptions_methods[] = {
	ZEND_ME(Gd_Jpeg_WriteOptions, __construct, arginfo_class_Gd_Jpeg_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static const zend_function_entry class_Gd_Jpeg_Reader_methods[] = {
	ZEND_ME(Gd_Jpeg_Reader, __construct, arginfo_class_Gd_Jpeg_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Jpeg_Reader, fromFile, arginfo_class_Gd_Jpeg_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Reader, fromString, arginfo_class_Gd_Jpeg_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Reader, fromStream, arginfo_class_Gd_Jpeg_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Reader, info, arginfo_class_Gd_Jpeg_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Jpeg_Reader, read, arginfo_class_Gd_Jpeg_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static const zend_function_entry class_Gd_Jpeg_Codec_methods[] = {
	ZEND_ME(Gd_Jpeg_Codec, __construct, arginfo_class_Gd_Jpeg_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Jpeg_Codec, fromFile, arginfo_class_Gd_Jpeg_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Codec, fromString, arginfo_class_Gd_Jpeg_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Codec, fromStream, arginfo_class_Gd_Jpeg_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Codec, toFile, arginfo_class_Gd_Jpeg_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Codec, toStream, arginfo_class_Gd_Jpeg_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jpeg_Codec, toString, arginfo_class_Gd_Jpeg_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_ColorSpace(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Jpeg\\ColorSpace", IS_LONG, NULL);

	zval enum_case_Unknown_value;
	ZVAL_LONG(&enum_case_Unknown_value, 0);
	zend_enum_add_case_cstr(class_entry, "Unknown", &enum_case_Unknown_value);

	zval enum_case_Grayscale_value;
	ZVAL_LONG(&enum_case_Grayscale_value, 1);
	zend_enum_add_case_cstr(class_entry, "Grayscale", &enum_case_Grayscale_value);

	zval enum_case_Rgb_value;
	ZVAL_LONG(&enum_case_Rgb_value, 2);
	zend_enum_add_case_cstr(class_entry, "Rgb", &enum_case_Rgb_value);

	zval enum_case_YCbCr_value;
	ZVAL_LONG(&enum_case_YCbCr_value, 3);
	zend_enum_add_case_cstr(class_entry, "YCbCr", &enum_case_YCbCr_value);

	zval enum_case_Cmyk_value;
	ZVAL_LONG(&enum_case_Cmyk_value, 4);
	zend_enum_add_case_cstr(class_entry, "Cmyk", &enum_case_Cmyk_value);

	zval enum_case_Ycck_value;
	ZVAL_LONG(&enum_case_Ycck_value, 5);
	zend_enum_add_case_cstr(class_entry, "Ycck", &enum_case_Ycck_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_DensityUnit(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Jpeg\\DensityUnit", IS_LONG, NULL);

	zval enum_case_None_value;
	ZVAL_LONG(&enum_case_None_value, 0);
	zend_enum_add_case_cstr(class_entry, "None", &enum_case_None_value);

	zval enum_case_Dpi_value;
	ZVAL_LONG(&enum_case_Dpi_value, 1);
	zend_enum_add_case_cstr(class_entry, "Dpi", &enum_case_Dpi_value);

	zval enum_case_Dpcm_value;
	ZVAL_LONG(&enum_case_Dpcm_value, 2);
	zend_enum_add_case_cstr(class_entry, "Dpcm", &enum_case_Dpcm_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_DctMethod(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Jpeg\\DctMethod", IS_LONG, NULL);

	zval enum_case_Default_value;
	ZVAL_LONG(&enum_case_Default_value, 0);
	zend_enum_add_case_cstr(class_entry, "Default", &enum_case_Default_value);

	zval enum_case_Slow_value;
	ZVAL_LONG(&enum_case_Slow_value, 1);
	zend_enum_add_case_cstr(class_entry, "Slow", &enum_case_Slow_value);

	zval enum_case_Fast_value;
	ZVAL_LONG(&enum_case_Fast_value, 2);
	zend_enum_add_case_cstr(class_entry, "Fast", &enum_case_Fast_value);

	zval enum_case_Float_value;
	ZVAL_LONG(&enum_case_Float_value, 3);
	zend_enum_add_case_cstr(class_entry, "Float", &enum_case_Float_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jpeg", "Info", class_Gd_Jpeg_Info_methods);
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

	zval property_bitsPerSample_default_value;
	ZVAL_UNDEF(&property_bitsPerSample_default_value);
	zend_string *property_bitsPerSample_name = zend_string_init("bitsPerSample", sizeof("bitsPerSample") - 1, true);
	zend_declare_typed_property(class_entry, property_bitsPerSample_name, &property_bitsPerSample_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_bitsPerSample_name, true);

	zval property_components_default_value;
	ZVAL_UNDEF(&property_components_default_value);
	zend_string *property_components_name = zend_string_init("components", sizeof("components") - 1, true);
	zend_declare_typed_property(class_entry, property_components_name, &property_components_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_components_name, true);

	zval property_colorSpaceTag_default_value;
	ZVAL_UNDEF(&property_colorSpaceTag_default_value);
	zend_string *property_colorSpaceTag_name = zend_string_init("colorSpaceTag", sizeof("colorSpaceTag") - 1, true);
	zend_declare_typed_property(class_entry, property_colorSpaceTag_name, &property_colorSpaceTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_colorSpaceTag_name, true);

	zval property_colorSpace_default_value;
	ZVAL_UNDEF(&property_colorSpace_default_value);
	zend_string *property_colorSpace_name = zend_string_init("colorSpace", sizeof("colorSpace") - 1, true);
	zend_string *property_colorSpace_class_Gd_Jpeg_ColorSpace = zend_string_init("Gd\\Jpeg\\ColorSpace", sizeof("Gd\\Jpeg\\ColorSpace")-1, 1);
	zend_declare_typed_property(class_entry, property_colorSpace_name, &property_colorSpace_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_colorSpace_class_Gd_Jpeg_ColorSpace, 0, MAY_BE_NULL));
	zend_string_release_ex(property_colorSpace_name, true);

	zval property_progressive_default_value;
	ZVAL_UNDEF(&property_progressive_default_value);
	zend_string *property_progressive_name = zend_string_init("progressive", sizeof("progressive") - 1, true);
	zend_declare_typed_property(class_entry, property_progressive_name, &property_progressive_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_progressive_name, true);

	zval property_densityUnitTag_default_value;
	ZVAL_UNDEF(&property_densityUnitTag_default_value);
	zend_string *property_densityUnitTag_name = zend_string_init("densityUnitTag", sizeof("densityUnitTag") - 1, true);
	zend_declare_typed_property(class_entry, property_densityUnitTag_name, &property_densityUnitTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_densityUnitTag_name, true);

	zval property_densityUnit_default_value;
	ZVAL_UNDEF(&property_densityUnit_default_value);
	zend_string *property_densityUnit_name = zend_string_init("densityUnit", sizeof("densityUnit") - 1, true);
	zend_string *property_densityUnit_class_Gd_Jpeg_DensityUnit = zend_string_init("Gd\\Jpeg\\DensityUnit", sizeof("Gd\\Jpeg\\DensityUnit")-1, 1);
	zend_declare_typed_property(class_entry, property_densityUnit_name, &property_densityUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_densityUnit_class_Gd_Jpeg_DensityUnit, 0, MAY_BE_NULL));
	zend_string_release_ex(property_densityUnit_name, true);

	zval property_xDensity_default_value;
	ZVAL_UNDEF(&property_xDensity_default_value);
	zend_string *property_xDensity_name = zend_string_init("xDensity", sizeof("xDensity") - 1, true);
	zend_declare_typed_property(class_entry, property_xDensity_name, &property_xDensity_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_xDensity_name, true);

	zval property_yDensity_default_value;
	ZVAL_UNDEF(&property_yDensity_default_value);
	zend_string *property_yDensity_name = zend_string_init("yDensity", sizeof("yDensity") - 1, true);
	zend_declare_typed_property(class_entry, property_yDensity_name, &property_yDensity_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_yDensity_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, 0));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_ReadOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jpeg", "ReadOptions", class_Gd_Jpeg_ReadOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_ignoreWarnings_default_value;
	ZVAL_UNDEF(&property_ignoreWarnings_default_value);
	zend_string *property_ignoreWarnings_name = zend_string_init("ignoreWarnings", sizeof("ignoreWarnings") - 1, true);
	zend_declare_typed_property(class_entry, property_ignoreWarnings_name, &property_ignoreWarnings_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_ignoreWarnings_name, true);

	zval property_scaleNumerator_default_value;
	ZVAL_UNDEF(&property_scaleNumerator_default_value);
	zend_string *property_scaleNumerator_name = zend_string_init("scaleNumerator", sizeof("scaleNumerator") - 1, true);
	zend_declare_typed_property(class_entry, property_scaleNumerator_name, &property_scaleNumerator_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_scaleNumerator_name, true);

	zval property_scaleDenominator_default_value;
	ZVAL_UNDEF(&property_scaleDenominator_default_value);
	zend_string *property_scaleDenominator_name = zend_string_init("scaleDenominator", sizeof("scaleDenominator") - 1, true);
	zend_declare_typed_property(class_entry, property_scaleDenominator_name, &property_scaleDenominator_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_scaleDenominator_name, true);

	zval property_dctMethod_default_value;
	ZVAL_UNDEF(&property_dctMethod_default_value);
	zend_string *property_dctMethod_name = zend_string_init("dctMethod", sizeof("dctMethod") - 1, true);
	zend_string *property_dctMethod_class_Gd_Jpeg_DctMethod = zend_string_init("Gd\\Jpeg\\DctMethod", sizeof("Gd\\Jpeg\\DctMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_dctMethod_name, &property_dctMethod_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_dctMethod_class_Gd_Jpeg_DctMethod, 0, 0));
	zend_string_release_ex(property_dctMethod_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jpeg", "WriteOptions", class_Gd_Jpeg_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_quality_default_value;
	ZVAL_UNDEF(&property_quality_default_value);
	zend_string *property_quality_name = zend_string_init("quality", sizeof("quality") - 1, true);
	zend_declare_typed_property(class_entry, property_quality_name, &property_quality_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_quality_name, true);

	zval property_progressive_default_value;
	ZVAL_UNDEF(&property_progressive_default_value);
	zend_string *property_progressive_name = zend_string_init("progressive", sizeof("progressive") - 1, true);
	zend_declare_typed_property(class_entry, property_progressive_name, &property_progressive_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_progressive_name, true);

	zval property_forceNoSubsampling_default_value;
	ZVAL_UNDEF(&property_forceNoSubsampling_default_value);
	zend_string *property_forceNoSubsampling_name = zend_string_init("forceNoSubsampling", sizeof("forceNoSubsampling") - 1, true);
	zend_declare_typed_property(class_entry, property_forceNoSubsampling_name, &property_forceNoSubsampling_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_forceNoSubsampling_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jpeg", "Reader", class_Gd_Jpeg_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JPEG_CODEC)
static zend_class_entry *register_class_Gd_Jpeg_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jpeg", "Codec", class_Gd_Jpeg_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif
