/* This is a generated file, edit gd_image.stub.php instead.
 * Stub hash: fac7cbd9060d32dd2e9cf0b8accabe3fb83dc49e
 * Has decl header: yes */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_PerceptualDiffOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, mode, Gd\\PerceptualDiffMode, 0, "Gd\\PerceptualDiffMode::None")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, highlightColor, IS_LONG, 0, "0xff0000")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_PerceptualDiffResult___construct, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, pixelsChanged, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, maximumDelta, IS_DOUBLE, 0)
	ZEND_ARG_OBJ_INFO(0, diffImage, GdImage, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_RotateOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, backgroundColor, IS_LONG, 0, "0x7f000000")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_ScaleOptions___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, width, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, height, IS_LONG, 1, "null")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, fit, Gd\\ScaleFit, 0, "Gd\\ScaleFit::Cover")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, gravity, Gd\\ScaleGravity, 0, "Gd\\ScaleGravity::Center")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, strategy, Gd\\ScaleStrategy, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, backgroundColor, IS_LONG, 0, "0x7f000000")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, interpolation, Gd\\InterpolationMethod, 1, "null")
ZEND_END_ARG_INFO()

#if defined(HAVE_GD_BUNDLED)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_TransformOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, clip, Gd\\Rect, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, backgroundColor, IS_LONG, 0, "0x7f000000")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, interpolation, Gd\\InterpolationMethod, 1, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_AutoCropOptions___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, mode, Gd\\AutoCropMode, 0, "Gd\\AutoCropMode::Default")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, threshold, IS_DOUBLE, 0, "0.5")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, color, IS_LONG, 0, "-1")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Image___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Image_create, 0, 2, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, colorModel, Gd\\ColorModel, 0, "Gd\\ColorModel::TrueColor")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Image_loadFromFile, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Image_loadFromStream, 0, 1, GdImage, 0)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Image_loadFromString, 0, 1, GdImage, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Gd_PerceptualDiffOptions, __construct);
ZEND_METHOD(Gd_PerceptualDiffResult, __construct);
ZEND_METHOD(Gd_RotateOptions, __construct);
ZEND_METHOD(Gd_ScaleOptions, __construct);
#if defined(HAVE_GD_BUNDLED)
ZEND_METHOD(Gd_TransformOptions, __construct);
#endif
ZEND_METHOD(Gd_AutoCropOptions, __construct);
ZEND_METHOD(Gd_Image, __construct);
ZEND_METHOD(Gd_Image, create);
ZEND_METHOD(Gd_Image, loadFromFile);
ZEND_METHOD(Gd_Image, loadFromStream);
ZEND_METHOD(Gd_Image, loadFromString);

static const zend_function_entry class_Gd_PerceptualDiffOptions_methods[] = {
	ZEND_ME(Gd_PerceptualDiffOptions, __construct, arginfo_class_Gd_PerceptualDiffOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_PerceptualDiffResult_methods[] = {
	ZEND_ME(Gd_PerceptualDiffResult, __construct, arginfo_class_Gd_PerceptualDiffResult___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_RotateOptions_methods[] = {
	ZEND_ME(Gd_RotateOptions, __construct, arginfo_class_Gd_RotateOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_ScaleOptions_methods[] = {
	ZEND_ME(Gd_ScaleOptions, __construct, arginfo_class_Gd_ScaleOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

#if defined(HAVE_GD_BUNDLED)
static const zend_function_entry class_Gd_TransformOptions_methods[] = {
	ZEND_ME(Gd_TransformOptions, __construct, arginfo_class_Gd_TransformOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
#endif

static const zend_function_entry class_Gd_AutoCropOptions_methods[] = {
	ZEND_ME(Gd_AutoCropOptions, __construct, arginfo_class_Gd_AutoCropOptions___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Image_methods[] = {
	ZEND_ME(Gd_Image, __construct, arginfo_class_Gd_Image___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Image, create, arginfo_class_Gd_Image_create, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Image, loadFromFile, arginfo_class_Gd_Image_loadFromFile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Image, loadFromStream, arginfo_class_Gd_Image_loadFromStream, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Image, loadFromString, arginfo_class_Gd_Image_loadFromString, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Gd_Codec_Format(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\Codec\\Format", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Png", NULL);

	zend_enum_add_case_cstr(class_entry, "Jpeg", NULL);

	zend_enum_add_case_cstr(class_entry, "Webp", NULL);

	zend_enum_add_case_cstr(class_entry, "Bmp", NULL);

	zend_enum_add_case_cstr(class_entry, "Avif", NULL);

	zend_enum_add_case_cstr(class_entry, "Heif", NULL);

	zend_enum_add_case_cstr(class_entry, "Jxl", NULL);

	zend_enum_add_case_cstr(class_entry, "Tiff", NULL);

	zend_enum_add_case_cstr(class_entry, "Gif", NULL);

	zend_enum_add_case_cstr(class_entry, "Qoi", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Codec_WriteOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Codec", "WriteOptions", NULL);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

static zend_class_entry *register_class_Gd_ColorModel(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\ColorModel", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "TrueColor", NULL);

	zend_enum_add_case_cstr(class_entry, "Palette", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_PerceptualDiffMode(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\PerceptualDiffMode", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "None", NULL);

	zend_enum_add_case_cstr(class_entry, "Overlay", NULL);

	zend_enum_add_case_cstr(class_entry, "Mask", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_ScaleFit(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\ScaleFit", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Cover", NULL);

	zend_enum_add_case_cstr(class_entry, "Contain", NULL);

	zend_enum_add_case_cstr(class_entry, "Fill", NULL);

	zend_enum_add_case_cstr(class_entry, "Inside", NULL);

	zend_enum_add_case_cstr(class_entry, "Outside", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_ScaleGravity(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\ScaleGravity", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Northwest", NULL);

	zend_enum_add_case_cstr(class_entry, "North", NULL);

	zend_enum_add_case_cstr(class_entry, "Northeast", NULL);

	zend_enum_add_case_cstr(class_entry, "West", NULL);

	zend_enum_add_case_cstr(class_entry, "Center", NULL);

	zend_enum_add_case_cstr(class_entry, "East", NULL);

	zend_enum_add_case_cstr(class_entry, "Southwest", NULL);

	zend_enum_add_case_cstr(class_entry, "South", NULL);

	zend_enum_add_case_cstr(class_entry, "Southeast", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_ScaleStrategy(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\ScaleStrategy", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Entropy", NULL);

	zend_enum_add_case_cstr(class_entry, "Attention", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_InterpolationMethod(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\InterpolationMethod", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Default", NULL);

	zend_enum_add_case_cstr(class_entry, "Bell", NULL);

	zend_enum_add_case_cstr(class_entry, "Bessel", NULL);

	zend_enum_add_case_cstr(class_entry, "Linear", NULL);

	zend_enum_add_case_cstr(class_entry, "CubicSpline", NULL);

	zend_enum_add_case_cstr(class_entry, "Blackman", NULL);

	zend_enum_add_case_cstr(class_entry, "Box", NULL);

	zend_enum_add_case_cstr(class_entry, "BSpline", NULL);

	zend_enum_add_case_cstr(class_entry, "CatmullRom", NULL);

	zend_enum_add_case_cstr(class_entry, "Gaussian", NULL);

	zend_enum_add_case_cstr(class_entry, "GeneralizedCubic", NULL);

	zend_enum_add_case_cstr(class_entry, "Hermite", NULL);

	zend_enum_add_case_cstr(class_entry, "Hamming", NULL);

	zend_enum_add_case_cstr(class_entry, "Hanning", NULL);

	zend_enum_add_case_cstr(class_entry, "Mitchell", NULL);

	zend_enum_add_case_cstr(class_entry, "NearestNeighbour", NULL);

	zend_enum_add_case_cstr(class_entry, "Power", NULL);

	zend_enum_add_case_cstr(class_entry, "Quadratic", NULL);

	zend_enum_add_case_cstr(class_entry, "Sinc", NULL);

	zend_enum_add_case_cstr(class_entry, "Triangle", NULL);

	zend_enum_add_case_cstr(class_entry, "Weighted4", NULL);

	zend_enum_add_case_cstr(class_entry, "Lanczos3", NULL);

	zend_enum_add_case_cstr(class_entry, "Lanczos8", NULL);

	zend_enum_add_case_cstr(class_entry, "BlackmanBessel", NULL);

	zend_enum_add_case_cstr(class_entry, "BlackmanSinc", NULL);

	zend_enum_add_case_cstr(class_entry, "QuadraticBSpline", NULL);

	zend_enum_add_case_cstr(class_entry, "Cosine", NULL);

	zend_enum_add_case_cstr(class_entry, "Welsh", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_AutoCropMode(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\AutoCropMode", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Default", NULL);

	zend_enum_add_case_cstr(class_entry, "Transparent", NULL);

	zend_enum_add_case_cstr(class_entry, "Black", NULL);

	zend_enum_add_case_cstr(class_entry, "White", NULL);

	zend_enum_add_case_cstr(class_entry, "Sides", NULL);

	zend_enum_add_case_cstr(class_entry, "Threshold", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_PerceptualDiffOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "PerceptualDiffOptions", class_Gd_PerceptualDiffOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_mode_default_value;
	ZVAL_UNDEF(&property_mode_default_value);
	zend_string *property_mode_name = zend_string_init("mode", sizeof("mode") - 1, true);
	zend_string *property_mode_class_Gd_PerceptualDiffMode = zend_string_init("Gd\\PerceptualDiffMode", sizeof("Gd\\PerceptualDiffMode")-1, 1);
	zend_declare_typed_property(class_entry, property_mode_name, &property_mode_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_mode_class_Gd_PerceptualDiffMode, 0, 0));
	zend_string_release_ex(property_mode_name, true);

	zval property_highlightColor_default_value;
	ZVAL_UNDEF(&property_highlightColor_default_value);
	zend_string *property_highlightColor_name = zend_string_init("highlightColor", sizeof("highlightColor") - 1, true);
	zend_declare_typed_property(class_entry, property_highlightColor_name, &property_highlightColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_highlightColor_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_PerceptualDiffResult(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "PerceptualDiffResult", class_Gd_PerceptualDiffResult_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_pixelsChanged_default_value;
	ZVAL_UNDEF(&property_pixelsChanged_default_value);
	zend_string *property_pixelsChanged_name = zend_string_init("pixelsChanged", sizeof("pixelsChanged") - 1, true);
	zend_declare_typed_property(class_entry, property_pixelsChanged_name, &property_pixelsChanged_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_pixelsChanged_name, true);

	zval property_maximumDelta_default_value;
	ZVAL_UNDEF(&property_maximumDelta_default_value);
	zend_string *property_maximumDelta_name = zend_string_init("maximumDelta", sizeof("maximumDelta") - 1, true);
	zend_declare_typed_property(class_entry, property_maximumDelta_name, &property_maximumDelta_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_maximumDelta_name, true);

	zval property_diffImage_default_value;
	ZVAL_UNDEF(&property_diffImage_default_value);
	zend_string *property_diffImage_name = zend_string_init("diffImage", sizeof("diffImage") - 1, true);
	zend_string *property_diffImage_class_GdImage = zend_string_init("GdImage", sizeof("GdImage")-1, 1);
	zend_declare_typed_property(class_entry, property_diffImage_name, &property_diffImage_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_diffImage_class_GdImage, 0, MAY_BE_NULL));
	zend_string_release_ex(property_diffImage_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_RotateOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "RotateOptions", class_Gd_RotateOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_backgroundColor_default_value;
	ZVAL_UNDEF(&property_backgroundColor_default_value);
	zend_string *property_backgroundColor_name = zend_string_init("backgroundColor", sizeof("backgroundColor") - 1, true);
	zend_declare_typed_property(class_entry, property_backgroundColor_name, &property_backgroundColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_backgroundColor_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_ScaleOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "ScaleOptions", class_Gd_ScaleOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_width_default_value;
	ZVAL_UNDEF(&property_width_default_value);
	zend_string *property_width_name = zend_string_init("width", sizeof("width") - 1, true);
	zend_declare_typed_property(class_entry, property_width_name, &property_width_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_width_name, true);

	zval property_height_default_value;
	ZVAL_UNDEF(&property_height_default_value);
	zend_string *property_height_name = zend_string_init("height", sizeof("height") - 1, true);
	zend_declare_typed_property(class_entry, property_height_name, &property_height_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release_ex(property_height_name, true);

	zval property_fit_default_value;
	ZVAL_UNDEF(&property_fit_default_value);
	zend_string *property_fit_name = zend_string_init("fit", sizeof("fit") - 1, true);
	zend_string *property_fit_class_Gd_ScaleFit = zend_string_init("Gd\\ScaleFit", sizeof("Gd\\ScaleFit")-1, 1);
	zend_declare_typed_property(class_entry, property_fit_name, &property_fit_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_fit_class_Gd_ScaleFit, 0, 0));
	zend_string_release_ex(property_fit_name, true);

	zval property_gravity_default_value;
	ZVAL_UNDEF(&property_gravity_default_value);
	zend_string *property_gravity_name = zend_string_init("gravity", sizeof("gravity") - 1, true);
	zend_string *property_gravity_class_Gd_ScaleGravity = zend_string_init("Gd\\ScaleGravity", sizeof("Gd\\ScaleGravity")-1, 1);
	zend_declare_typed_property(class_entry, property_gravity_name, &property_gravity_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_gravity_class_Gd_ScaleGravity, 0, 0));
	zend_string_release_ex(property_gravity_name, true);

	zval property_strategy_default_value;
	ZVAL_UNDEF(&property_strategy_default_value);
	zend_string *property_strategy_name = zend_string_init("strategy", sizeof("strategy") - 1, true);
	zend_string *property_strategy_class_Gd_ScaleStrategy = zend_string_init("Gd\\ScaleStrategy", sizeof("Gd\\ScaleStrategy")-1, 1);
	zend_declare_typed_property(class_entry, property_strategy_name, &property_strategy_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_strategy_class_Gd_ScaleStrategy, 0, MAY_BE_NULL));
	zend_string_release_ex(property_strategy_name, true);

	zval property_backgroundColor_default_value;
	ZVAL_UNDEF(&property_backgroundColor_default_value);
	zend_string *property_backgroundColor_name = zend_string_init("backgroundColor", sizeof("backgroundColor") - 1, true);
	zend_declare_typed_property(class_entry, property_backgroundColor_name, &property_backgroundColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_backgroundColor_name, true);

	zval property_interpolation_default_value;
	ZVAL_UNDEF(&property_interpolation_default_value);
	zend_string *property_interpolation_name = zend_string_init("interpolation", sizeof("interpolation") - 1, true);
	zend_string *property_interpolation_class_Gd_InterpolationMethod = zend_string_init("Gd\\InterpolationMethod", sizeof("Gd\\InterpolationMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_interpolation_name, &property_interpolation_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_interpolation_class_Gd_InterpolationMethod, 0, MAY_BE_NULL));
	zend_string_release_ex(property_interpolation_name, true);

	return class_entry;
}

#if defined(HAVE_GD_BUNDLED)
static zend_class_entry *register_class_Gd_TransformOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "TransformOptions", class_Gd_TransformOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_clip_default_value;
	ZVAL_UNDEF(&property_clip_default_value);
	zend_string *property_clip_name = zend_string_init("clip", sizeof("clip") - 1, true);
	zend_string *property_clip_class_Gd_Rect = zend_string_init("Gd\\Rect", sizeof("Gd\\Rect")-1, 1);
	zend_declare_typed_property(class_entry, property_clip_name, &property_clip_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_clip_class_Gd_Rect, 0, MAY_BE_NULL));
	zend_string_release_ex(property_clip_name, true);

	zval property_backgroundColor_default_value;
	ZVAL_UNDEF(&property_backgroundColor_default_value);
	zend_string *property_backgroundColor_name = zend_string_init("backgroundColor", sizeof("backgroundColor") - 1, true);
	zend_declare_typed_property(class_entry, property_backgroundColor_name, &property_backgroundColor_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_backgroundColor_name, true);

	zval property_interpolation_default_value;
	ZVAL_UNDEF(&property_interpolation_default_value);
	zend_string *property_interpolation_name = zend_string_init("interpolation", sizeof("interpolation") - 1, true);
	zend_string *property_interpolation_class_Gd_InterpolationMethod = zend_string_init("Gd\\InterpolationMethod", sizeof("Gd\\InterpolationMethod")-1, 1);
	zend_declare_typed_property(class_entry, property_interpolation_name, &property_interpolation_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_interpolation_class_Gd_InterpolationMethod, 0, MAY_BE_NULL));
	zend_string_release_ex(property_interpolation_name, true);

	return class_entry;
}
#endif

static zend_class_entry *register_class_Gd_AutoCropOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "AutoCropOptions", class_Gd_AutoCropOptions_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_READONLY_CLASS);

	zval property_mode_default_value;
	ZVAL_UNDEF(&property_mode_default_value);
	zend_string *property_mode_name = zend_string_init("mode", sizeof("mode") - 1, true);
	zend_string *property_mode_class_Gd_AutoCropMode = zend_string_init("Gd\\AutoCropMode", sizeof("Gd\\AutoCropMode")-1, 1);
	zend_declare_typed_property(class_entry, property_mode_name, &property_mode_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_mode_class_Gd_AutoCropMode, 0, 0));
	zend_string_release_ex(property_mode_name, true);

	zval property_threshold_default_value;
	ZVAL_UNDEF(&property_threshold_default_value);
	zend_string *property_threshold_name = zend_string_init("threshold", sizeof("threshold") - 1, true);
	zend_declare_typed_property(class_entry, property_threshold_name, &property_threshold_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_threshold_name, true);

	zval property_color_default_value;
	ZVAL_UNDEF(&property_color_default_value);
	zend_string *property_color_name = zend_string_init("color", sizeof("color") - 1, true);
	zend_declare_typed_property(class_entry, property_color_name, &property_color_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release_ex(property_color_name, true);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Image(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Image", class_Gd_Image_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
