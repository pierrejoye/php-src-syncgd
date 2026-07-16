/* This is a generated file, edit gd_image.stub.php instead.
 * Stub hash: d449884d79b2e73e5a5adb2e69fd15a8f79022da */

#ifndef ZEND_GD_IMAGE_DECL_d449884d79b2e73e5a5adb2e69fd15a8f79022da_H
#define ZEND_GD_IMAGE_DECL_d449884d79b2e73e5a5adb2e69fd15a8f79022da_H

typedef enum zend_enum_Gd_Codec_Format {
	ZEND_ENUM_Gd_Codec_Format_Png = 1,
	ZEND_ENUM_Gd_Codec_Format_Jpeg = 2,
	ZEND_ENUM_Gd_Codec_Format_Webp = 3,
	ZEND_ENUM_Gd_Codec_Format_Bmp = 4,
	ZEND_ENUM_Gd_Codec_Format_Avif = 5,
	ZEND_ENUM_Gd_Codec_Format_Heif = 6,
	ZEND_ENUM_Gd_Codec_Format_Jxl = 7,
	ZEND_ENUM_Gd_Codec_Format_Tiff = 8,
	ZEND_ENUM_Gd_Codec_Format_Gif = 9,
	ZEND_ENUM_Gd_Codec_Format_Qoi = 10,
} zend_enum_Gd_Codec_Format;

typedef enum zend_enum_Gd_ColorModel {
	ZEND_ENUM_Gd_ColorModel_TrueColor = 1,
	ZEND_ENUM_Gd_ColorModel_Palette = 2,
} zend_enum_Gd_ColorModel;

typedef enum zend_enum_Gd_PerceptualDiffMode {
	ZEND_ENUM_Gd_PerceptualDiffMode_None = 1,
	ZEND_ENUM_Gd_PerceptualDiffMode_Overlay = 2,
	ZEND_ENUM_Gd_PerceptualDiffMode_Mask = 3,
} zend_enum_Gd_PerceptualDiffMode;

typedef enum zend_enum_Gd_ScaleFit {
	ZEND_ENUM_Gd_ScaleFit_Cover = 1,
	ZEND_ENUM_Gd_ScaleFit_Contain = 2,
	ZEND_ENUM_Gd_ScaleFit_Fill = 3,
	ZEND_ENUM_Gd_ScaleFit_Inside = 4,
	ZEND_ENUM_Gd_ScaleFit_Outside = 5,
} zend_enum_Gd_ScaleFit;

typedef enum zend_enum_Gd_ScaleGravity {
	ZEND_ENUM_Gd_ScaleGravity_Northwest = 1,
	ZEND_ENUM_Gd_ScaleGravity_North = 2,
	ZEND_ENUM_Gd_ScaleGravity_Northeast = 3,
	ZEND_ENUM_Gd_ScaleGravity_West = 4,
	ZEND_ENUM_Gd_ScaleGravity_Center = 5,
	ZEND_ENUM_Gd_ScaleGravity_East = 6,
	ZEND_ENUM_Gd_ScaleGravity_Southwest = 7,
	ZEND_ENUM_Gd_ScaleGravity_South = 8,
	ZEND_ENUM_Gd_ScaleGravity_Southeast = 9,
} zend_enum_Gd_ScaleGravity;

typedef enum zend_enum_Gd_ScaleStrategy {
	ZEND_ENUM_Gd_ScaleStrategy_Entropy = 1,
	ZEND_ENUM_Gd_ScaleStrategy_Attention = 2,
} zend_enum_Gd_ScaleStrategy;

typedef enum zend_enum_Gd_ScaleInterpolation {
	ZEND_ENUM_Gd_ScaleInterpolation_Default = 1,
	ZEND_ENUM_Gd_ScaleInterpolation_Bell = 2,
	ZEND_ENUM_Gd_ScaleInterpolation_Bessel = 3,
	ZEND_ENUM_Gd_ScaleInterpolation_Linear = 4,
	ZEND_ENUM_Gd_ScaleInterpolation_CubicSpline = 5,
	ZEND_ENUM_Gd_ScaleInterpolation_Blackman = 6,
	ZEND_ENUM_Gd_ScaleInterpolation_Box = 7,
	ZEND_ENUM_Gd_ScaleInterpolation_BSpline = 8,
	ZEND_ENUM_Gd_ScaleInterpolation_CatmullRom = 9,
	ZEND_ENUM_Gd_ScaleInterpolation_Gaussian = 10,
	ZEND_ENUM_Gd_ScaleInterpolation_GeneralizedCubic = 11,
	ZEND_ENUM_Gd_ScaleInterpolation_Hermite = 12,
	ZEND_ENUM_Gd_ScaleInterpolation_Hamming = 13,
	ZEND_ENUM_Gd_ScaleInterpolation_Hanning = 14,
	ZEND_ENUM_Gd_ScaleInterpolation_Mitchell = 15,
	ZEND_ENUM_Gd_ScaleInterpolation_NearestNeighbour = 16,
	ZEND_ENUM_Gd_ScaleInterpolation_Power = 17,
	ZEND_ENUM_Gd_ScaleInterpolation_Quadratic = 18,
	ZEND_ENUM_Gd_ScaleInterpolation_Sinc = 19,
	ZEND_ENUM_Gd_ScaleInterpolation_Triangle = 20,
	ZEND_ENUM_Gd_ScaleInterpolation_Weighted4 = 21,
	ZEND_ENUM_Gd_ScaleInterpolation_Lanczos3 = 22,
	ZEND_ENUM_Gd_ScaleInterpolation_Lanczos8 = 23,
	ZEND_ENUM_Gd_ScaleInterpolation_BlackmanBessel = 24,
	ZEND_ENUM_Gd_ScaleInterpolation_BlackmanSinc = 25,
	ZEND_ENUM_Gd_ScaleInterpolation_QuadraticBSpline = 26,
	ZEND_ENUM_Gd_ScaleInterpolation_Cosine = 27,
	ZEND_ENUM_Gd_ScaleInterpolation_Welsh = 28,
} zend_enum_Gd_ScaleInterpolation;

typedef enum zend_enum_Gd_AutoCropMode {
	ZEND_ENUM_Gd_AutoCropMode_Default = 1,
	ZEND_ENUM_Gd_AutoCropMode_Transparent = 2,
	ZEND_ENUM_Gd_AutoCropMode_Black = 3,
	ZEND_ENUM_Gd_AutoCropMode_White = 4,
	ZEND_ENUM_Gd_AutoCropMode_Sides = 5,
	ZEND_ENUM_Gd_AutoCropMode_Threshold = 6,
} zend_enum_Gd_AutoCropMode;

#endif /* ZEND_GD_IMAGE_DECL_d449884d79b2e73e5a5adb2e69fd15a8f79022da_H */
