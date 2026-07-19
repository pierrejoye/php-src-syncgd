/* This is a generated file, edit gd_text.stub.php instead.
 * Stub hash: b3b2e3b0624c1574e357da971dc2767b02df5e8b
 * Has decl header: yes */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_FontFace___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_FontFace_fromFile, 0, 1, Gd\\FontFace, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, faceIndex, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_FontFace_fromString, 0, 1, Gd\\FontFace, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, faceIndex, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_TextOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, shaping, Gd\\TextShaping, 0, "Gd\\TextShaping::None")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, lineSpacing, IS_DOUBLE, 0, "1.0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_TextExtents___construct, 0, 0, 6)
	ZEND_ARG_TYPE_INFO(0, xBearing, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, yBearing, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, xAdvance, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, yAdvance, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setFontFace, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, font, Gd\\FontFace, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setFontSize, 0, 1, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_textPath, 0, 3, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\TextOptions, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_showText arginfo_class_Gd_Context_textPath

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_textExtents, 0, 1, Gd\\TextExtents, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\TextOptions, 1, "null")
ZEND_END_ARG_INFO()

ZEND_METHOD(Gd_FontFace, __construct);
ZEND_METHOD(Gd_FontFace, fromFile);
ZEND_METHOD(Gd_FontFace, fromString);
ZEND_METHOD(Gd_TextOptions, __construct);
ZEND_METHOD(Gd_TextExtents, __construct);
ZEND_METHOD(Gd_Context, setFontFace);
ZEND_METHOD(Gd_Context, setFontSize);
ZEND_METHOD(Gd_Context, textPath);
ZEND_METHOD(Gd_Context, showText);
ZEND_METHOD(Gd_Context, textExtents);

static const zend_function_entry class_Gd_FontFace_methods[] = {
	ZEND_ME(Gd_FontFace, __construct, arginfo_class_Gd_FontFace___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_FontFace, fromFile, arginfo_class_Gd_FontFace_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_FontFace, fromString, arginfo_class_Gd_FontFace_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_TextOptions_methods[] = {
	ZEND_ME(Gd_TextOptions, __construct, arginfo_class_Gd_TextOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_TextExtents_methods[] = {
	ZEND_ME(Gd_TextExtents, __construct, arginfo_class_Gd_TextExtents___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Context_methods[] = {
	ZEND_ME(Gd_Context, setFontFace, arginfo_class_Gd_Context_setFontFace, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setFontSize, arginfo_class_Gd_Context_setFontSize, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, textPath, arginfo_class_Gd_Context_textPath, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, showText, arginfo_class_Gd_Context_showText, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, textExtents, arginfo_class_Gd_Context_textExtents, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Gd_Text_TextException(zend_class_entry *class_entry_Gd_GdException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Text", "TextException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_GdException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Text_InvalidTextException(zend_class_entry *class_entry_Gd_Text_TextException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Text", "InvalidTextException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Text_TextException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Text_UnavailableException(zend_class_entry *class_entry_Gd_Text_TextException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Text", "UnavailableException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Text_TextException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Text_FontException(zend_class_entry *class_entry_Gd_Text_TextException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Text", "FontException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Text_TextException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Text_LayoutException(zend_class_entry *class_entry_Gd_Text_TextException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Text", "LayoutException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Text_TextException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Text_MemoryException(zend_class_entry *class_entry_Gd_Text_TextException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Text", "MemoryException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Text_TextException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_TextShaping(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\TextShaping", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "None", NULL);

	zend_enum_add_case_cstr(class_entry, "Raqm", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_FontFace(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "FontFace", class_Gd_FontFace_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_TextOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "TextOptions", class_Gd_TextOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_shaping_default_value;
	ZVAL_UNDEF(&property_shaping_default_value);
	zend_string *property_shaping_name = zend_string_init("shaping", sizeof("shaping") - 1, true);
	zend_string *property_shaping_class_Gd_TextShaping = zend_string_init("Gd\\TextShaping", sizeof("Gd\\TextShaping")-1, 1);
	zend_declare_typed_property(class_entry, property_shaping_name, &property_shaping_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_shaping_class_Gd_TextShaping, 0, 0));
	zend_string_release_ex(property_shaping_name, true);

	zval property_lineSpacing_default_value;
	ZVAL_UNDEF(&property_lineSpacing_default_value);
	zend_string *property_lineSpacing_name = zend_string_init("lineSpacing", sizeof("lineSpacing") - 1, true);
	zend_declare_typed_property(class_entry, property_lineSpacing_name, &property_lineSpacing_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_lineSpacing_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_TextExtents(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "TextExtents", class_Gd_TextExtents_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_xBearing_default_value;
	ZVAL_UNDEF(&property_xBearing_default_value);
	zend_string *property_xBearing_name = zend_string_init("xBearing", sizeof("xBearing") - 1, true);
	zend_declare_typed_property(class_entry, property_xBearing_name, &property_xBearing_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_xBearing_name, true);

	zval property_yBearing_default_value;
	ZVAL_UNDEF(&property_yBearing_default_value);
	zend_string *property_yBearing_name = zend_string_init("yBearing", sizeof("yBearing") - 1, true);
	zend_declare_typed_property(class_entry, property_yBearing_name, &property_yBearing_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_yBearing_name, true);

	zval property_width_default_value;
	ZVAL_UNDEF(&property_width_default_value);
	zend_string *property_width_name = zend_string_init("width", sizeof("width") - 1, true);
	zend_declare_typed_property(class_entry, property_width_name, &property_width_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_width_name, true);

	zval property_height_default_value;
	ZVAL_UNDEF(&property_height_default_value);
	zend_string *property_height_name = zend_string_init("height", sizeof("height") - 1, true);
	zend_declare_typed_property(class_entry, property_height_name, &property_height_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_height_name, true);

	zval property_xAdvance_default_value;
	ZVAL_UNDEF(&property_xAdvance_default_value);
	zend_string *property_xAdvance_name = zend_string_init("xAdvance", sizeof("xAdvance") - 1, true);
	zend_declare_typed_property(class_entry, property_xAdvance_name, &property_xAdvance_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_xAdvance_name, true);

	zval property_yAdvance_default_value;
	ZVAL_UNDEF(&property_yAdvance_default_value);
	zend_string *property_yAdvance_name = zend_string_init("yAdvance", sizeof("yAdvance") - 1, true);
	zend_declare_typed_property(class_entry, property_yAdvance_name, &property_yAdvance_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_yAdvance_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Context(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Context", class_Gd_Context_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
