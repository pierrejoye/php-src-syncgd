/* This is a generated file, edit gd_gif.stub.php instead.
 * Stub hash: aa533b9cbba92785fafdaa455ca800f58697dec5
 * Has decl header: yes */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_Codec___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Gif\\WriteOptions, 0, "new Gd\\Gif\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Gif\\WriteOptions, 0, "new Gd\\Gif\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Gif\\WriteOptions, 0, "new Gd\\Gif\\WriteOptions()")
ZEND_END_ARG_INFO()

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_GIF_ANIM_READ_API) && defined(HAVE_GD_BUNDLED)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_Info___construct, 0, 0, 8)
	ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, backgroundIndex, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, globalColorTable, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, colorResolution, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, pixelAspectRatio, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, loopCount, IS_LONG, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_BUNDLED)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_Reader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Reader_fromFile, 0, 1, Gd\\Gif\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Reader_fromString, 0, 1, Gd\\Gif\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Reader_fromStream, 0, 1, Gd\\Gif\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Reader_info, 0, 0, Gd\\Gif\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_GIF_ANIM_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_Frame___construct, 0, 0, 12)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, frameIndex, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, delayMs, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, disposalTag, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, disposal, Gd\\Gif\\DisposalMethod, 1)
	ZEND_ARG_TYPE_INFO(0, transparentIndex, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, localColorTable, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, interlaced, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_AnimReader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimReader_fromFile, 0, 1, Gd\\Gif\\AnimReader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimReader_fromString, 0, 1, Gd\\Gif\\AnimReader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimReader_fromStream, 0, 1, Gd\\Gif\\AnimReader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_AnimReader_isAnimatedFile, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_AnimReader_isAnimatedString, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_AnimReader_isAnimatedStream, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimReader_info, 0, 0, Gd\\Gif\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimReader_next, 0, 0, Gd\\Gif\\Frame, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_GIF_ANIM_WRITE_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_FrameOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, x, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, y, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, delayMs, IS_LONG, 0, "0")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, disposal, Gd\\Gif\\DisposalMethod, 0, "Gd\\Gif\\DisposalMethod::None")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, localColorMap, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, optimizeAgainstPrevious, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Gif_AnimWriter___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimWriter_toFile, 0, 1, Gd\\Gif\\AnimWriter, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, globalColorMap, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, loops, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimWriter_toStream, 0, 1, Gd\\Gif\\AnimWriter, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, globalColorMap, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, loops, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gif_AnimWriter_toMemory, 0, 0, Gd\\Gif\\AnimWriter, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, globalColorMap, _IS_BOOL, 0, "true")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, loops, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_AnimWriter_addFrame, 0, 1, IS_STATIC, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Gif\\FrameOptions, 0, "new Gd\\Gif\\FrameOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Gif_AnimWriter_finish, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()
#endif

ZEND_METHOD(Gd_Gif_WriteOptions, __construct);
ZEND_METHOD(Gd_Gif_Codec, __construct);
ZEND_METHOD(Gd_Gif_Codec, fromFile);
ZEND_METHOD(Gd_Gif_Codec, fromString);
ZEND_METHOD(Gd_Gif_Codec, fromStream);
ZEND_METHOD(Gd_Gif_Codec, toFile);
ZEND_METHOD(Gd_Gif_Codec, toStream);
ZEND_METHOD(Gd_Gif_Codec, toString);
#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_GIF_ANIM_READ_API) && defined(HAVE_GD_BUNDLED)
ZEND_METHOD(Gd_Gif_Info, __construct);
#endif
#if defined(HAVE_GD_BUNDLED)
ZEND_METHOD(Gd_Gif_Reader, __construct);
ZEND_METHOD(Gd_Gif_Reader, fromFile);
ZEND_METHOD(Gd_Gif_Reader, fromString);
ZEND_METHOD(Gd_Gif_Reader, fromStream);
ZEND_METHOD(Gd_Gif_Reader, info);
ZEND_METHOD(Gd_Gif_Reader, read);
#endif
#if defined(HAVE_GD_GIF_ANIM_READ_API)
ZEND_METHOD(Gd_Gif_Frame, __construct);
ZEND_METHOD(Gd_Gif_AnimReader, __construct);
ZEND_METHOD(Gd_Gif_AnimReader, fromFile);
ZEND_METHOD(Gd_Gif_AnimReader, fromString);
ZEND_METHOD(Gd_Gif_AnimReader, fromStream);
ZEND_METHOD(Gd_Gif_AnimReader, isAnimatedFile);
ZEND_METHOD(Gd_Gif_AnimReader, isAnimatedString);
ZEND_METHOD(Gd_Gif_AnimReader, isAnimatedStream);
ZEND_METHOD(Gd_Gif_AnimReader, info);
ZEND_METHOD(Gd_Gif_AnimReader, next);
#endif
#if defined(HAVE_GD_GIF_ANIM_WRITE_API)
ZEND_METHOD(Gd_Gif_FrameOptions, __construct);
ZEND_METHOD(Gd_Gif_AnimWriter, __construct);
ZEND_METHOD(Gd_Gif_AnimWriter, toFile);
ZEND_METHOD(Gd_Gif_AnimWriter, toStream);
ZEND_METHOD(Gd_Gif_AnimWriter, toMemory);
ZEND_METHOD(Gd_Gif_AnimWriter, addFrame);
ZEND_METHOD(Gd_Gif_AnimWriter, finish);
#endif

static const zend_function_entry class_Gd_Gif_WriteOptions_methods[] = {
	ZEND_ME(Gd_Gif_WriteOptions, __construct, arginfo_class_Gd_Gif_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Gif_Codec_methods[] = {
	ZEND_ME(Gd_Gif_Codec, __construct, arginfo_class_Gd_Gif_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Gif_Codec, fromFile, arginfo_class_Gd_Gif_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Codec, fromString, arginfo_class_Gd_Gif_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Codec, fromStream, arginfo_class_Gd_Gif_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Codec, toFile, arginfo_class_Gd_Gif_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Codec, toStream, arginfo_class_Gd_Gif_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Codec, toString, arginfo_class_Gd_Gif_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_GIF_ANIM_READ_API) && defined(HAVE_GD_BUNDLED)
static const zend_function_entry class_Gd_Gif_Info_methods[] = {
	ZEND_ME(Gd_Gif_Info, __construct, arginfo_class_Gd_Gif_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_BUNDLED)
static const zend_function_entry class_Gd_Gif_Reader_methods[] = {
	ZEND_ME(Gd_Gif_Reader, __construct, arginfo_class_Gd_Gif_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Gif_Reader, fromFile, arginfo_class_Gd_Gif_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Reader, fromString, arginfo_class_Gd_Gif_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Reader, fromStream, arginfo_class_Gd_Gif_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_Reader, info, arginfo_class_Gd_Gif_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Gif_Reader, read, arginfo_class_Gd_Gif_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_GIF_ANIM_READ_API)
static const zend_function_entry class_Gd_Gif_Frame_methods[] = {
	ZEND_ME(Gd_Gif_Frame, __construct, arginfo_class_Gd_Gif_Frame___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_GIF_ANIM_READ_API)
static const zend_function_entry class_Gd_Gif_AnimReader_methods[] = {
	ZEND_ME(Gd_Gif_AnimReader, __construct, arginfo_class_Gd_Gif_AnimReader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Gif_AnimReader, fromFile, arginfo_class_Gd_Gif_AnimReader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimReader, fromString, arginfo_class_Gd_Gif_AnimReader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimReader, fromStream, arginfo_class_Gd_Gif_AnimReader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimReader, isAnimatedFile, arginfo_class_Gd_Gif_AnimReader_isAnimatedFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimReader, isAnimatedString, arginfo_class_Gd_Gif_AnimReader_isAnimatedString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimReader, isAnimatedStream, arginfo_class_Gd_Gif_AnimReader_isAnimatedStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimReader, info, arginfo_class_Gd_Gif_AnimReader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Gif_AnimReader, next, arginfo_class_Gd_Gif_AnimReader_next, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_GIF_ANIM_WRITE_API)
static const zend_function_entry class_Gd_Gif_FrameOptions_methods[] = {
	ZEND_ME(Gd_Gif_FrameOptions, __construct, arginfo_class_Gd_Gif_FrameOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_GIF_ANIM_WRITE_API)
static const zend_function_entry class_Gd_Gif_AnimWriter_methods[] = {
	ZEND_ME(Gd_Gif_AnimWriter, __construct, arginfo_class_Gd_Gif_AnimWriter___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Gif_AnimWriter, toFile, arginfo_class_Gd_Gif_AnimWriter_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimWriter, toStream, arginfo_class_Gd_Gif_AnimWriter_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimWriter, toMemory, arginfo_class_Gd_Gif_AnimWriter_toMemory, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Gif_AnimWriter, addFrame, arginfo_class_Gd_Gif_AnimWriter_addFrame, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Gif_AnimWriter, finish, arginfo_class_Gd_Gif_AnimWriter_finish, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

static zend_class_entry *register_class_Gd_Gif_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "WriteOptions", class_Gd_Gif_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Gif_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "Codec", class_Gd_Gif_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

#if defined(HAVE_GD_GIF_ANIM_READ_API) || defined(HAVE_GD_GIF_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Gif_DisposalMethod(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Gif\\DisposalMethod", IS_LONG, NULL);

	zval enum_case_Unknown_value;
	ZVAL_LONG(&enum_case_Unknown_value, 0);
	zend_enum_add_case_cstr(class_entry, "Unknown", &enum_case_Unknown_value);

	zval enum_case_None_value;
	ZVAL_LONG(&enum_case_None_value, 1);
	zend_enum_add_case_cstr(class_entry, "None", &enum_case_None_value);

	zval enum_case_RestoreBackground_value;
	ZVAL_LONG(&enum_case_RestoreBackground_value, 2);
	zend_enum_add_case_cstr(class_entry, "RestoreBackground", &enum_case_RestoreBackground_value);

	zval enum_case_RestorePrevious_value;
	ZVAL_LONG(&enum_case_RestorePrevious_value, 3);
	zend_enum_add_case_cstr(class_entry, "RestorePrevious", &enum_case_RestorePrevious_value);

	return class_entry;
}
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_GIF_ANIM_READ_API) && defined(HAVE_GD_BUNDLED)
static zend_class_entry *register_class_Gd_Gif_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "Info", class_Gd_Gif_Info_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_version_default_value;
	ZVAL_UNDEF(&property_version_default_value);
	zend_string *property_version_name = zend_string_init("version", sizeof("version") - 1, true);
	zend_declare_typed_property(class_entry, property_version_name, &property_version_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
	zend_string_release_ex(property_version_name, true);

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

	zval property_backgroundIndex_default_value;
	ZVAL_UNDEF(&property_backgroundIndex_default_value);
	zend_string *property_backgroundIndex_name = zend_string_init("backgroundIndex", sizeof("backgroundIndex") - 1, true);
	zend_declare_typed_property(class_entry, property_backgroundIndex_name, &property_backgroundIndex_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_backgroundIndex_name, true);

	zval property_globalColorTable_default_value;
	ZVAL_UNDEF(&property_globalColorTable_default_value);
	zend_string *property_globalColorTable_name = zend_string_init("globalColorTable", sizeof("globalColorTable") - 1, true);
	zend_declare_typed_property(class_entry, property_globalColorTable_name, &property_globalColorTable_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_globalColorTable_name, true);

	zval property_colorResolution_default_value;
	ZVAL_UNDEF(&property_colorResolution_default_value);
	zend_string *property_colorResolution_name = zend_string_init("colorResolution", sizeof("colorResolution") - 1, true);
	zend_declare_typed_property(class_entry, property_colorResolution_name, &property_colorResolution_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_colorResolution_name, true);

	zval property_pixelAspectRatio_default_value;
	ZVAL_UNDEF(&property_pixelAspectRatio_default_value);
	zend_string *property_pixelAspectRatio_name = zend_string_init("pixelAspectRatio", sizeof("pixelAspectRatio") - 1, true);
	zend_declare_typed_property(class_entry, property_pixelAspectRatio_name, &property_pixelAspectRatio_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_pixelAspectRatio_name, true);

	zval property_loopCount_default_value;
	ZVAL_UNDEF(&property_loopCount_default_value);
	zend_string *property_loopCount_name = zend_string_init("loopCount", sizeof("loopCount") - 1, true);
	zend_declare_typed_property(class_entry, property_loopCount_name, &property_loopCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_loopCount_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_BUNDLED)
static zend_class_entry *register_class_Gd_Gif_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "Reader", class_Gd_Gif_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_GIF_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Gif_Frame(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "Frame", class_Gd_Gif_Frame_methods);
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

	zval property_delayMs_default_value;
	ZVAL_UNDEF(&property_delayMs_default_value);
	zend_string *property_delayMs_name = zend_string_init("delayMs", sizeof("delayMs") - 1, true);
	zend_declare_typed_property(class_entry, property_delayMs_name, &property_delayMs_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_delayMs_name, true);

	zval property_disposalTag_default_value;
	ZVAL_UNDEF(&property_disposalTag_default_value);
	zend_string *property_disposalTag_name = zend_string_init("disposalTag", sizeof("disposalTag") - 1, true);
	zend_declare_typed_property(class_entry, property_disposalTag_name, &property_disposalTag_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_disposalTag_name, true);

	zval property_disposal_default_value;
	ZVAL_UNDEF(&property_disposal_default_value);
	zend_string *property_disposal_name = zend_string_init("disposal", sizeof("disposal") - 1, true);
	zend_string *property_disposal_class_Gd_Gif_DisposalMethod = zend_string_init("Gd\\Gif\\DisposalMethod", sizeof("Gd\\Gif\\DisposalMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_disposal_name, &property_disposal_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_disposal_class_Gd_Gif_DisposalMethod, 0, MAY_BE_NULL));
	zend_string_release_ex(property_disposal_name, true);

	zval property_transparentIndex_default_value;
	ZVAL_UNDEF(&property_transparentIndex_default_value);
	zend_string *property_transparentIndex_name = zend_string_init("transparentIndex", sizeof("transparentIndex") - 1, true);
	zend_declare_typed_property(class_entry, property_transparentIndex_name, &property_transparentIndex_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_transparentIndex_name, true);

	zval property_localColorTable_default_value;
	ZVAL_UNDEF(&property_localColorTable_default_value);
	zend_string *property_localColorTable_name = zend_string_init("localColorTable", sizeof("localColorTable") - 1, true);
	zend_declare_typed_property(class_entry, property_localColorTable_name, &property_localColorTable_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_localColorTable_name, true);

	zval property_interlaced_default_value;
	ZVAL_UNDEF(&property_interlaced_default_value);
	zend_string *property_interlaced_name = zend_string_init("interlaced", sizeof("interlaced") - 1, true);
	zend_declare_typed_property(class_entry, property_interlaced_name, &property_interlaced_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_interlaced_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_GIF_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Gif_AnimReader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "AnimReader", class_Gd_Gif_AnimReader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_GIF_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Gif_FrameOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "FrameOptions", class_Gd_Gif_FrameOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

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

	zval property_delayMs_default_value;
	ZVAL_UNDEF(&property_delayMs_default_value);
	zend_string *property_delayMs_name = zend_string_init("delayMs", sizeof("delayMs") - 1, true);
	zend_declare_typed_property(class_entry, property_delayMs_name, &property_delayMs_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_delayMs_name, true);

	zval property_disposal_default_value;
	ZVAL_UNDEF(&property_disposal_default_value);
	zend_string *property_disposal_name = zend_string_init("disposal", sizeof("disposal") - 1, true);
	zend_string *property_disposal_class_Gd_Gif_DisposalMethod = zend_string_init("Gd\\Gif\\DisposalMethod", sizeof("Gd\\Gif\\DisposalMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_disposal_name, &property_disposal_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_disposal_class_Gd_Gif_DisposalMethod, 0, 0));
	zend_string_release_ex(property_disposal_name, true);

	zval property_localColorMap_default_value;
	ZVAL_UNDEF(&property_localColorMap_default_value);
	zend_string *property_localColorMap_name = zend_string_init("localColorMap", sizeof("localColorMap") - 1, true);
	zend_declare_typed_property(class_entry, property_localColorMap_name, &property_localColorMap_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_localColorMap_name, true);

	zval property_optimizeAgainstPrevious_default_value;
	ZVAL_UNDEF(&property_optimizeAgainstPrevious_default_value);
	zend_string *property_optimizeAgainstPrevious_name = zend_string_init("optimizeAgainstPrevious", sizeof("optimizeAgainstPrevious") - 1, true);
	zend_declare_typed_property(class_entry, property_optimizeAgainstPrevious_name, &property_optimizeAgainstPrevious_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_optimizeAgainstPrevious_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_GIF_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Gif_AnimWriter(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Gif", "AnimWriter", class_Gd_Gif_AnimWriter_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif
