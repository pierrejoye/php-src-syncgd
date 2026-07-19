/* This is a generated file, edit gd_qoi.stub.php instead.
 * Stub hash: a41e84f3539e5260245336af624222c6ae9bca94
 * Has decl header: yes */

#if defined(HAVE_GD_QOI)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Qoi_ReadOptions___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Qoi_Info___construct, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, channels, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, colorspaceTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, colorspace, Gd\\Qoi\\Colorspace, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Qoi_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, colorspace, Gd\\Qoi\\Colorspace, 0, "Gd\\Qoi\\Colorspace::SRGB")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Qoi_Reader___construct arginfo_class_Gd_Qoi_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Reader_fromFile, 0, 1, Gd\\Qoi\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\ReadOptions, 0, "new Gd\\Qoi\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Reader_fromStream, 0, 1, Gd\\Qoi\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\ReadOptions, 0, "new Gd\\Qoi\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Reader_fromString, 0, 1, Gd\\Qoi\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\ReadOptions, 0, "new Gd\\Qoi\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Reader_info, 0, 0, Gd\\Qoi\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Qoi_Codec___construct arginfo_class_Gd_Qoi_ReadOptions___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\ReadOptions, 0, "new Gd\\Qoi\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\ReadOptions, 0, "new Gd\\Qoi\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Qoi_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\ReadOptions, 0, "new Gd\\Qoi\\ReadOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Qoi_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\WriteOptions, 0, "new Gd\\Qoi\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Qoi_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\WriteOptions, 0, "new Gd\\Qoi\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Qoi_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Qoi\\WriteOptions, 0, "new Gd\\Qoi\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_QOI)
ZEND_METHOD(Gd_Qoi_ReadOptions, __construct);
ZEND_METHOD(Gd_Qoi_Info, __construct);
ZEND_METHOD(Gd_Qoi_WriteOptions, __construct);
ZEND_METHOD(Gd_Qoi_Reader, __construct);
ZEND_METHOD(Gd_Qoi_Reader, fromFile);
ZEND_METHOD(Gd_Qoi_Reader, fromStream);
ZEND_METHOD(Gd_Qoi_Reader, fromString);
ZEND_METHOD(Gd_Qoi_Reader, info);
ZEND_METHOD(Gd_Qoi_Reader, read);
ZEND_METHOD(Gd_Qoi_Codec, __construct);
ZEND_METHOD(Gd_Qoi_Codec, fromFile);
ZEND_METHOD(Gd_Qoi_Codec, fromStream);
ZEND_METHOD(Gd_Qoi_Codec, fromString);
ZEND_METHOD(Gd_Qoi_Codec, toFile);
ZEND_METHOD(Gd_Qoi_Codec, toStream);
ZEND_METHOD(Gd_Qoi_Codec, toString);
#endif

#if defined(HAVE_GD_QOI)
static const zend_function_entry class_Gd_Qoi_ReadOptions_methods[] = {
	ZEND_ME(Gd_Qoi_ReadOptions, __construct, arginfo_class_Gd_Qoi_ReadOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_QOI)
static const zend_function_entry class_Gd_Qoi_Info_methods[] = {
	ZEND_ME(Gd_Qoi_Info, __construct, arginfo_class_Gd_Qoi_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_QOI)
static const zend_function_entry class_Gd_Qoi_WriteOptions_methods[] = {
	ZEND_ME(Gd_Qoi_WriteOptions, __construct, arginfo_class_Gd_Qoi_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_QOI)
static const zend_function_entry class_Gd_Qoi_Reader_methods[] = {
	ZEND_ME(Gd_Qoi_Reader, __construct, arginfo_class_Gd_Qoi_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Qoi_Reader, fromFile, arginfo_class_Gd_Qoi_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Reader, fromStream, arginfo_class_Gd_Qoi_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Reader, fromString, arginfo_class_Gd_Qoi_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Reader, info, arginfo_class_Gd_Qoi_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Qoi_Reader, read, arginfo_class_Gd_Qoi_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_QOI)
static const zend_function_entry class_Gd_Qoi_Codec_methods[] = {
	ZEND_ME(Gd_Qoi_Codec, __construct, arginfo_class_Gd_Qoi_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Qoi_Codec, fromFile, arginfo_class_Gd_Qoi_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Codec, fromStream, arginfo_class_Gd_Qoi_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Codec, fromString, arginfo_class_Gd_Qoi_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Codec, toFile, arginfo_class_Gd_Qoi_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Codec, toStream, arginfo_class_Gd_Qoi_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Qoi_Codec, toString, arginfo_class_Gd_Qoi_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_QOI)
static zend_class_entry *register_class_Gd_Qoi_ReadOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Qoi", "ReadOptions", class_Gd_Qoi_ReadOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	return class_entry;
}
#endif

#if defined(HAVE_GD_QOI)
static zend_class_entry *register_class_Gd_Qoi_Colorspace(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Qoi\\Colorspace", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "SRGB", NULL);

	zend_enum_add_case_cstr(class_entry, "Linear", NULL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_QOI)
static zend_class_entry *register_class_Gd_Qoi_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Qoi", "Info", class_Gd_Qoi_Info_methods);
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

	zval property_channels_default_value;
	ZVAL_UNDEF(&property_channels_default_value);
	zend_string *property_channels_name = zend_string_init("channels", sizeof("channels") - 1, true);
	zend_declare_typed_property(class_entry, property_channels_name, &property_channels_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_channels_name, true);

	zval property_colorspaceTag_default_value;
	ZVAL_UNDEF(&property_colorspaceTag_default_value);
	zend_string *property_colorspaceTag_name = zend_string_init("colorspaceTag", sizeof("colorspaceTag") - 1, true);
	zend_declare_typed_property(class_entry, property_colorspaceTag_name, &property_colorspaceTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_colorspaceTag_name, true);

	zval property_colorspace_default_value;
	ZVAL_UNDEF(&property_colorspace_default_value);
	zend_string *property_colorspace_name = zend_string_init("colorspace", sizeof("colorspace") - 1, true);
	zend_string *property_colorspace_class_Gd_Qoi_Colorspace = zend_string_init("Gd\\Qoi\\Colorspace", sizeof("Gd\\Qoi\\Colorspace")-1, 1);
	zend_declare_typed_property(class_entry, property_colorspace_name, &property_colorspace_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_colorspace_class_Gd_Qoi_Colorspace, 0, 0));
	zend_string_release_ex(property_colorspace_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_QOI)
static zend_class_entry *register_class_Gd_Qoi_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Qoi", "WriteOptions", class_Gd_Qoi_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_colorspace_default_value;
	ZVAL_UNDEF(&property_colorspace_default_value);
	zend_string *property_colorspace_name = zend_string_init("colorspace", sizeof("colorspace") - 1, true);
	zend_string *property_colorspace_class_Gd_Qoi_Colorspace = zend_string_init("Gd\\Qoi\\Colorspace", sizeof("Gd\\Qoi\\Colorspace")-1, 1);
	zend_declare_typed_property(class_entry, property_colorspace_name, &property_colorspace_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_colorspace_class_Gd_Qoi_Colorspace, 0, 0));
	zend_string_release_ex(property_colorspace_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_QOI)
static zend_class_entry *register_class_Gd_Qoi_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Qoi", "Reader", class_Gd_Qoi_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_QOI)
static zend_class_entry *register_class_Gd_Qoi_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Qoi", "Codec", class_Gd_Qoi_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif
