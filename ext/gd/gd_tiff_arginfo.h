/* This is a generated file, edit gd_tiff.stub.php instead.
 * Stub hash: d6c4296d30ee85e978b13c10f899017ba5dbbd72
 * Has decl header: yes */

#if defined(HAVE_GD_TIFF_WRITE_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Tiff_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, compression, Gd\\Tiff\\Compression, 0, "Gd\\Tiff\\Compression::Deflate")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, colorSpace, Gd\\Tiff\\ColorSpace, 0, "Gd\\Tiff\\ColorSpace::Rgba")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, minIsWhite, _IS_BOOL, 0, "false")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, resolutionUnit, Gd\\Tiff\\ResolutionUnit, 0, "Gd\\Tiff\\ResolutionUnit::Inch")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, xResolution, IS_DOUBLE, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, yResolution, IS_DOUBLE, 1, "null")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Tiff_Writer___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Writer_toFile, 0, 1, Gd\\Tiff\\Writer, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Tiff\\WriteOptions, 0, "new Gd\\Tiff\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Writer_toStream, 0, 1, Gd\\Tiff\\Writer, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Tiff\\WriteOptions, 0, "new Gd\\Tiff\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Writer_toMemory, 0, 0, Gd\\Tiff\\Writer, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Tiff\\WriteOptions, 0, "new Gd\\Tiff\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Tiff_Writer_addPage, 0, 1, IS_STATIC, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Tiff_Writer_finish, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) && defined(HAVE_GD_TIFF_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Tiff_Codec___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Tiff_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Tiff\\WriteOptions, 0, "new Gd\\Tiff\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Tiff_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Tiff\\WriteOptions, 0, "new Gd\\Tiff\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Tiff_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Tiff\\WriteOptions, 0, "new Gd\\Tiff\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_TIFF_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Tiff_Info___construct, 0, 0, 14)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, pageCount, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, bitsPerSample, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, samplesPerPixel, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, compressionTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, compression, Gd\\Tiff\\Compression, 1)
	ZEND_ARG_TYPE_INFO(0, photometricTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, photometric, Gd\\Tiff\\Photometric, 1)
	ZEND_ARG_TYPE_INFO(0, minIsWhite, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, xResolution, IS_DOUBLE, 1)
	ZEND_ARG_TYPE_INFO(0, yResolution, IS_DOUBLE, 1)
	ZEND_ARG_OBJ_INFO(0, resolutionUnit, Gd\\Tiff\\ResolutionUnit, 1)
	ZEND_ARG_OBJ_INFO(0, metadata, Gd\\Metadata, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Tiff_Page___construct, 0, 0, 18)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, pageIndex, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, bitsPerSample, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, samplesPerPixel, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, compressionTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, compression, Gd\\Tiff\\Compression, 1)
	ZEND_ARG_TYPE_INFO(0, photometricTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, photometric, Gd\\Tiff\\Photometric, 1)
	ZEND_ARG_TYPE_INFO(0, planarConfigurationTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, planarConfiguration, Gd\\Tiff\\PlanarConfiguration, 1)
	ZEND_ARG_TYPE_INFO(0, minIsWhite, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, hasAlpha, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, isTiled, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, xResolution, IS_DOUBLE, 1)
	ZEND_ARG_TYPE_INFO(0, yResolution, IS_DOUBLE, 1)
	ZEND_ARG_OBJ_INFO(0, resolutionUnit, Gd\\Tiff\\ResolutionUnit, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Tiff_Reader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Reader_fromFile, 0, 1, Gd\\Tiff\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Reader_fromString, 0, 1, Gd\\Tiff\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Reader_fromStream, 0, 1, Gd\\Tiff\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Reader_info, 0, 0, Gd\\Tiff\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Tiff_Reader_next, 0, 0, Gd\\Tiff\\Page, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_TIFF_WRITE_API)
ZEND_METHOD(Gd_Tiff_WriteOptions, __construct);
ZEND_METHOD(Gd_Tiff_Writer, __construct);
ZEND_METHOD(Gd_Tiff_Writer, toFile);
ZEND_METHOD(Gd_Tiff_Writer, toStream);
ZEND_METHOD(Gd_Tiff_Writer, toMemory);
ZEND_METHOD(Gd_Tiff_Writer, addPage);
ZEND_METHOD(Gd_Tiff_Writer, finish);
#endif
#if defined(HAVE_GD_TIFF_WRITE_API) && defined(HAVE_GD_TIFF_READ_API)
ZEND_METHOD(Gd_Tiff_Codec, __construct);
ZEND_METHOD(Gd_Tiff_Codec, fromFile);
ZEND_METHOD(Gd_Tiff_Codec, fromString);
ZEND_METHOD(Gd_Tiff_Codec, fromStream);
ZEND_METHOD(Gd_Tiff_Codec, toFile);
ZEND_METHOD(Gd_Tiff_Codec, toStream);
ZEND_METHOD(Gd_Tiff_Codec, toString);
#endif
#if defined(HAVE_GD_TIFF_READ_API)
ZEND_METHOD(Gd_Tiff_Info, __construct);
ZEND_METHOD(Gd_Tiff_Page, __construct);
ZEND_METHOD(Gd_Tiff_Reader, __construct);
ZEND_METHOD(Gd_Tiff_Reader, fromFile);
ZEND_METHOD(Gd_Tiff_Reader, fromString);
ZEND_METHOD(Gd_Tiff_Reader, fromStream);
ZEND_METHOD(Gd_Tiff_Reader, info);
ZEND_METHOD(Gd_Tiff_Reader, next);
#endif

#if defined(HAVE_GD_TIFF_WRITE_API)
static const zend_function_entry class_Gd_Tiff_WriteOptions_methods[] = {
	ZEND_ME(Gd_Tiff_WriteOptions, __construct, arginfo_class_Gd_Tiff_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_TIFF_WRITE_API)
static const zend_function_entry class_Gd_Tiff_Writer_methods[] = {
	ZEND_ME(Gd_Tiff_Writer, __construct, arginfo_class_Gd_Tiff_Writer___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Tiff_Writer, toFile, arginfo_class_Gd_Tiff_Writer_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Writer, toStream, arginfo_class_Gd_Tiff_Writer_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Writer, toMemory, arginfo_class_Gd_Tiff_Writer_toMemory, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Writer, addPage, arginfo_class_Gd_Tiff_Writer_addPage, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Tiff_Writer, finish, arginfo_class_Gd_Tiff_Writer_finish, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) && defined(HAVE_GD_TIFF_READ_API)
static const zend_function_entry class_Gd_Tiff_Codec_methods[] = {
	ZEND_ME(Gd_Tiff_Codec, __construct, arginfo_class_Gd_Tiff_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Tiff_Codec, fromFile, arginfo_class_Gd_Tiff_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Codec, fromString, arginfo_class_Gd_Tiff_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Codec, fromStream, arginfo_class_Gd_Tiff_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Codec, toFile, arginfo_class_Gd_Tiff_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Codec, toStream, arginfo_class_Gd_Tiff_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Codec, toString, arginfo_class_Gd_Tiff_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static const zend_function_entry class_Gd_Tiff_Info_methods[] = {
	ZEND_ME(Gd_Tiff_Info, __construct, arginfo_class_Gd_Tiff_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static const zend_function_entry class_Gd_Tiff_Page_methods[] = {
	ZEND_ME(Gd_Tiff_Page, __construct, arginfo_class_Gd_Tiff_Page___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static const zend_function_entry class_Gd_Tiff_Reader_methods[] = {
	ZEND_ME(Gd_Tiff_Reader, __construct, arginfo_class_Gd_Tiff_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Tiff_Reader, fromFile, arginfo_class_Gd_Tiff_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Reader, fromString, arginfo_class_Gd_Tiff_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Reader, fromStream, arginfo_class_Gd_Tiff_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Tiff_Reader, info, arginfo_class_Gd_Tiff_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Tiff_Reader, next, arginfo_class_Gd_Tiff_Reader_next, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_ColorSpace(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Tiff\\ColorSpace", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Rgb", NULL);

	zend_enum_add_case_cstr(class_entry, "Rgba", NULL);

	zend_enum_add_case_cstr(class_entry, "Gray", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_Compression(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Tiff\\Compression", IS_LONG, NULL);

	zval enum_case_None_value;
	ZVAL_LONG(&enum_case_None_value, 1);
	zend_enum_add_case_cstr(class_entry, "None", &enum_case_None_value);

	zval enum_case_CcittRle_value;
	ZVAL_LONG(&enum_case_CcittRle_value, 2);
	zend_enum_add_case_cstr(class_entry, "CcittRle", &enum_case_CcittRle_value);

	zval enum_case_CcittFax3_value;
	ZVAL_LONG(&enum_case_CcittFax3_value, 3);
	zend_enum_add_case_cstr(class_entry, "CcittFax3", &enum_case_CcittFax3_value);

	zval enum_case_CcittFax4_value;
	ZVAL_LONG(&enum_case_CcittFax4_value, 4);
	zend_enum_add_case_cstr(class_entry, "CcittFax4", &enum_case_CcittFax4_value);

	zval enum_case_Lzw_value;
	ZVAL_LONG(&enum_case_Lzw_value, 5);
	zend_enum_add_case_cstr(class_entry, "Lzw", &enum_case_Lzw_value);

	zval enum_case_Jpeg_value;
	ZVAL_LONG(&enum_case_Jpeg_value, 7);
	zend_enum_add_case_cstr(class_entry, "Jpeg", &enum_case_Jpeg_value);

	zval enum_case_AdobeDeflate_value;
	ZVAL_LONG(&enum_case_AdobeDeflate_value, 8);
	zend_enum_add_case_cstr(class_entry, "AdobeDeflate", &enum_case_AdobeDeflate_value);

	zval enum_case_Deflate_value;
	ZVAL_LONG(&enum_case_Deflate_value, 32946);
	zend_enum_add_case_cstr(class_entry, "Deflate", &enum_case_Deflate_value);

	zval enum_case_PackBits_value;
	ZVAL_LONG(&enum_case_PackBits_value, 32773);
	zend_enum_add_case_cstr(class_entry, "PackBits", &enum_case_PackBits_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_ResolutionUnit(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Tiff\\ResolutionUnit", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "None", NULL);

	zend_enum_add_case_cstr(class_entry, "Inch", NULL);

	zend_enum_add_case_cstr(class_entry, "Centimeter", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_WRITE_API)
static zend_class_entry *register_class_Gd_Tiff_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Tiff", "WriteOptions", class_Gd_Tiff_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_compression_default_value;
	ZVAL_UNDEF(&property_compression_default_value);
	zend_string *property_compression_name = zend_string_init("compression", sizeof("compression") - 1, true);
	zend_string *property_compression_class_Gd_Tiff_Compression = zend_string_init("Gd\\Tiff\\Compression", sizeof("Gd\\Tiff\\Compression")-1, 1);
	zend_declare_typed_property(class_entry, property_compression_name, &property_compression_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_compression_class_Gd_Tiff_Compression, 0, 0));
	zend_string_release_ex(property_compression_name, true);

	zval property_colorSpace_default_value;
	ZVAL_UNDEF(&property_colorSpace_default_value);
	zend_string *property_colorSpace_name = zend_string_init("colorSpace", sizeof("colorSpace") - 1, true);
	zend_string *property_colorSpace_class_Gd_Tiff_ColorSpace = zend_string_init("Gd\\Tiff\\ColorSpace", sizeof("Gd\\Tiff\\ColorSpace")-1, 1);
	zend_declare_typed_property(class_entry, property_colorSpace_name, &property_colorSpace_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_colorSpace_class_Gd_Tiff_ColorSpace, 0, 0));
	zend_string_release_ex(property_colorSpace_name, true);

	zval property_minIsWhite_default_value;
	ZVAL_UNDEF(&property_minIsWhite_default_value);
	zend_string *property_minIsWhite_name = zend_string_init("minIsWhite", sizeof("minIsWhite") - 1, true);
	zend_declare_typed_property(class_entry, property_minIsWhite_name, &property_minIsWhite_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_minIsWhite_name, true);

	zval property_resolutionUnit_default_value;
	ZVAL_UNDEF(&property_resolutionUnit_default_value);
	zend_string *property_resolutionUnit_name = zend_string_init("resolutionUnit", sizeof("resolutionUnit") - 1, true);
	zend_string *property_resolutionUnit_class_Gd_Tiff_ResolutionUnit = zend_string_init("Gd\\Tiff\\ResolutionUnit", sizeof("Gd\\Tiff\\ResolutionUnit")-1, 1);
	zend_declare_typed_property(class_entry, property_resolutionUnit_name, &property_resolutionUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_resolutionUnit_class_Gd_Tiff_ResolutionUnit, 0, 0));
	zend_string_release_ex(property_resolutionUnit_name, true);

	zval property_xResolution_default_value;
	ZVAL_UNDEF(&property_xResolution_default_value);
	zend_string *property_xResolution_name = zend_string_init("xResolution", sizeof("xResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_xResolution_name, &property_xResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE|MAY_BE_NULL));
	zend_string_release_ex(property_xResolution_name, true);

	zval property_yResolution_default_value;
	ZVAL_UNDEF(&property_yResolution_default_value);
	zend_string *property_yResolution_name = zend_string_init("yResolution", sizeof("yResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_yResolution_name, &property_yResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE|MAY_BE_NULL));
	zend_string_release_ex(property_yResolution_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_WRITE_API)
static zend_class_entry *register_class_Gd_Tiff_Writer(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Tiff", "Writer", class_Gd_Tiff_Writer_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) && defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Tiff", "Codec", class_Gd_Tiff_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_Photometric(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Tiff\\Photometric", IS_LONG, NULL);

	zval enum_case_MinIsWhite_value;
	ZVAL_LONG(&enum_case_MinIsWhite_value, 0);
	zend_enum_add_case_cstr(class_entry, "MinIsWhite", &enum_case_MinIsWhite_value);

	zval enum_case_MinIsBlack_value;
	ZVAL_LONG(&enum_case_MinIsBlack_value, 1);
	zend_enum_add_case_cstr(class_entry, "MinIsBlack", &enum_case_MinIsBlack_value);

	zval enum_case_Rgb_value;
	ZVAL_LONG(&enum_case_Rgb_value, 2);
	zend_enum_add_case_cstr(class_entry, "Rgb", &enum_case_Rgb_value);

	zval enum_case_Palette_value;
	ZVAL_LONG(&enum_case_Palette_value, 3);
	zend_enum_add_case_cstr(class_entry, "Palette", &enum_case_Palette_value);

	zval enum_case_TransparencyMask_value;
	ZVAL_LONG(&enum_case_TransparencyMask_value, 4);
	zend_enum_add_case_cstr(class_entry, "TransparencyMask", &enum_case_TransparencyMask_value);

	zval enum_case_Separated_value;
	ZVAL_LONG(&enum_case_Separated_value, 5);
	zend_enum_add_case_cstr(class_entry, "Separated", &enum_case_Separated_value);

	zval enum_case_YCbCr_value;
	ZVAL_LONG(&enum_case_YCbCr_value, 6);
	zend_enum_add_case_cstr(class_entry, "YCbCr", &enum_case_YCbCr_value);

	zval enum_case_CieLab_value;
	ZVAL_LONG(&enum_case_CieLab_value, 8);
	zend_enum_add_case_cstr(class_entry, "CieLab", &enum_case_CieLab_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_PlanarConfiguration(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Tiff\\PlanarConfiguration", IS_LONG, NULL);

	zval enum_case_Contiguous_value;
	ZVAL_LONG(&enum_case_Contiguous_value, 1);
	zend_enum_add_case_cstr(class_entry, "Contiguous", &enum_case_Contiguous_value);

	zval enum_case_Separate_value;
	ZVAL_LONG(&enum_case_Separate_value, 2);
	zend_enum_add_case_cstr(class_entry, "Separate", &enum_case_Separate_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Tiff", "Info", class_Gd_Tiff_Info_methods);
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

	zval property_pageCount_default_value;
	ZVAL_UNDEF(&property_pageCount_default_value);
	zend_string *property_pageCount_name = zend_string_init("pageCount", sizeof("pageCount") - 1, true);
	zend_declare_typed_property(class_entry, property_pageCount_name, &property_pageCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_pageCount_name, true);

	zval property_bitsPerSample_default_value;
	ZVAL_UNDEF(&property_bitsPerSample_default_value);
	zend_string *property_bitsPerSample_name = zend_string_init("bitsPerSample", sizeof("bitsPerSample") - 1, true);
	zend_declare_typed_property(class_entry, property_bitsPerSample_name, &property_bitsPerSample_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_bitsPerSample_name, true);

	zval property_samplesPerPixel_default_value;
	ZVAL_UNDEF(&property_samplesPerPixel_default_value);
	zend_string *property_samplesPerPixel_name = zend_string_init("samplesPerPixel", sizeof("samplesPerPixel") - 1, true);
	zend_declare_typed_property(class_entry, property_samplesPerPixel_name, &property_samplesPerPixel_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_samplesPerPixel_name, true);

	zval property_compressionTag_default_value;
	ZVAL_UNDEF(&property_compressionTag_default_value);
	zend_string *property_compressionTag_name = zend_string_init("compressionTag", sizeof("compressionTag") - 1, true);
	zend_declare_typed_property(class_entry, property_compressionTag_name, &property_compressionTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_compressionTag_name, true);

	zval property_compression_default_value;
	ZVAL_UNDEF(&property_compression_default_value);
	zend_string *property_compression_name = zend_string_init("compression", sizeof("compression") - 1, true);
	zend_string *property_compression_class_Gd_Tiff_Compression = zend_string_init("Gd\\Tiff\\Compression", sizeof("Gd\\Tiff\\Compression")-1, 1);
	zend_declare_typed_property(class_entry, property_compression_name, &property_compression_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_compression_class_Gd_Tiff_Compression, 0, MAY_BE_NULL));
	zend_string_release_ex(property_compression_name, true);

	zval property_photometricTag_default_value;
	ZVAL_UNDEF(&property_photometricTag_default_value);
	zend_string *property_photometricTag_name = zend_string_init("photometricTag", sizeof("photometricTag") - 1, true);
	zend_declare_typed_property(class_entry, property_photometricTag_name, &property_photometricTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_photometricTag_name, true);

	zval property_photometric_default_value;
	ZVAL_UNDEF(&property_photometric_default_value);
	zend_string *property_photometric_name = zend_string_init("photometric", sizeof("photometric") - 1, true);
	zend_string *property_photometric_class_Gd_Tiff_Photometric = zend_string_init("Gd\\Tiff\\Photometric", sizeof("Gd\\Tiff\\Photometric")-1, 1);
	zend_declare_typed_property(class_entry, property_photometric_name, &property_photometric_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_photometric_class_Gd_Tiff_Photometric, 0, MAY_BE_NULL));
	zend_string_release_ex(property_photometric_name, true);

	zval property_minIsWhite_default_value;
	ZVAL_UNDEF(&property_minIsWhite_default_value);
	zend_string *property_minIsWhite_name = zend_string_init("minIsWhite", sizeof("minIsWhite") - 1, true);
	zend_declare_typed_property(class_entry, property_minIsWhite_name, &property_minIsWhite_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_minIsWhite_name, true);

	zval property_xResolution_default_value;
	ZVAL_UNDEF(&property_xResolution_default_value);
	zend_string *property_xResolution_name = zend_string_init("xResolution", sizeof("xResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_xResolution_name, &property_xResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE|MAY_BE_NULL));
	zend_string_release_ex(property_xResolution_name, true);

	zval property_yResolution_default_value;
	ZVAL_UNDEF(&property_yResolution_default_value);
	zend_string *property_yResolution_name = zend_string_init("yResolution", sizeof("yResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_yResolution_name, &property_yResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE|MAY_BE_NULL));
	zend_string_release_ex(property_yResolution_name, true);

	zval property_resolutionUnit_default_value;
	ZVAL_UNDEF(&property_resolutionUnit_default_value);
	zend_string *property_resolutionUnit_name = zend_string_init("resolutionUnit", sizeof("resolutionUnit") - 1, true);
	zend_string *property_resolutionUnit_class_Gd_Tiff_ResolutionUnit = zend_string_init("Gd\\Tiff\\ResolutionUnit", sizeof("Gd\\Tiff\\ResolutionUnit")-1, 1);
	zend_declare_typed_property(class_entry, property_resolutionUnit_name, &property_resolutionUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_resolutionUnit_class_Gd_Tiff_ResolutionUnit, 0, MAY_BE_NULL));
	zend_string_release_ex(property_resolutionUnit_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, 0));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_Page(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Tiff", "Page", class_Gd_Tiff_Page_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_image_default_value;
	ZVAL_UNDEF(&property_image_default_value);
	zend_string *property_image_name = zend_string_init("image", sizeof("image") - 1, true);
	zend_string *property_image_class_GdImage = zend_string_init("GdImage", sizeof("GdImage")-1, 1);
	zend_declare_typed_property(class_entry, property_image_name, &property_image_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_image_class_GdImage, 0, 0));
	zend_string_release_ex(property_image_name, true);

	zval property_pageIndex_default_value;
	ZVAL_UNDEF(&property_pageIndex_default_value);
	zend_string *property_pageIndex_name = zend_string_init("pageIndex", sizeof("pageIndex") - 1, true);
	zend_declare_typed_property(class_entry, property_pageIndex_name, &property_pageIndex_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_pageIndex_name, true);

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

	zval property_samplesPerPixel_default_value;
	ZVAL_UNDEF(&property_samplesPerPixel_default_value);
	zend_string *property_samplesPerPixel_name = zend_string_init("samplesPerPixel", sizeof("samplesPerPixel") - 1, true);
	zend_declare_typed_property(class_entry, property_samplesPerPixel_name, &property_samplesPerPixel_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_samplesPerPixel_name, true);

	zval property_compressionTag_default_value;
	ZVAL_UNDEF(&property_compressionTag_default_value);
	zend_string *property_compressionTag_name = zend_string_init("compressionTag", sizeof("compressionTag") - 1, true);
	zend_declare_typed_property(class_entry, property_compressionTag_name, &property_compressionTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_compressionTag_name, true);

	zval property_compression_default_value;
	ZVAL_UNDEF(&property_compression_default_value);
	zend_string *property_compression_name = zend_string_init("compression", sizeof("compression") - 1, true);
	zend_string *property_compression_class_Gd_Tiff_Compression = zend_string_init("Gd\\Tiff\\Compression", sizeof("Gd\\Tiff\\Compression")-1, 1);
	zend_declare_typed_property(class_entry, property_compression_name, &property_compression_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_compression_class_Gd_Tiff_Compression, 0, MAY_BE_NULL));
	zend_string_release_ex(property_compression_name, true);

	zval property_photometricTag_default_value;
	ZVAL_UNDEF(&property_photometricTag_default_value);
	zend_string *property_photometricTag_name = zend_string_init("photometricTag", sizeof("photometricTag") - 1, true);
	zend_declare_typed_property(class_entry, property_photometricTag_name, &property_photometricTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_photometricTag_name, true);

	zval property_photometric_default_value;
	ZVAL_UNDEF(&property_photometric_default_value);
	zend_string *property_photometric_name = zend_string_init("photometric", sizeof("photometric") - 1, true);
	zend_string *property_photometric_class_Gd_Tiff_Photometric = zend_string_init("Gd\\Tiff\\Photometric", sizeof("Gd\\Tiff\\Photometric")-1, 1);
	zend_declare_typed_property(class_entry, property_photometric_name, &property_photometric_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_photometric_class_Gd_Tiff_Photometric, 0, MAY_BE_NULL));
	zend_string_release_ex(property_photometric_name, true);

	zval property_planarConfigurationTag_default_value;
	ZVAL_UNDEF(&property_planarConfigurationTag_default_value);
	zend_string *property_planarConfigurationTag_name = zend_string_init("planarConfigurationTag", sizeof("planarConfigurationTag") - 1, true);
	zend_declare_typed_property(class_entry, property_planarConfigurationTag_name, &property_planarConfigurationTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_planarConfigurationTag_name, true);

	zval property_planarConfiguration_default_value;
	ZVAL_UNDEF(&property_planarConfiguration_default_value);
	zend_string *property_planarConfiguration_name = zend_string_init("planarConfiguration", sizeof("planarConfiguration") - 1, true);
	zend_string *property_planarConfiguration_class_Gd_Tiff_PlanarConfiguration = zend_string_init("Gd\\Tiff\\PlanarConfiguration", sizeof("Gd\\Tiff\\PlanarConfiguration")-1, 1);
	zend_declare_typed_property(class_entry, property_planarConfiguration_name, &property_planarConfiguration_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_planarConfiguration_class_Gd_Tiff_PlanarConfiguration, 0, MAY_BE_NULL));
	zend_string_release_ex(property_planarConfiguration_name, true);

	zval property_minIsWhite_default_value;
	ZVAL_UNDEF(&property_minIsWhite_default_value);
	zend_string *property_minIsWhite_name = zend_string_init("minIsWhite", sizeof("minIsWhite") - 1, true);
	zend_declare_typed_property(class_entry, property_minIsWhite_name, &property_minIsWhite_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_minIsWhite_name, true);

	zval property_hasAlpha_default_value;
	ZVAL_UNDEF(&property_hasAlpha_default_value);
	zend_string *property_hasAlpha_name = zend_string_init("hasAlpha", sizeof("hasAlpha") - 1, true);
	zend_declare_typed_property(class_entry, property_hasAlpha_name, &property_hasAlpha_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_hasAlpha_name, true);

	zval property_isTiled_default_value;
	ZVAL_UNDEF(&property_isTiled_default_value);
	zend_string *property_isTiled_name = zend_string_init("isTiled", sizeof("isTiled") - 1, true);
	zend_declare_typed_property(class_entry, property_isTiled_name, &property_isTiled_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_isTiled_name, true);

	zval property_xResolution_default_value;
	ZVAL_UNDEF(&property_xResolution_default_value);
	zend_string *property_xResolution_name = zend_string_init("xResolution", sizeof("xResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_xResolution_name, &property_xResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE|MAY_BE_NULL));
	zend_string_release_ex(property_xResolution_name, true);

	zval property_yResolution_default_value;
	ZVAL_UNDEF(&property_yResolution_default_value);
	zend_string *property_yResolution_name = zend_string_init("yResolution", sizeof("yResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_yResolution_name, &property_yResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE|MAY_BE_NULL));
	zend_string_release_ex(property_yResolution_name, true);

	zval property_resolutionUnit_default_value;
	ZVAL_UNDEF(&property_resolutionUnit_default_value);
	zend_string *property_resolutionUnit_name = zend_string_init("resolutionUnit", sizeof("resolutionUnit") - 1, true);
	zend_string *property_resolutionUnit_class_Gd_Tiff_ResolutionUnit = zend_string_init("Gd\\Tiff\\ResolutionUnit", sizeof("Gd\\Tiff\\ResolutionUnit")-1, 1);
	zend_declare_typed_property(class_entry, property_resolutionUnit_name, &property_resolutionUnit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_resolutionUnit_class_Gd_Tiff_ResolutionUnit, 0, MAY_BE_NULL));
	zend_string_release_ex(property_resolutionUnit_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_TIFF_READ_API)
static zend_class_entry *register_class_Gd_Tiff_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Tiff", "Reader", class_Gd_Tiff_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif
