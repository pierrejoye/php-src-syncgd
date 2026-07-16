/* This is a generated file, edit gd_bmp.stub.php instead.
 * Stub hash: 665609a488c6abfaa80e68dc9c317413db031ad6
 * Has decl header: yes */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Bmp_ReadOptions___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Bmp_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, bitsPerPixel, IS_LONG, 0, "0")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, compression, Gd\\Bmp\\Compression, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quantize, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, forceV4Header, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, rgb555, _IS_BOOL, 0, "false")
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

ZEND_METHOD(Gd_Bmp_ReadOptions, __construct);
ZEND_METHOD(Gd_Bmp_WriteOptions, __construct);
ZEND_METHOD(Gd_Bmp_Codec, __construct);
ZEND_METHOD(Gd_Bmp_Codec, fromFile);
ZEND_METHOD(Gd_Bmp_Codec, fromString);
ZEND_METHOD(Gd_Bmp_Codec, fromStream);
ZEND_METHOD(Gd_Bmp_Codec, toFile);
ZEND_METHOD(Gd_Bmp_Codec, toStream);
ZEND_METHOD(Gd_Bmp_Codec, toString);

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

	return class_entry;
}

static zend_class_entry *register_class_Gd_Bmp_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Bmp", "Codec", class_Gd_Bmp_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
