/* This is a generated file, edit gd_jxl.stub.php instead.
 * Stub hash: a8e8750b9ab17f5cd99085c0b3f289eb2540e739 */

#if defined(HAVE_GD_JXL_CODEC)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_WriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, lossless, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, distance, IS_DOUBLE, 0, "1.0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, effort, IS_LONG, 0, "7")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_Codec___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Codec_fromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Codec_fromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Codec_fromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jxl_Codec_toFile, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jxl\\WriteOptions, 0, "new Gd\\Jxl\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jxl_Codec_toStream, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jxl\\WriteOptions, 0, "new Gd\\Jxl\\WriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jxl_Codec_toString, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jxl\\WriteOptions, 0, "new Gd\\Jxl\\WriteOptions()")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_Info___construct, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, animated, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, loopCount, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, metadata, Gd\\Metadata, 1, "null")
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_JXL_ANIM_READ_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_Frame___construct, 0, 0, 4)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, frameIndex, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, delayMs, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timestampMs, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_AnimReader___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimReader_fromFile, 0, 1, Gd\\Jxl\\AnimReader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimReader_fromString, 0, 1, Gd\\Jxl\\AnimReader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimReader_fromStream, 0, 1, Gd\\Jxl\\AnimReader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimReader_info, 0, 0, Gd\\Jxl\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimReader_next, 0, 0, Gd\\Jxl\\Frame, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_JXL_CODEC)
#define arginfo_class_Gd_Jxl_Reader___construct arginfo_class_Gd_Jxl_Codec___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Reader_fromFile, 0, 1, Gd\\Jxl\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Reader_fromString, 0, 1, Gd\\Jxl\\Reader, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Reader_fromStream, 0, 1, Gd\\Jxl\\Reader, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Reader_info, 0, 0, Gd\\Jxl\\Info, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_Reader_read, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_JXL_ANIM_WRITE_API)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_AnimWriteOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, canvasWidth, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, canvasHeight, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, loopCount, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, lossless, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, distance, IS_DOUBLE, 0, "1.0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, effort, IS_LONG, 0, "7")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Jxl_AnimWriter___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimWriter_toFile, 0, 1, Gd\\Jxl\\AnimWriter, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jxl\\AnimWriteOptions, 0, "new Gd\\Jxl\\AnimWriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimWriter_toStream, 0, 1, Gd\\Jxl\\AnimWriter, 0)
	ZEND_ARG_INFO(0, stream)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jxl\\AnimWriteOptions, 0, "new Gd\\Jxl\\AnimWriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Jxl_AnimWriter_toMemory, 0, 0, Gd\\Jxl\\AnimWriter, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, Gd\\Jxl\\AnimWriteOptions, 0, "new Gd\\Jxl\\AnimWriteOptions()")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jxl_AnimWriter_addFrame, 0, 1, IS_STATIC, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, delayMs, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Jxl_AnimWriter_finish, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_GD_JXL_CODEC)
ZEND_METHOD(Gd_Jxl_WriteOptions, __construct);
ZEND_METHOD(Gd_Jxl_Codec, __construct);
ZEND_METHOD(Gd_Jxl_Codec, fromFile);
ZEND_METHOD(Gd_Jxl_Codec, fromString);
ZEND_METHOD(Gd_Jxl_Codec, fromStream);
ZEND_METHOD(Gd_Jxl_Codec, toFile);
ZEND_METHOD(Gd_Jxl_Codec, toStream);
ZEND_METHOD(Gd_Jxl_Codec, toString);
#endif
#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API)
ZEND_METHOD(Gd_Jxl_Info, __construct);
#endif
#if defined(HAVE_GD_JXL_ANIM_READ_API)
ZEND_METHOD(Gd_Jxl_Frame, __construct);
ZEND_METHOD(Gd_Jxl_AnimReader, __construct);
ZEND_METHOD(Gd_Jxl_AnimReader, fromFile);
ZEND_METHOD(Gd_Jxl_AnimReader, fromString);
ZEND_METHOD(Gd_Jxl_AnimReader, fromStream);
ZEND_METHOD(Gd_Jxl_AnimReader, info);
ZEND_METHOD(Gd_Jxl_AnimReader, next);
#endif
#if defined(HAVE_GD_JXL_CODEC)
ZEND_METHOD(Gd_Jxl_Reader, __construct);
ZEND_METHOD(Gd_Jxl_Reader, fromFile);
ZEND_METHOD(Gd_Jxl_Reader, fromString);
ZEND_METHOD(Gd_Jxl_Reader, fromStream);
ZEND_METHOD(Gd_Jxl_Reader, info);
ZEND_METHOD(Gd_Jxl_Reader, read);
#endif
#if defined(HAVE_GD_JXL_ANIM_WRITE_API)
ZEND_METHOD(Gd_Jxl_AnimWriteOptions, __construct);
ZEND_METHOD(Gd_Jxl_AnimWriter, __construct);
ZEND_METHOD(Gd_Jxl_AnimWriter, toFile);
ZEND_METHOD(Gd_Jxl_AnimWriter, toStream);
ZEND_METHOD(Gd_Jxl_AnimWriter, toMemory);
ZEND_METHOD(Gd_Jxl_AnimWriter, addFrame);
ZEND_METHOD(Gd_Jxl_AnimWriter, finish);
#endif

#if defined(HAVE_GD_JXL_CODEC)
static const zend_function_entry class_Gd_Jxl_WriteOptions_methods[] = {
	ZEND_ME(Gd_Jxl_WriteOptions, __construct, arginfo_class_Gd_Jxl_WriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_CODEC)
static const zend_function_entry class_Gd_Jxl_Codec_methods[] = {
	ZEND_ME(Gd_Jxl_Codec, __construct, arginfo_class_Gd_Jxl_Codec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Jxl_Codec, fromFile, arginfo_class_Gd_Jxl_Codec_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Codec, fromString, arginfo_class_Gd_Jxl_Codec_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Codec, fromStream, arginfo_class_Gd_Jxl_Codec_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Codec, toFile, arginfo_class_Gd_Jxl_Codec_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Codec, toStream, arginfo_class_Gd_Jxl_Codec_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Codec, toString, arginfo_class_Gd_Jxl_Codec_toString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API)
static const zend_function_entry class_Gd_Jxl_Info_methods[] = {
	ZEND_ME(Gd_Jxl_Info, __construct, arginfo_class_Gd_Jxl_Info___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_ANIM_READ_API)
static const zend_function_entry class_Gd_Jxl_Frame_methods[] = {
	ZEND_ME(Gd_Jxl_Frame, __construct, arginfo_class_Gd_Jxl_Frame___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_ANIM_READ_API)
static const zend_function_entry class_Gd_Jxl_AnimReader_methods[] = {
	ZEND_ME(Gd_Jxl_AnimReader, __construct, arginfo_class_Gd_Jxl_AnimReader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Jxl_AnimReader, fromFile, arginfo_class_Gd_Jxl_AnimReader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_AnimReader, fromString, arginfo_class_Gd_Jxl_AnimReader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_AnimReader, fromStream, arginfo_class_Gd_Jxl_AnimReader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_AnimReader, info, arginfo_class_Gd_Jxl_AnimReader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Jxl_AnimReader, next, arginfo_class_Gd_Jxl_AnimReader_next, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_CODEC)
static const zend_function_entry class_Gd_Jxl_Reader_methods[] = {
	ZEND_ME(Gd_Jxl_Reader, __construct, arginfo_class_Gd_Jxl_Reader___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Jxl_Reader, fromFile, arginfo_class_Gd_Jxl_Reader_fromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Reader, fromString, arginfo_class_Gd_Jxl_Reader_fromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Reader, fromStream, arginfo_class_Gd_Jxl_Reader_fromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_Reader, info, arginfo_class_Gd_Jxl_Reader_info, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Jxl_Reader, read, arginfo_class_Gd_Jxl_Reader_read, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_ANIM_WRITE_API)
static const zend_function_entry class_Gd_Jxl_AnimWriteOptions_methods[] = {
	ZEND_ME(Gd_Jxl_AnimWriteOptions, __construct, arginfo_class_Gd_Jxl_AnimWriteOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_ANIM_WRITE_API)
static const zend_function_entry class_Gd_Jxl_AnimWriter_methods[] = {
	ZEND_ME(Gd_Jxl_AnimWriter, __construct, arginfo_class_Gd_Jxl_AnimWriter___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Jxl_AnimWriter, toFile, arginfo_class_Gd_Jxl_AnimWriter_toFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_AnimWriter, toStream, arginfo_class_Gd_Jxl_AnimWriter_toStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_AnimWriter, toMemory, arginfo_class_Gd_Jxl_AnimWriter_toMemory, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Jxl_AnimWriter, addFrame, arginfo_class_Gd_Jxl_AnimWriter_addFrame, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Jxl_AnimWriter, finish, arginfo_class_Gd_Jxl_AnimWriter_finish, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

#if defined(HAVE_GD_JXL_CODEC)
static zend_class_entry *register_class_Gd_Jxl_WriteOptions(zend_class_entry *class_entry_Gd_Codec_WriteOptions)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "WriteOptions", class_Gd_Jxl_WriteOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);
	zend_class_implements(class_entry, 1, class_entry_Gd_Codec_WriteOptions);

	zval property_lossless_default_value;
	ZVAL_UNDEF(&property_lossless_default_value);
	zend_string *property_lossless_name = zend_string_init("lossless", sizeof("lossless") - 1, true);
	zend_declare_typed_property(class_entry, property_lossless_name, &property_lossless_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_lossless_name, true);

	zval property_distance_default_value;
	ZVAL_UNDEF(&property_distance_default_value);
	zend_string *property_distance_name = zend_string_init("distance", sizeof("distance") - 1, true);
	zend_declare_typed_property(class_entry, property_distance_name, &property_distance_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_distance_name, true);

	zval property_effort_default_value;
	ZVAL_UNDEF(&property_effort_default_value);
	zend_string *property_effort_name = zend_string_init("effort", sizeof("effort") - 1, true);
	zend_declare_typed_property(class_entry, property_effort_name, &property_effort_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_effort_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_CODEC)
static zend_class_entry *register_class_Gd_Jxl_Codec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "Codec", class_Gd_Jxl_Codec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Jxl_Info(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "Info", class_Gd_Jxl_Info_methods);
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

	zval property_animated_default_value;
	ZVAL_UNDEF(&property_animated_default_value);
	zend_string *property_animated_name = zend_string_init("animated", sizeof("animated") - 1, true);
	zend_declare_typed_property(class_entry, property_animated_name, &property_animated_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_animated_name, true);

	zval property_loopCount_default_value;
	ZVAL_UNDEF(&property_loopCount_default_value);
	zend_string *property_loopCount_name = zend_string_init("loopCount", sizeof("loopCount") - 1, true);
	zend_declare_typed_property(class_entry, property_loopCount_name, &property_loopCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_loopCount_name, true);

	zval property_metadata_default_value;
	ZVAL_UNDEF(&property_metadata_default_value);
	zend_string *property_metadata_name = zend_string_init("metadata", sizeof("metadata") - 1, true);
	zend_string *property_metadata_class_Gd_Metadata = zend_string_init("Gd\\Metadata", sizeof("Gd\\Metadata")-1, 1);
	zend_declare_typed_property(class_entry, property_metadata_name, &property_metadata_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_metadata_class_Gd_Metadata, 0, MAY_BE_NULL));
	zend_string_release_ex(property_metadata_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Jxl_Frame(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "Frame", class_Gd_Jxl_Frame_methods);
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

	zval property_delayMs_default_value;
	ZVAL_UNDEF(&property_delayMs_default_value);
	zend_string *property_delayMs_name = zend_string_init("delayMs", sizeof("delayMs") - 1, true);
	zend_declare_typed_property(class_entry, property_delayMs_name, &property_delayMs_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_delayMs_name, true);

	zval property_timestampMs_default_value;
	ZVAL_UNDEF(&property_timestampMs_default_value);
	zend_string *property_timestampMs_name = zend_string_init("timestampMs", sizeof("timestampMs") - 1, true);
	zend_declare_typed_property(class_entry, property_timestampMs_name, &property_timestampMs_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_timestampMs_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_ANIM_READ_API)
static zend_class_entry *register_class_Gd_Jxl_AnimReader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "AnimReader", class_Gd_Jxl_AnimReader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_CODEC)
static zend_class_entry *register_class_Gd_Jxl_Reader(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "Reader", class_Gd_Jxl_Reader_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Jxl_AnimWriteOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "AnimWriteOptions", class_Gd_Jxl_AnimWriteOptions_methods);
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

	zval property_lossless_default_value;
	ZVAL_UNDEF(&property_lossless_default_value);
	zend_string *property_lossless_name = zend_string_init("lossless", sizeof("lossless") - 1, true);
	zend_declare_typed_property(class_entry, property_lossless_name, &property_lossless_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release_ex(property_lossless_name, true);

	zval property_distance_default_value;
	ZVAL_UNDEF(&property_distance_default_value);
	zend_string *property_distance_name = zend_string_init("distance", sizeof("distance") - 1, true);
	zend_declare_typed_property(class_entry, property_distance_name, &property_distance_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_distance_name, true);

	zval property_effort_default_value;
	ZVAL_UNDEF(&property_effort_default_value);
	zend_string *property_effort_name = zend_string_init("effort", sizeof("effort") - 1, true);
	zend_declare_typed_property(class_entry, property_effort_name, &property_effort_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_effort_name, true);

	return class_entry;
}
#endif

#if defined(HAVE_GD_JXL_ANIM_WRITE_API)
static zend_class_entry *register_class_Gd_Jxl_AnimWriter(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Jxl", "AnimWriter", class_Gd_Jxl_AnimWriter_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
#endif
