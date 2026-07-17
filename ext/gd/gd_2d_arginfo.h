/* This is a generated file, edit gd_2d.stub.php instead.
 * Stub hash: 185af65fa97920efd3fc72b5f7a3df6d47060253 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_CompositeOperator_isUnbounded, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Paint___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Gradient___construct arginfo_class_Gd_Paint___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gradient_addColorStopRgb, 0, 4, Gd\\Gradient, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gradient_addColorStopRgba, 0, 5, Gd\\Gradient, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gradient_setExtend, 0, 1, Gd\\Gradient, 0)
	ZEND_ARG_OBJ_INFO(0, extend, Gd\\ExtendMode, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Gradient_setMatrix, 0, 1, Gd\\Gradient, 0)
	ZEND_ARG_OBJ_INFO(0, matrix, Gd\\Matrix, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_LinearGradient___construct, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_RadialGradient___construct, 0, 0, 6)
	ZEND_ARG_TYPE_INFO(0, x0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, r0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, r1, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Matrix___construct arginfo_class_Gd_Paint___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_translation, 0, 2, Gd\\Matrix, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Matrix_scaling arginfo_class_Gd_Matrix_translation

#define arginfo_class_Gd_Matrix_shearing arginfo_class_Gd_Matrix_translation

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_rotation, 0, 1, Gd\\Matrix, 0)
	ZEND_ARG_TYPE_INFO(0, radians, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_rotationAround, 0, 3, Gd\\Matrix, 0)
	ZEND_ARG_TYPE_INFO(0, radians, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_fromCoefficients, 0, 6, Gd\\Matrix, 0)
	ZEND_ARG_TYPE_INFO(0, m00, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, m10, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, m01, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, m11, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, m02, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, m12, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Matrix_translate arginfo_class_Gd_Matrix_translation

#define arginfo_class_Gd_Matrix_scale arginfo_class_Gd_Matrix_translation

#define arginfo_class_Gd_Matrix_shear arginfo_class_Gd_Matrix_translation

#define arginfo_class_Gd_Matrix_rotate arginfo_class_Gd_Matrix_rotation

#define arginfo_class_Gd_Matrix_rotateAround arginfo_class_Gd_Matrix_rotationAround

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_multiply, 0, 1, Gd\\Matrix, 0)
	ZEND_ARG_OBJ_INFO(0, other, Gd\\Matrix, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_inverted, 0, 0, Gd\\Matrix, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Matrix_mapPoint, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Matrix_mapRect, 0, 1, Gd\\Rect, 0)
	ZEND_ARG_OBJ_INFO(0, rect, Gd\\Rect, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Rect___construct, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Path___construct arginfo_class_Gd_Paint___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_moveTo, 0, 2, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_relMoveTo, 0, 2, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, dx, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Path_lineTo arginfo_class_Gd_Path_moveTo

#define arginfo_class_Gd_Path_relLineTo arginfo_class_Gd_Path_relMoveTo

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_curveTo, 0, 6, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x3, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y3, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_relCurveTo, 0, 6, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, dx1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dx2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dx3, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy3, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_quadTo, 0, 4, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_relQuadTo, 0, 4, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, dx1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dx2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy2, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_arc, 0, 5, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, cx, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, cy, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a1, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Path_negativeArc arginfo_class_Gd_Path_arc

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_rect, 0, 4, Gd\\Path, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_close, 0, 0, Gd\\Path, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_appendPath, 0, 1, Gd\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, other, Gd\\Path, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Path_transform, 0, 1, Gd\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, matrix, Gd\\Matrix, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Pattern___construct arginfo_class_Gd_Paint___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Pattern_setExtend, 0, 1, Gd\\Pattern, 0)
	ZEND_ARG_OBJ_INFO(0, extend, Gd\\ExtendMode, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Pattern_setMatrix, 0, 1, Gd\\Pattern, 0)
	ZEND_ARG_OBJ_INFO(0, matrix, Gd\\Matrix, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Pattern_setOpacity, 0, 1, Gd\\Pattern, 0)
	ZEND_ARG_TYPE_INFO(0, opacity, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context___construct arginfo_class_Gd_Paint___construct

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_createForImage, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_getImage, 0, 0, GdImage, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Context_flushImage, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_reloadImage arginfo_class_Gd_Context_flushImage

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Context_destroy, 0, 0, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flush, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_createPattern, 0, 1, Gd\\Pattern, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_appendPath, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, path, Gd\\Path, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_save, 0, 0, Gd\\Context, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_restore arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_clip arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_clipPreserve arginfo_class_Gd_Context_save

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setSource, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, source, Gd\\Paint, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setSourceRgb, 0, 3, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setSourceRgba, 0, 4, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, r, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setSourceImage, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, image, GdImage, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, x, IS_DOUBLE, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, y, IS_DOUBLE, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setOperator, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, operator, Gd\\CompositeOperator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setOpacity, 0, 1, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, opacity, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setLineWidth, 0, 1, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setLineCap, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, cap, Gd\\LineCap, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setLineJoin, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, join, Gd\\LineJoin, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setDash, 0, 2, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_setFillRule, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, rule, Gd\\FillRule, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_beginPath arginfo_class_Gd_Context_save

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_scale, 0, 2, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_translate arginfo_class_Gd_Context_scale

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_rotate, 0, 1, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, radians, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_transform, 0, 1, Gd\\Context, 0)
	ZEND_ARG_OBJ_INFO(0, matrix, Gd\\Matrix, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_moveTo arginfo_class_Gd_Context_scale

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_relMoveTo, 0, 2, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, dx, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_lineTo arginfo_class_Gd_Context_scale

#define arginfo_class_Gd_Context_relLineTo arginfo_class_Gd_Context_relMoveTo

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_curveTo, 0, 6, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x3, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y3, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_relCurveTo, 0, 6, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, dx1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dx2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dx3, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy3, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_quadTo, 0, 4, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_relQuadTo, 0, 4, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, dx1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dx2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, dy2, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_arc, 0, 5, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, cx, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, cy, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a1, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_negativeArc arginfo_class_Gd_Context_arc

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Context_rect, 0, 4, Gd\\Context, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Gd_Context_closePath arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_fill arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_fillPreserve arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_stroke arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_strokePreserve arginfo_class_Gd_Context_save

#define arginfo_class_Gd_Context_paint arginfo_class_Gd_Context_save

ZEND_METHOD(Gd_CompositeOperator, isUnbounded);
ZEND_METHOD(Gd_Paint, __construct);
ZEND_METHOD(Gd_Gradient, __construct);
ZEND_METHOD(Gd_Gradient, addColorStopRgb);
ZEND_METHOD(Gd_Gradient, addColorStopRgba);
ZEND_METHOD(Gd_Gradient, setExtend);
ZEND_METHOD(Gd_Gradient, setMatrix);
ZEND_METHOD(Gd_LinearGradient, __construct);
ZEND_METHOD(Gd_RadialGradient, __construct);
ZEND_METHOD(Gd_Matrix, __construct);
ZEND_METHOD(Gd_Matrix, translation);
ZEND_METHOD(Gd_Matrix, scaling);
ZEND_METHOD(Gd_Matrix, shearing);
ZEND_METHOD(Gd_Matrix, rotation);
ZEND_METHOD(Gd_Matrix, rotationAround);
ZEND_METHOD(Gd_Matrix, fromCoefficients);
ZEND_METHOD(Gd_Matrix, translate);
ZEND_METHOD(Gd_Matrix, scale);
ZEND_METHOD(Gd_Matrix, shear);
ZEND_METHOD(Gd_Matrix, rotate);
ZEND_METHOD(Gd_Matrix, rotateAround);
ZEND_METHOD(Gd_Matrix, multiply);
ZEND_METHOD(Gd_Matrix, inverted);
ZEND_METHOD(Gd_Matrix, mapPoint);
ZEND_METHOD(Gd_Matrix, mapRect);
ZEND_METHOD(Gd_Rect, __construct);
ZEND_METHOD(Gd_Path, __construct);
ZEND_METHOD(Gd_Path, moveTo);
ZEND_METHOD(Gd_Path, relMoveTo);
ZEND_METHOD(Gd_Path, lineTo);
ZEND_METHOD(Gd_Path, relLineTo);
ZEND_METHOD(Gd_Path, curveTo);
ZEND_METHOD(Gd_Path, relCurveTo);
ZEND_METHOD(Gd_Path, quadTo);
ZEND_METHOD(Gd_Path, relQuadTo);
ZEND_METHOD(Gd_Path, arc);
ZEND_METHOD(Gd_Path, negativeArc);
ZEND_METHOD(Gd_Path, rect);
ZEND_METHOD(Gd_Path, close);
ZEND_METHOD(Gd_Path, appendPath);
ZEND_METHOD(Gd_Path, transform);
ZEND_METHOD(Gd_Pattern, __construct);
ZEND_METHOD(Gd_Pattern, setExtend);
ZEND_METHOD(Gd_Pattern, setMatrix);
ZEND_METHOD(Gd_Pattern, setOpacity);
ZEND_METHOD(Gd_Context, __construct);
ZEND_METHOD(Gd_Context, createForImage);
ZEND_METHOD(Gd_Context, getImage);
ZEND_METHOD(Gd_Context, flushImage);
ZEND_METHOD(Gd_Context, reloadImage);
ZEND_METHOD(Gd_Context, destroy);
ZEND_METHOD(Gd_Context, createPattern);
ZEND_METHOD(Gd_Context, appendPath);
ZEND_METHOD(Gd_Context, save);
ZEND_METHOD(Gd_Context, restore);
ZEND_METHOD(Gd_Context, clip);
ZEND_METHOD(Gd_Context, clipPreserve);
ZEND_METHOD(Gd_Context, setSource);
ZEND_METHOD(Gd_Context, setSourceRgb);
ZEND_METHOD(Gd_Context, setSourceRgba);
ZEND_METHOD(Gd_Context, setSourceImage);
ZEND_METHOD(Gd_Context, setOperator);
ZEND_METHOD(Gd_Context, setOpacity);
ZEND_METHOD(Gd_Context, setLineWidth);
ZEND_METHOD(Gd_Context, setLineCap);
ZEND_METHOD(Gd_Context, setLineJoin);
ZEND_METHOD(Gd_Context, setDash);
ZEND_METHOD(Gd_Context, setFillRule);
ZEND_METHOD(Gd_Context, beginPath);
ZEND_METHOD(Gd_Context, scale);
ZEND_METHOD(Gd_Context, translate);
ZEND_METHOD(Gd_Context, rotate);
ZEND_METHOD(Gd_Context, transform);
ZEND_METHOD(Gd_Context, moveTo);
ZEND_METHOD(Gd_Context, relMoveTo);
ZEND_METHOD(Gd_Context, lineTo);
ZEND_METHOD(Gd_Context, relLineTo);
ZEND_METHOD(Gd_Context, curveTo);
ZEND_METHOD(Gd_Context, relCurveTo);
ZEND_METHOD(Gd_Context, quadTo);
ZEND_METHOD(Gd_Context, relQuadTo);
ZEND_METHOD(Gd_Context, arc);
ZEND_METHOD(Gd_Context, negativeArc);
ZEND_METHOD(Gd_Context, rect);
ZEND_METHOD(Gd_Context, closePath);
ZEND_METHOD(Gd_Context, fill);
ZEND_METHOD(Gd_Context, fillPreserve);
ZEND_METHOD(Gd_Context, stroke);
ZEND_METHOD(Gd_Context, strokePreserve);
ZEND_METHOD(Gd_Context, paint);

static const zend_function_entry class_Gd_CompositeOperator_methods[] = {
	ZEND_ME(Gd_CompositeOperator, isUnbounded, arginfo_class_Gd_CompositeOperator_isUnbounded, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Paint_methods[] = {
	ZEND_ME(Gd_Paint, __construct, arginfo_class_Gd_Paint___construct, ZEND_ACC_PRIVATE)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Gradient_methods[] = {
	ZEND_ME(Gd_Gradient, __construct, arginfo_class_Gd_Gradient___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Gradient, addColorStopRgb, arginfo_class_Gd_Gradient_addColorStopRgb, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Gradient, addColorStopRgba, arginfo_class_Gd_Gradient_addColorStopRgba, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Gradient, setExtend, arginfo_class_Gd_Gradient_setExtend, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Gradient, setMatrix, arginfo_class_Gd_Gradient_setMatrix, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_LinearGradient_methods[] = {
	ZEND_ME(Gd_LinearGradient, __construct, arginfo_class_Gd_LinearGradient___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_RadialGradient_methods[] = {
	ZEND_ME(Gd_RadialGradient, __construct, arginfo_class_Gd_RadialGradient___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Matrix_methods[] = {
	ZEND_ME(Gd_Matrix, __construct, arginfo_class_Gd_Matrix___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, translation, arginfo_class_Gd_Matrix_translation, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Matrix, scaling, arginfo_class_Gd_Matrix_scaling, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Matrix, shearing, arginfo_class_Gd_Matrix_shearing, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Matrix, rotation, arginfo_class_Gd_Matrix_rotation, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Matrix, rotationAround, arginfo_class_Gd_Matrix_rotationAround, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Matrix, fromCoefficients, arginfo_class_Gd_Matrix_fromCoefficients, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Matrix, translate, arginfo_class_Gd_Matrix_translate, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, scale, arginfo_class_Gd_Matrix_scale, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, shear, arginfo_class_Gd_Matrix_shear, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, rotate, arginfo_class_Gd_Matrix_rotate, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, rotateAround, arginfo_class_Gd_Matrix_rotateAround, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, multiply, arginfo_class_Gd_Matrix_multiply, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, inverted, arginfo_class_Gd_Matrix_inverted, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, mapPoint, arginfo_class_Gd_Matrix_mapPoint, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Matrix, mapRect, arginfo_class_Gd_Matrix_mapRect, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Rect_methods[] = {
	ZEND_ME(Gd_Rect, __construct, arginfo_class_Gd_Rect___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Path_methods[] = {
	ZEND_ME(Gd_Path, __construct, arginfo_class_Gd_Path___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, moveTo, arginfo_class_Gd_Path_moveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, relMoveTo, arginfo_class_Gd_Path_relMoveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, lineTo, arginfo_class_Gd_Path_lineTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, relLineTo, arginfo_class_Gd_Path_relLineTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, curveTo, arginfo_class_Gd_Path_curveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, relCurveTo, arginfo_class_Gd_Path_relCurveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, quadTo, arginfo_class_Gd_Path_quadTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, relQuadTo, arginfo_class_Gd_Path_relQuadTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, arc, arginfo_class_Gd_Path_arc, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, negativeArc, arginfo_class_Gd_Path_negativeArc, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, rect, arginfo_class_Gd_Path_rect, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, close, arginfo_class_Gd_Path_close, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, appendPath, arginfo_class_Gd_Path_appendPath, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Path, transform, arginfo_class_Gd_Path_transform, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Pattern_methods[] = {
	ZEND_ME(Gd_Pattern, __construct, arginfo_class_Gd_Pattern___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Pattern, setExtend, arginfo_class_Gd_Pattern_setExtend, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Pattern, setMatrix, arginfo_class_Gd_Pattern_setMatrix, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Pattern, setOpacity, arginfo_class_Gd_Pattern_setOpacity, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Gd_Context_methods[] = {
	ZEND_ME(Gd_Context, __construct, arginfo_class_Gd_Context___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Context, createForImage, arginfo_class_Gd_Context_createForImage, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Context, getImage, arginfo_class_Gd_Context_getImage, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, flushImage, arginfo_class_Gd_Context_flushImage, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, reloadImage, arginfo_class_Gd_Context_reloadImage, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, destroy, arginfo_class_Gd_Context_destroy, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, createPattern, arginfo_class_Gd_Context_createPattern, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, appendPath, arginfo_class_Gd_Context_appendPath, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, save, arginfo_class_Gd_Context_save, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, restore, arginfo_class_Gd_Context_restore, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, clip, arginfo_class_Gd_Context_clip, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, clipPreserve, arginfo_class_Gd_Context_clipPreserve, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setSource, arginfo_class_Gd_Context_setSource, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setSourceRgb, arginfo_class_Gd_Context_setSourceRgb, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setSourceRgba, arginfo_class_Gd_Context_setSourceRgba, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setSourceImage, arginfo_class_Gd_Context_setSourceImage, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setOperator, arginfo_class_Gd_Context_setOperator, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setOpacity, arginfo_class_Gd_Context_setOpacity, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setLineWidth, arginfo_class_Gd_Context_setLineWidth, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setLineCap, arginfo_class_Gd_Context_setLineCap, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setLineJoin, arginfo_class_Gd_Context_setLineJoin, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setDash, arginfo_class_Gd_Context_setDash, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, setFillRule, arginfo_class_Gd_Context_setFillRule, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, beginPath, arginfo_class_Gd_Context_beginPath, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, scale, arginfo_class_Gd_Context_scale, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, translate, arginfo_class_Gd_Context_translate, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, rotate, arginfo_class_Gd_Context_rotate, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, transform, arginfo_class_Gd_Context_transform, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, moveTo, arginfo_class_Gd_Context_moveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, relMoveTo, arginfo_class_Gd_Context_relMoveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, lineTo, arginfo_class_Gd_Context_lineTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, relLineTo, arginfo_class_Gd_Context_relLineTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, curveTo, arginfo_class_Gd_Context_curveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, relCurveTo, arginfo_class_Gd_Context_relCurveTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, quadTo, arginfo_class_Gd_Context_quadTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, relQuadTo, arginfo_class_Gd_Context_relQuadTo, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, arc, arginfo_class_Gd_Context_arc, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, negativeArc, arginfo_class_Gd_Context_negativeArc, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, rect, arginfo_class_Gd_Context_rect, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, closePath, arginfo_class_Gd_Context_closePath, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, fill, arginfo_class_Gd_Context_fill, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, fillPreserve, arginfo_class_Gd_Context_fillPreserve, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, stroke, arginfo_class_Gd_Context_stroke, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, strokePreserve, arginfo_class_Gd_Context_strokePreserve, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Context, paint, arginfo_class_Gd_Context_paint, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Gd_ExtendMode(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\ExtendMode", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "None", NULL);

	zend_enum_add_case_cstr(class_entry, "Repeat", NULL);

	zend_enum_add_case_cstr(class_entry, "Reflect", NULL);

	zend_enum_add_case_cstr(class_entry, "Pad", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_LineCap(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\LineCap", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Butt", NULL);

	zend_enum_add_case_cstr(class_entry, "Round", NULL);

	zend_enum_add_case_cstr(class_entry, "Square", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_LineJoin(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\LineJoin", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Miter", NULL);

	zend_enum_add_case_cstr(class_entry, "Round", NULL);

	zend_enum_add_case_cstr(class_entry, "Bevel", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_FillRule(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\FillRule", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "NonZero", NULL);

	zend_enum_add_case_cstr(class_entry, "EvenOdd", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_CompositeOperator(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Gd\\CompositeOperator", IS_UNDEF, class_Gd_CompositeOperator_methods);

	zend_enum_add_case_cstr(class_entry, "Clear", NULL);

	zend_enum_add_case_cstr(class_entry, "Source", NULL);

	zend_enum_add_case_cstr(class_entry, "Over", NULL);

	zend_enum_add_case_cstr(class_entry, "In", NULL);

	zend_enum_add_case_cstr(class_entry, "Out", NULL);

	zend_enum_add_case_cstr(class_entry, "Atop", NULL);

	zend_enum_add_case_cstr(class_entry, "Dest", NULL);

	zend_enum_add_case_cstr(class_entry, "DestOver", NULL);

	zend_enum_add_case_cstr(class_entry, "DestIn", NULL);

	zend_enum_add_case_cstr(class_entry, "DestOut", NULL);

	zend_enum_add_case_cstr(class_entry, "DestAtop", NULL);

	zend_enum_add_case_cstr(class_entry, "Xor", NULL);

	zend_enum_add_case_cstr(class_entry, "Add", NULL);

	zend_enum_add_case_cstr(class_entry, "Saturate", NULL);

	zend_enum_add_case_cstr(class_entry, "Multiply", NULL);

	zend_enum_add_case_cstr(class_entry, "Screen", NULL);

	zend_enum_add_case_cstr(class_entry, "Overlay", NULL);

	zend_enum_add_case_cstr(class_entry, "Darken", NULL);

	zend_enum_add_case_cstr(class_entry, "Lighten", NULL);

	zend_enum_add_case_cstr(class_entry, "ColorDodge", NULL);

	zend_enum_add_case_cstr(class_entry, "ColorBurn", NULL);

	zend_enum_add_case_cstr(class_entry, "HardLight", NULL);

	zend_enum_add_case_cstr(class_entry, "SoftLight", NULL);

	zend_enum_add_case_cstr(class_entry, "Difference", NULL);

	zend_enum_add_case_cstr(class_entry, "Exclusion", NULL);

	zend_enum_add_case_cstr(class_entry, "HslHue", NULL);

	zend_enum_add_case_cstr(class_entry, "HslSaturation", NULL);

	zend_enum_add_case_cstr(class_entry, "HslColor", NULL);

	zend_enum_add_case_cstr(class_entry, "HslLuminosity", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Paint(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Paint", class_Gd_Paint_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_ABSTRACT|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Gradient(zend_class_entry *class_entry_Gd_Paint)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Gradient", class_Gd_Gradient_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Paint, ZEND_ACC_ABSTRACT|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_LinearGradient(zend_class_entry *class_entry_Gd_Gradient)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "LinearGradient", class_Gd_LinearGradient_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Gradient, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_RadialGradient(zend_class_entry *class_entry_Gd_Gradient)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "RadialGradient", class_Gd_RadialGradient_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Gradient, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Matrix(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Matrix", class_Gd_Matrix_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Rect(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Rect", class_Gd_Rect_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	zval property_x_default_value;
	ZVAL_UNDEF(&property_x_default_value);
	zend_string *property_x_name = zend_string_init("x", sizeof("x") - 1, true);
	zend_declare_typed_property(class_entry, property_x_name, &property_x_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_x_name, true);

	zval property_y_default_value;
	ZVAL_UNDEF(&property_y_default_value);
	zend_string *property_y_name = zend_string_init("y", sizeof("y") - 1, true);
	zend_declare_typed_property(class_entry, property_y_name, &property_y_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_DOUBLE));
	zend_string_release_ex(property_y_name, true);

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

	return class_entry;
}

static zend_class_entry *register_class_Gd_Path(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Path", class_Gd_Path_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Pattern(zend_class_entry *class_entry_Gd_Paint)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Pattern", class_Gd_Pattern_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_Paint, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Context(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Context", class_Gd_Context_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
