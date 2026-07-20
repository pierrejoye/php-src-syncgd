/* This is a generated file, edit gd_webp.stub.php instead.
 * Stub hash: f42175d77aadb74c7207a8e33bff4838d173e552
 * Has decl header: yes */

#if defined(HAVE_GD_WEBP)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quality, IS_LONG, 0, "-1")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_Codec___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Webp\\WriteOptions, 0, "new Gd\\Webp\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Webp\\WriteOptions, 0, "new Gd\\Webp\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Webp\\WriteOptions, 0, "new Gd\\Webp\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_Info___construct, 0, 0, 8)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, frameCount, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, loopCount, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, backgroundColor, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, formatFlags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, isAnimated, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, metadata, Gd\\Metadata, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_BUNDLED)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_Reader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Reader_fromFile, 0, 1, Gd\\Webp\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Reader_fromString, 0, 1, Gd\\Webp\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Reader_fromStream, 0, 1, Gd\\Webp\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Reader_info, 0, 0, Gd\\Webp\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_Frame___construct, 0, 0, 14)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, frameIndex, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, durationMs, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timestampMs, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, disposeTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, dispose, Gd\\Webp\\DisposeMethod, 1)
	ZEND_ARG_TYPE_INFO(0, blendTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, blend, Gd\\Webp\\BlendMethod, 1)
	ZEND_ARG_TYPE_INFO(0, hasAlpha, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, complete, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_AnimReader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimReader_fromFile, 0, 1, Gd\\Webp\\AnimReader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimReader_fromString, 0, 1, Gd\\Webp\\AnimReader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimReader_fromStream, 0, 1, Gd\\Webp\\AnimReader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_AnimReader_isAnimatedFile, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_AnimReader_isAnimatedString, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_AnimReader_isAnimatedStream, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimReader_info, 0, 0, Gd\\Webp\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimReader_next, 0, 0, Gd\\Webp\\Frame, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_WEBP_ANIM_WRITE_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_AnimWriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, canvasWidth, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, canvasHeight, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, loopCount, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, backgroundColor, IS_LONG, 0, "0x0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, quality, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, lossless, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, method, IS_LONG, 0, "4")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, minimizeSize, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, kmin, IS_LONG, 0, "9")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, kmax, IS_LONG, 0, "17")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, allowMixed, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Webp_AnimWriter___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimWriter_toFile, 0, 1, Gd\\Webp\\AnimWriter, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Webp\\AnimWriteOptions, 0, "new Gd\\Webp\\AnimWriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimWriter_toStream, 0, 1, Gd\\Webp\\AnimWriter, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Webp\\AnimWriteOptions, 0, "new Gd\\Webp\\AnimWriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Webp_AnimWriter_toMemory, 0, 0, Gd\\Webp\\AnimWriter, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Webp\\AnimWriteOptions, 0, "new Gd\\Webp\\AnimWriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_AnimWriter_addFrame, 0, 2, IS_STATIC, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, durationMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Webp_AnimWriter_finish, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_WEBP)
ZEND_METHOD(Gd_Webp_WriteOptions, __construct);
ZEND_METHOD(Gd_Webp_Codec, __construct);
ZEND_METHOD(Gd_Webp_Codec, fromFile);
ZEND_METHOD(Gd_Webp_Codec, fromString);
ZEND_METHOD(Gd_Webp_Codec, fromStream);
ZEND_METHOD(Gd_Webp_Codec, toFile);
ZEND_METHOD(Gd_Webp_Codec, toStream);
ZEND_METHOD(Gd_Webp_Codec, toString);
#endif
#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
ZEND_METHOD(Gd_Webp_Info, __construct);
#endif
#if defined(HAVE_GD_BUNDLED)
ZEND_METHOD(Gd_Webp_Reader, __construct);
ZEND_METHOD(Gd_Webp_Reader, fromFile);
ZEND_METHOD(Gd_Webp_Reader, fromString);
ZEND_METHOD(Gd_Webp_Reader, fromStream);
ZEND_METHOD(Gd_Webp_Reader, info);
ZEND_METHOD(Gd_Webp_Reader, read);
#endif
#if defined(HAVE_GD_WEBP_ANIM_READ_API)
ZEND_METHOD(Gd_Webp_Frame, __construct);
ZEND_METHOD(Gd_Webp_AnimReader, __construct);
ZEND_METHOD(Gd_Webp_AnimReader, fromFile);
ZEND_METHOD(Gd_Webp_AnimReader, fromString);
ZEND_METHOD(Gd_Webp_AnimReader, fromStream);
ZEND_METHOD(Gd_Webp_AnimReader, isAnimatedFile);
ZEND_METHOD(Gd_Webp_AnimReader, isAnimatedString);
ZEND_METHOD(Gd_Webp_AnimReader, isAnimatedStream);
ZEND_METHOD(Gd_Webp_AnimReader, info);
ZEND_METHOD(Gd_Webp_AnimReader, next);
#endif
#if defined(HAVE_GD_WEBP_ANIM_WRITE_API)
ZEND_METHOD(Gd_Webp_AnimWriteOptions, __construct);
ZEND_METHOD(Gd_Webp_AnimWriter, __construct);
ZEND_METHOD(Gd_Webp_AnimWriter, toFile);
ZEND_METHOD(Gd_Webp_AnimWriter, toStream);
ZEND_METHOD(Gd_Webp_AnimWriter, toMemory);
ZEND_METHOD(Gd_Webp_AnimWriter, addFrame);
ZEND_METHOD(Gd_Webp_AnimWriter, finish);
#endif

#if defined(HAVE_GD_WEBP)
static const zend_function_entry class_Gd_Webp_WriteOptions_methods[] = {
	ZEND_ME(Gd_Webp_WriteOptions, __construct, arginfo_class_Gd_Webp_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_WEBP)
static const zend_function_entry class_Gd_Webp_Codec_methods[] = {
	ZEND_ME(Gd_Webp_Codec, __construct, arginfo_class_Gd_Webp_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Webp_Codec, fromFile, arginfo_class_Gd_Webp_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Codec, fromString, arginfo_class_Gd_Webp_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Codec, fromStream, arginfo_class_Gd_Webp_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Codec, toFile, arginfo_class_Gd_Webp_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Codec, toStream, arginfo_class_Gd_Webp_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Codec, toString, arginfo_class_Gd_Webp_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
static const zend_function_entry class_Gd_Webp_Info_methods[] = {
	ZEND_ME(Gd_Webp_Info, __construct, arginfo_class_Gd_Webp_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_BUNDLED)
static const zend_function_entry class_Gd_Webp_Reader_methods[] = {
	ZEND_ME(Gd_Webp_Reader, __construct, arginfo_class_Gd_Webp_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Webp_Reader, fromFile, arginfo_class_Gd_Webp_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Reader, fromString, arginfo_class_Gd_Webp_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Reader, fromStream, arginfo_class_Gd_Webp_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_Reader, info, arginfo_class_Gd_Webp_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Webp_Reader, read, arginfo_class_Gd_Webp_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API)
static const zend_function_entry class_Gd_Webp_Frame_methods[] = {
	ZEND_ME(Gd_Webp_Frame, __construct, arginfo_class_Gd_Webp_Frame___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API)
static const zend_function_entry class_Gd_Webp_AnimReader_methods[] = {
	ZEND_ME(Gd_Webp_AnimReader, __construct, arginfo_class_Gd_Webp_AnimReader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Webp_AnimReader, fromFile, arginfo_class_Gd_Webp_AnimReader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimReader, fromString, arginfo_class_Gd_Webp_AnimReader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimReader, fromStream, arginfo_class_Gd_Webp_AnimReader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimReader, isAnimatedFile, arginfo_class_Gd_Webp_AnimReader_isAnimatedFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimReader, isAnimatedString, arginfo_class_Gd_Webp_AnimReader_isAnimatedString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimReader, isAnimatedStream, arginfo_class_Gd_Webp_AnimReader_isAnimatedStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimReader, info, arginfo_class_Gd_Webp_AnimReader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Webp_AnimReader, next, arginfo_class_Gd_Webp_AnimReader_next, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static const zend_function_entry class_Gd_Webp_AnimWriteOptions_methods[] = {
	ZEND_ME(Gd_Webp_AnimWriteOptions, __construct, arginfo_class_Gd_Webp_AnimWriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static const zend_function_entry class_Gd_Webp_AnimWriter_methods[] = {
	ZEND_ME(Gd_Webp_AnimWriter, __construct, arginfo_class_Gd_Webp_AnimWriter___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Webp_AnimWriter, toFile, arginfo_class_Gd_Webp_AnimWriter_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimWriter, toStream, arginfo_class_Gd_Webp_AnimWriter_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimWriter, toMemory, arginfo_class_Gd_Webp_AnimWriter_toMemory, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Webp_AnimWriter, addFrame, arginfo_class_Gd_Webp_AnimWriter_addFrame, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Webp_AnimWriter, finish, arginfo_class_Gd_Webp_AnimWriter_finish, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_WEBP)
static zend_class_entry *register_class_Gd_Webp_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "WriteOptions", class_Gd_Webp_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_quality_default_value;
	ZVAL_UNDEF(&property_quality_default_value);
	zend_string *property_quality_name = zend_string_init("quality", sizeof("quality") - 1, true);
	zend_declare_typed_property(class_entry, property_quality_name, &property_quality_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_quality_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP)
static zend_class_entry *register_class_Gd_Webp_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "Codec", class_Gd_Webp_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API) || defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Webp_DisposeMethod(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Webp\\DisposeMethod", IS_LONG, NULL);

	zval enum_case_None_value;
	ZVAL_LONG(&enum_case_None_value, 0);
	zend_enum_add_case_cstr(class_entry, "None", &enum_case_None_value);

	zval enum_case_Background_value;
	ZVAL_LONG(&enum_case_Background_value, 1);
	zend_enum_add_case_cstr(class_entry, "Background", &enum_case_Background_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API) || defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Webp_BlendMethod(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Webp\\BlendMethod", IS_LONG, NULL);

	zval enum_case_Alpha_value;
	ZVAL_LONG(&enum_case_Alpha_value, 0);
	zend_enum_add_case_cstr(class_entry, "Alpha", &enum_case_Alpha_value);

	zval enum_case_None_value;
	ZVAL_LONG(&enum_case_None_value, 1);
	zend_enum_add_case_cstr(class_entry, "None", &enum_case_None_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Webp_FormatFlag(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Webp\\FormatFlag", IS_LONG, NULL);

	zval enum_case_Animation_value;
	ZVAL_LONG(&enum_case_Animation_value, 0x2);
	zend_enum_add_case_cstr(class_entry, "Animation", &enum_case_Animation_value);

	zval enum_case_Xmp_value;
	ZVAL_LONG(&enum_case_Xmp_value, 0x4);
	zend_enum_add_case_cstr(class_entry, "Xmp", &enum_case_Xmp_value);

	zval enum_case_Exif_value;
	ZVAL_LONG(&enum_case_Exif_value, 0x8);
	zend_enum_add_case_cstr(class_entry, "Exif", &enum_case_Exif_value);

	zval enum_case_Alpha_value;
	ZVAL_LONG(&enum_case_Alpha_value, 0x10);
	zend_enum_add_case_cstr(class_entry, "Alpha", &enum_case_Alpha_value);

	zval enum_case_Iccp_value;
	ZVAL_LONG(&enum_case_Iccp_value, 0x20);
	zend_enum_add_case_cstr(class_entry, "Iccp", &enum_case_Iccp_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Webp_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "Info", class_Gd_Webp_Info_methods);
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

	zval property_frameCount_default_value;
	ZVAL_UNDEF(&property_frameCount_default_value);
	zend_string *property_frameCount_name = zend_string_init("frameCount", sizeof("frameCount") - 1, true);
	zend_declare_typed_property(class_entry, property_frameCount_name, &property_frameCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_frameCount_name, true);

	zval property_loopCount_default_value;
	ZVAL_UNDEF(&property_loopCount_default_value);
	zend_string *property_loopCount_name = zend_string_init("loopCount", sizeof("loopCount") - 1, true);
	zend_declare_typed_property(class_entry, property_loopCount_name, &property_loopCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_loopCount_name, true);

	zval property_backgroundColor_default_value;
	ZVAL_UNDEF(&property_backgroundColor_default_value);
	zend_string *property_backgroundColor_name = zend_string_init("backgroundColor", sizeof("backgroundColor") - 1, true);
	zend_declare_typed_property(class_entry, property_backgroundColor_name, &property_backgroundColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_backgroundColor_name, true);

	zval property_formatFlags_default_value;
	ZVAL_UNDEF(&property_formatFlags_default_value);
	zend_string *property_formatFlags_name = zend_string_init("formatFlags", sizeof("formatFlags") - 1, true);
	zend_declare_typed_property(class_entry, property_formatFlags_name, &property_formatFlags_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_formatFlags_name, true);

	zval property_isAnimated_default_value;
	ZVAL_UNDEF(&property_isAnimated_default_value);
	zend_string *property_isAnimated_name = zend_string_init("isAnimated", sizeof("isAnimated") - 1, true);
	zend_declare_typed_property(class_entry, property_isAnimated_name, &property_isAnimated_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_isAnimated_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, 0));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_BUNDLED)
static zend_class_entry *register_class_Gd_Webp_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "Reader", class_Gd_Webp_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Webp_Frame(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "Frame", class_Gd_Webp_Frame_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_image_default_value;
	ZVAL_UNDEF(&property_image_default_value);
	zend_string *property_image_name = zend_string_init("image", sizeof("image") - 1, true);
	zend_string *property_image_class_GdImage = zend_string_init("GdImage", sizeof("GdImage")-1, 1);
	zend_declare_typed_property(class_entry, property_image_name, &property_image_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_image_class_GdImage, 0, 0));
	zend_string_release_ex(property_image_name, true);

	zval property_frameIndex_default_value;
	ZVAL_UNDEF(&property_frameIndex_default_value);
	zend_string *property_frameIndex_name = zend_string_init("frameIndex", sizeof("frameIndex") - 1, true);
	zend_declare_typed_property(class_entry, property_frameIndex_name, &property_frameIndex_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_frameIndex_name, true);

	zval property_x_default_value;
	ZVAL_UNDEF(&property_x_default_value);
	zend_string *property_x_name = zend_string_init("x", sizeof("x") - 1, true);
	zend_declare_typed_property(class_entry, property_x_name, &property_x_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_x_name, true);

	zval property_y_default_value;
	ZVAL_UNDEF(&property_y_default_value);
	zend_string *property_y_name = zend_string_init("y", sizeof("y") - 1, true);
	zend_declare_typed_property(class_entry, property_y_name, &property_y_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_y_name, true);

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

	zval property_durationMs_default_value;
	ZVAL_UNDEF(&property_durationMs_default_value);
	zend_string *property_durationMs_name = zend_string_init("durationMs", sizeof("durationMs") - 1, true);
	zend_declare_typed_property(class_entry, property_durationMs_name, &property_durationMs_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_durationMs_name, true);

	zval property_timestampMs_default_value;
	ZVAL_UNDEF(&property_timestampMs_default_value);
	zend_string *property_timestampMs_name = zend_string_init("timestampMs", sizeof("timestampMs") - 1, true);
	zend_declare_typed_property(class_entry, property_timestampMs_name, &property_timestampMs_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_timestampMs_name, true);

	zval property_disposeTag_default_value;
	ZVAL_UNDEF(&property_disposeTag_default_value);
	zend_string *property_disposeTag_name = zend_string_init("disposeTag", sizeof("disposeTag") - 1, true);
	zend_declare_typed_property(class_entry, property_disposeTag_name, &property_disposeTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_disposeTag_name, true);

	zval property_dispose_default_value;
	ZVAL_UNDEF(&property_dispose_default_value);
	zend_string *property_dispose_name = zend_string_init("dispose", sizeof("dispose") - 1, true);
	zend_string *property_dispose_class_Gd_Webp_DisposeMethod = zend_string_init("Gd\\Webp\\DisposeMethod", sizeof("Gd\\Webp\\DisposeMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_dispose_name, &property_dispose_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_dispose_class_Gd_Webp_DisposeMethod, 0, MAY_BE_NULL));
	zend_string_release_ex(property_dispose_name, true);

	zval property_blendTag_default_value;
	ZVAL_UNDEF(&property_blendTag_default_value);
	zend_string *property_blendTag_name = zend_string_init("blendTag", sizeof("blendTag") - 1, true);
	zend_declare_typed_property(class_entry, property_blendTag_name, &property_blendTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_blendTag_name, true);

	zval property_blend_default_value;
	ZVAL_UNDEF(&property_blend_default_value);
	zend_string *property_blend_name = zend_string_init("blend", sizeof("blend") - 1, true);
	zend_string *property_blend_class_Gd_Webp_BlendMethod = zend_string_init("Gd\\Webp\\BlendMethod", sizeof("Gd\\Webp\\BlendMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_blend_name, &property_blend_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_blend_class_Gd_Webp_BlendMethod, 0, MAY_BE_NULL));
	zend_string_release_ex(property_blend_name, true);

	zval property_hasAlpha_default_value;
	ZVAL_UNDEF(&property_hasAlpha_default_value);
	zend_string *property_hasAlpha_name = zend_string_init("hasAlpha", sizeof("hasAlpha") - 1, true);
	zend_declare_typed_property(class_entry, property_hasAlpha_name, &property_hasAlpha_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_hasAlpha_name, true);

	zval property_complete_default_value;
	ZVAL_UNDEF(&property_complete_default_value);
	zend_string *property_complete_name = zend_string_init("complete", sizeof("complete") - 1, true);
	zend_declare_typed_property(class_entry, property_complete_name, &property_complete_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_complete_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Webp_AnimReader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "AnimReader", class_Gd_Webp_AnimReader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Webp_AnimWriteOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "AnimWriteOptions", class_Gd_Webp_AnimWriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_canvasWidth_default_value;
	ZVAL_UNDEF(&property_canvasWidth_default_value);
	zend_string *property_canvasWidth_name = zend_string_init("canvasWidth", sizeof("canvasWidth") - 1, true);
	zend_declare_typed_property(class_entry, property_canvasWidth_name, &property_canvasWidth_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_canvasWidth_name, true);

	zval property_canvasHeight_default_value;
	ZVAL_UNDEF(&property_canvasHeight_default_value);
	zend_string *property_canvasHeight_name = zend_string_init("canvasHeight", sizeof("canvasHeight") - 1, true);
	zend_declare_typed_property(class_entry, property_canvasHeight_name, &property_canvasHeight_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_canvasHeight_name, true);

	zval property_loopCount_default_value;
	ZVAL_UNDEF(&property_loopCount_default_value);
	zend_string *property_loopCount_name = zend_string_init("loopCount", sizeof("loopCount") - 1, true);
	zend_declare_typed_property(class_entry, property_loopCount_name, &property_loopCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_loopCount_name, true);

	zval property_backgroundColor_default_value;
	ZVAL_UNDEF(&property_backgroundColor_default_value);
	zend_string *property_backgroundColor_name = zend_string_init("backgroundColor", sizeof("backgroundColor") - 1, true);
	zend_declare_typed_property(class_entry, property_backgroundColor_name, &property_backgroundColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_backgroundColor_name, true);

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

	zval property_method_default_value;
	ZVAL_UNDEF(&property_method_default_value);
	zend_string *property_method_name = zend_string_init("method", sizeof("method") - 1, true);
	zend_declare_typed_property(class_entry, property_method_name, &property_method_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_method_name, true);

	zval property_minimizeSize_default_value;
	ZVAL_UNDEF(&property_minimizeSize_default_value);
	zend_string *property_minimizeSize_name = zend_string_init("minimizeSize", sizeof("minimizeSize") - 1, true);
	zend_declare_typed_property(class_entry, property_minimizeSize_name, &property_minimizeSize_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_minimizeSize_name, true);

	zval property_kmin_default_value;
	ZVAL_UNDEF(&property_kmin_default_value);
	zend_string *property_kmin_name = zend_string_init("kmin", sizeof("kmin") - 1, true);
	zend_declare_typed_property(class_entry, property_kmin_name, &property_kmin_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_kmin_name, true);

	zval property_kmax_default_value;
	ZVAL_UNDEF(&property_kmax_default_value);
	zend_string *property_kmax_name = zend_string_init("kmax", sizeof("kmax") - 1, true);
	zend_declare_typed_property(class_entry, property_kmax_name, &property_kmax_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_kmax_name, true);

	zval property_allowMixed_default_value;
	ZVAL_UNDEF(&property_allowMixed_default_value);
	zend_string *property_allowMixed_name = zend_string_init("allowMixed", sizeof("allowMixed") - 1, true);
	zend_declare_typed_property(class_entry, property_allowMixed_name, &property_allowMixed_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_allowMixed_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_WEBP_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Webp_AnimWriter(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Webp", "AnimWriter", class_Gd_Webp_AnimWriter_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif
