/* This is a generated file, edit gd_tiff.stub.php instead.
 * Stub hash: d6c4296d30ee85e978b13c10f899017ba5dbbd72 */

#ifndef ZEND_GD_TIFF_DECL_d6c4296d30ee85e978b13c10f899017ba5dbbd72_H
#define ZEND_GD_TIFF_DECL_d6c4296d30ee85e978b13c10f899017ba5dbbd72_H

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
typedef enum zend_enum_Gd_Tiff_ColorSpace {
	ZEND_ENUM_Gd_Tiff_ColorSpace_Rgb = 1,
	ZEND_ENUM_Gd_Tiff_ColorSpace_Rgba = 2,
	ZEND_ENUM_Gd_Tiff_ColorSpace_Gray = 3,
} zend_enum_Gd_Tiff_ColorSpace;
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
typedef enum zend_enum_Gd_Tiff_Compression {
	ZEND_ENUM_Gd_Tiff_Compression_None = 1,
	ZEND_ENUM_Gd_Tiff_Compression_CcittRle = 2,
	ZEND_ENUM_Gd_Tiff_Compression_CcittFax3 = 3,
	ZEND_ENUM_Gd_Tiff_Compression_CcittFax4 = 4,
	ZEND_ENUM_Gd_Tiff_Compression_Lzw = 5,
	ZEND_ENUM_Gd_Tiff_Compression_Jpeg = 6,
	ZEND_ENUM_Gd_Tiff_Compression_AdobeDeflate = 7,
	ZEND_ENUM_Gd_Tiff_Compression_Deflate = 8,
	ZEND_ENUM_Gd_Tiff_Compression_PackBits = 9,
} zend_enum_Gd_Tiff_Compression;
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
typedef enum zend_enum_Gd_Tiff_ResolutionUnit {
	ZEND_ENUM_Gd_Tiff_ResolutionUnit_None = 1,
	ZEND_ENUM_Gd_Tiff_ResolutionUnit_Inch = 2,
	ZEND_ENUM_Gd_Tiff_ResolutionUnit_Centimeter = 3,
} zend_enum_Gd_Tiff_ResolutionUnit;
#endif

#if defined(HAVE_GD_TIFF_READ_API)
typedef enum zend_enum_Gd_Tiff_Photometric {
	ZEND_ENUM_Gd_Tiff_Photometric_MinIsWhite = 1,
	ZEND_ENUM_Gd_Tiff_Photometric_MinIsBlack = 2,
	ZEND_ENUM_Gd_Tiff_Photometric_Rgb = 3,
	ZEND_ENUM_Gd_Tiff_Photometric_Palette = 4,
	ZEND_ENUM_Gd_Tiff_Photometric_TransparencyMask = 5,
	ZEND_ENUM_Gd_Tiff_Photometric_Separated = 6,
	ZEND_ENUM_Gd_Tiff_Photometric_YCbCr = 7,
	ZEND_ENUM_Gd_Tiff_Photometric_CieLab = 8,
} zend_enum_Gd_Tiff_Photometric;
#endif

#if defined(HAVE_GD_TIFF_READ_API)
typedef enum zend_enum_Gd_Tiff_PlanarConfiguration {
	ZEND_ENUM_Gd_Tiff_PlanarConfiguration_Contiguous = 1,
	ZEND_ENUM_Gd_Tiff_PlanarConfiguration_Separate = 2,
} zend_enum_Gd_Tiff_PlanarConfiguration;
#endif

#endif /* ZEND_GD_TIFF_DECL_d6c4296d30ee85e978b13c10f899017ba5dbbd72_H */
