/* This is a generated file, edit gd_heif.stub.php instead.
 * Stub hash: f6b7d38dcfd845dfce83ff91696516eba56cd9d7
 * Has decl header: yes */

#if defined(HAVE_GD_HEIF)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Heif_ReadOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, ignoreTransformations, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Heif_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quality, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, lossless, _IS_BOOL, 0, "false")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, codec, Gd\\Heif\\CompressionFormat, 0, "Gd\\Heif\\CompressionFormat::Hevc")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, chromaSubsampling, Gd\\Heif\\ChromaSubsampling, 0, "Gd\\Heif\\ChromaSubsampling::Yuv444")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Heif_Codec___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Heif_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Heif\\ReadOptions, 0, "new Gd\\Heif\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Heif_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Heif\\ReadOptions, 0, "new Gd\\Heif\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Heif_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Heif\\ReadOptions, 0, "new Gd\\Heif\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Heif_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Heif\\WriteOptions, 0, "new Gd\\Heif\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Heif_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Heif\\WriteOptions, 0, "new Gd\\Heif\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Heif_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Heif\\WriteOptions, 0, "new Gd\\Heif\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_HEIF)
ZEND_METHOD(Gd_Heif_ReadOptions, __construct);
ZEND_METHOD(Gd_Heif_WriteOptions, __construct);
ZEND_METHOD(Gd_Heif_Codec, __construct);
ZEND_METHOD(Gd_Heif_Codec, fromFile);
ZEND_METHOD(Gd_Heif_Codec, fromString);
ZEND_METHOD(Gd_Heif_Codec, fromStream);
ZEND_METHOD(Gd_Heif_Codec, toFile);
ZEND_METHOD(Gd_Heif_Codec, toStream);
ZEND_METHOD(Gd_Heif_Codec, toString);
#endif

#if defined(HAVE_GD_HEIF)
static const zend_function_entry class_Gd_Heif_ReadOptions_methods[] = {
	ZEND_ME(Gd_Heif_ReadOptions, __construct, arginfo_class_Gd_Heif_ReadOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_HEIF)
static const zend_function_entry class_Gd_Heif_WriteOptions_methods[] = {
	ZEND_ME(Gd_Heif_WriteOptions, __construct, arginfo_class_Gd_Heif_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_HEIF)
static const zend_function_entry class_Gd_Heif_Codec_methods[] = {
	ZEND_ME(Gd_Heif_Codec, __construct, arginfo_class_Gd_Heif_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Heif_Codec, fromFile, arginfo_class_Gd_Heif_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Heif_Codec, fromString, arginfo_class_Gd_Heif_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Heif_Codec, fromStream, arginfo_class_Gd_Heif_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Heif_Codec, toFile, arginfo_class_Gd_Heif_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Heif_Codec, toStream, arginfo_class_Gd_Heif_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Heif_Codec, toString, arginfo_class_Gd_Heif_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_HEIF)
static zend_class_entry *register_class_Gd_Heif_CompressionFormat(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Heif\\CompressionFormat", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Hevc", NULL);

	zend_enum_add_case_cstr(class_entry, "Av1", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_HEIF)
static zend_class_entry *register_class_Gd_Heif_ChromaSubsampling(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Heif\\ChromaSubsampling", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Yuv420", NULL);

	zend_enum_add_case_cstr(class_entry, "Yuv422", NULL);

	zend_enum_add_case_cstr(class_entry, "Yuv444", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_HEIF)
static zend_class_entry *register_class_Gd_Heif_ReadOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Heif", "ReadOptions", class_Gd_Heif_ReadOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_ignoreTransformations_default_value;
	ZVAL_UNDEF(&property_ignoreTransformations_default_value);
	zend_string *property_ignoreTransformations_name = zend_string_init("ignoreTransformations", sizeof("ignoreTransformations") - 1, true);
	zend_declare_typed_property(class_entry, property_ignoreTransformations_name, &property_ignoreTransformations_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_ignoreTransformations_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_HEIF)
static zend_class_entry *register_class_Gd_Heif_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Heif", "WriteOptions", class_Gd_Heif_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_quality_default_value;
	ZVAL_UNDEF(&property_quality_default_value);
	zend_string *property_quality_name = zend_string_init("quality", sizeof("quality") - 1, true);
	zend_declare_typed_property(class_entry, property_quality_name, &property_quality_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_quality_name, true);

	zval property_lossless_default_value;
	ZVAL_UNDEF(&property_lossless_default_value);
	zend_string *property_lossless_name = zend_string_init("lossless", sizeof("lossless") - 1, true);
	zend_declare_typed_property(class_entry, property_lossless_name, &property_lossless_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_lossless_name, true);

	zval property_codec_default_value;
	ZVAL_UNDEF(&property_codec_default_value);
	zend_string *property_codec_name = zend_string_init("codec", sizeof("codec") - 1, true);
	zend_string *property_codec_class_Gd_Heif_CompressionFormat = zend_string_init("Gd\\Heif\\CompressionFormat", sizeof("Gd\\Heif\\CompressionFormat")-1, 1);
	zend_declare_typed_property(class_entry, property_codec_name, &property_codec_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_codec_class_Gd_Heif_CompressionFormat, 0, 0));
	zend_string_release_ex(property_codec_name, true);

	zval property_chromaSubsampling_default_value;
	ZVAL_UNDEF(&property_chromaSubsampling_default_value);
	zend_string *property_chromaSubsampling_name = zend_string_init("chromaSubsampling", sizeof("chromaSubsampling") - 1, true);
	zend_string *property_chromaSubsampling_class_Gd_Heif_ChromaSubsampling = zend_string_init("Gd\\Heif\\ChromaSubsampling", sizeof("Gd\\Heif\\ChromaSubsampling")-1, 1);
	zend_declare_typed_property(class_entry, property_chromaSubsampling_name, &property_chromaSubsampling_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_chromaSubsampling_class_Gd_Heif_ChromaSubsampling, 0, 0));
	zend_string_release_ex(property_chromaSubsampling_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_HEIF)
static zend_class_entry *register_class_Gd_Heif_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Heif", "Codec", class_Gd_Heif_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif
