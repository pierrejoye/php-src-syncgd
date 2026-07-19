/* This is a generated file, edit gd_avif.stub.php instead.
 * Stub hash: 77478f9469ad6ac04e4a743a3e414b01a2821d79
 * Has decl header: yes */

#if defined(HAVE_GD_AVIF)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Avif_ReadOptions___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Avif_Info___construct, 0, 0, 10)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, isAnimation, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, isProgressive, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, frameCount, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, duration, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, hasAlpha, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, bitDepth, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, yuvFormat, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, metadata, Gd\\Metadata, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Avif_Reader___construct arginfo_class_Gd_Avif_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Reader_fromFile, 0, 1, Gd\\Avif\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Reader_fromString, 0, 1, Gd\\Avif\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Reader_fromStream, 0, 1, Gd\\Avif\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Reader_info, 0, 0, Gd\\Avif\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Avif_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quality, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, speed, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, lossless, _IS_BOOL, 0, "false")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, chromaSubsampling, Gd\\Avif\\ChromaSubsampling, 1, "null")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Avif_Codec___construct arginfo_class_Gd_Avif_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Avif\\ReadOptions, 0, "new Gd\\Avif\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Avif\\ReadOptions, 0, "new Gd\\Avif\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Avif_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Avif\\ReadOptions, 0, "new Gd\\Avif\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Avif_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Avif\\WriteOptions, 0, "new Gd\\Avif\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Avif_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Avif\\WriteOptions, 0, "new Gd\\Avif\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Avif_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Avif\\WriteOptions, 0, "new Gd\\Avif\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_AVIF)
ZEND_METHOD(Gd_Avif_ReadOptions, __construct);
ZEND_METHOD(Gd_Avif_Info, __construct);
ZEND_METHOD(Gd_Avif_Reader, __construct);
ZEND_METHOD(Gd_Avif_Reader, fromFile);
ZEND_METHOD(Gd_Avif_Reader, fromString);
ZEND_METHOD(Gd_Avif_Reader, fromStream);
ZEND_METHOD(Gd_Avif_Reader, info);
ZEND_METHOD(Gd_Avif_Reader, read);
ZEND_METHOD(Gd_Avif_WriteOptions, __construct);
ZEND_METHOD(Gd_Avif_Codec, __construct);
ZEND_METHOD(Gd_Avif_Codec, fromFile);
ZEND_METHOD(Gd_Avif_Codec, fromString);
ZEND_METHOD(Gd_Avif_Codec, fromStream);
ZEND_METHOD(Gd_Avif_Codec, toFile);
ZEND_METHOD(Gd_Avif_Codec, toStream);
ZEND_METHOD(Gd_Avif_Codec, toString);
#endif

#if defined(HAVE_GD_AVIF)
static const zend_function_entry class_Gd_Avif_ReadOptions_methods[] = {
	ZEND_ME(Gd_Avif_ReadOptions, __construct, arginfo_class_Gd_Avif_ReadOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_AVIF)
static const zend_function_entry class_Gd_Avif_Info_methods[] = {
	ZEND_ME(Gd_Avif_Info, __construct, arginfo_class_Gd_Avif_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_AVIF)
static const zend_function_entry class_Gd_Avif_Reader_methods[] = {
	ZEND_ME(Gd_Avif_Reader, __construct, arginfo_class_Gd_Avif_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Avif_Reader, fromFile, arginfo_class_Gd_Avif_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Reader, fromString, arginfo_class_Gd_Avif_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Reader, fromStream, arginfo_class_Gd_Avif_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Reader, info, arginfo_class_Gd_Avif_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Avif_Reader, read, arginfo_class_Gd_Avif_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_AVIF)
static const zend_function_entry class_Gd_Avif_WriteOptions_methods[] = {
	ZEND_ME(Gd_Avif_WriteOptions, __construct, arginfo_class_Gd_Avif_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_AVIF)
static const zend_function_entry class_Gd_Avif_Codec_methods[] = {
	ZEND_ME(Gd_Avif_Codec, __construct, arginfo_class_Gd_Avif_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Avif_Codec, fromFile, arginfo_class_Gd_Avif_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Codec, fromString, arginfo_class_Gd_Avif_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Codec, fromStream, arginfo_class_Gd_Avif_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Codec, toFile, arginfo_class_Gd_Avif_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Codec, toStream, arginfo_class_Gd_Avif_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Avif_Codec, toString, arginfo_class_Gd_Avif_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_AVIF)
static zend_class_entry *register_class_Gd_Avif_ChromaSubsampling(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Avif\\ChromaSubsampling", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Yuv420", NULL);

	zend_enum_add_case_cstr(class_entry, "Yuv444", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_AVIF)
static zend_class_entry *register_class_Gd_Avif_ReadOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Avif", "ReadOptions", class_Gd_Avif_ReadOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	return class_entry;
}
#endif

#if defined(HAVE_GD_AVIF)
static zend_class_entry *register_class_Gd_Avif_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Avif", "Info", class_Gd_Avif_Info_methods);
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

	zval property_isAnimation_default_value;
	ZVAL_UNDEF(&property_isAnimation_default_value);
	zend_string *property_isAnimation_name = zend_string_init("isAnimation", sizeof("isAnimation") - 1, true);
	zend_declare_typed_property(class_entry, property_isAnimation_name, &property_isAnimation_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_isAnimation_name, true);

	zval property_isProgressive_default_value;
	ZVAL_UNDEF(&property_isProgressive_default_value);
	zend_string *property_isProgressive_name = zend_string_init("isProgressive", sizeof("isProgressive") - 1, true);
	zend_declare_typed_property(class_entry, property_isProgressive_name, &property_isProgressive_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_isProgressive_name, true);

	zval property_frameCount_default_value;
	ZVAL_UNDEF(&property_frameCount_default_value);
	zend_string *property_frameCount_name = zend_string_init("frameCount", sizeof("frameCount") - 1, true);
	zend_declare_typed_property(class_entry, property_frameCount_name, &property_frameCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_frameCount_name, true);

	zval property_duration_default_value;
	ZVAL_UNDEF(&property_duration_default_value);
	zend_string *property_duration_name = zend_string_init("duration", sizeof("duration") - 1, true);
	zend_declare_typed_property(class_entry, property_duration_name, &property_duration_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_duration_name, true);

	zval property_hasAlpha_default_value;
	ZVAL_UNDEF(&property_hasAlpha_default_value);
	zend_string *property_hasAlpha_name = zend_string_init("hasAlpha", sizeof("hasAlpha") - 1, true);
	zend_declare_typed_property(class_entry, property_hasAlpha_name, &property_hasAlpha_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_hasAlpha_name, true);

	zval property_bitDepth_default_value;
	ZVAL_UNDEF(&property_bitDepth_default_value);
	zend_string *property_bitDepth_name = zend_string_init("bitDepth", sizeof("bitDepth") - 1, true);
	zend_declare_typed_property(class_entry, property_bitDepth_name, &property_bitDepth_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_bitDepth_name, true);

	zval property_yuvFormat_default_value;
	ZVAL_UNDEF(&property_yuvFormat_default_value);
	zend_string *property_yuvFormat_name = zend_string_init("yuvFormat", sizeof("yuvFormat") - 1, true);
	zend_declare_typed_property(class_entry, property_yuvFormat_name, &property_yuvFormat_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_yuvFormat_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, 0));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_AVIF)
static zend_class_entry *register_class_Gd_Avif_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Avif", "Reader", class_Gd_Avif_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_AVIF)
static zend_class_entry *register_class_Gd_Avif_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Avif", "WriteOptions", class_Gd_Avif_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_quality_default_value;
	ZVAL_UNDEF(&property_quality_default_value);
	zend_string *property_quality_name = zend_string_init("quality", sizeof("quality") - 1, true);
	zend_declare_typed_property(class_entry, property_quality_name, &property_quality_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_quality_name, true);

	zval property_speed_default_value;
	ZVAL_UNDEF(&property_speed_default_value);
	zend_string *property_speed_name = zend_string_init("speed", sizeof("speed") - 1, true);
	zend_declare_typed_property(class_entry, property_speed_name, &property_speed_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_speed_name, true);

	zval property_lossless_default_value;
	ZVAL_UNDEF(&property_lossless_default_value);
	zend_string *property_lossless_name = zend_string_init("lossless", sizeof("lossless") - 1, true);
	zend_declare_typed_property(class_entry, property_lossless_name, &property_lossless_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_lossless_name, true);

	zval property_chromaSubsampling_default_value;
	ZVAL_UNDEF(&property_chromaSubsampling_default_value);
	zend_string *property_chromaSubsampling_name = zend_string_init("chromaSubsampling", sizeof("chromaSubsampling") - 1, true);
	zend_string *property_chromaSubsampling_class_Gd_Avif_ChromaSubsampling = zend_string_init("Gd\\Avif\\ChromaSubsampling", sizeof("Gd\\Avif\\ChromaSubsampling")-1, 1);
	zend_declare_typed_property(class_entry, property_chromaSubsampling_name, &property_chromaSubsampling_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_chromaSubsampling_class_Gd_Avif_ChromaSubsampling, 0, MAY_BE_NULL));
	zend_string_release_ex(property_chromaSubsampling_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_AVIF)
static zend_class_entry *register_class_Gd_Avif_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Avif", "Codec", class_Gd_Avif_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif
