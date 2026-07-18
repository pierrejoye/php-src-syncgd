#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gd_avif_metadata.h"
#include "gdhelpers.h"

#include <limits.h>

#ifdef HAVE_LIBAVIF

static int gd_avif_set_profile(gdImageMetadata *metadata, const char *key,
                               const avifRWData *profile)
{
    if (profile->size == 0) {
        return GD_META_OK;
    }
    return gdImageMetadataSetProfile(metadata, key, profile->data, profile->size);
}

int gdAvifReadMetadataFromPtr(int size, const void *data, gdAvifInfo *info,
                              gdImageMetadata *metadata)
{
    avifDecoder *decoder = NULL;
    avifResult result;
    int status;

    if (size < 0 || data == NULL || info == NULL || metadata == NULL) {
        return GD_META_ERR_INVALID;
    }

    decoder = avifDecoderCreate();
    if (decoder == NULL) {
        return GD_META_ERR_NOMEM;
    }

#if AVIF_VERSION >= 90100
    decoder->strictFlags &= ~AVIF_STRICT_PIXI_REQUIRED;
#endif

    result = avifDecoderSetIOMemory(decoder, data, (size_t) size);
    if (result != AVIF_RESULT_OK) {
        avifDecoderDestroy(decoder);
        return GD_META_ERR_PARSE;
    }

    result = avifDecoderParse(decoder);
    if (result != AVIF_RESULT_OK || decoder->image == NULL) {
        avifDecoderDestroy(decoder);
        return GD_META_ERR_PARSE;
    }

    if (decoder->image->width > INT_MAX || decoder->image->height > INT_MAX) {
        avifDecoderDestroy(decoder);
        return GD_META_ERR_LIMIT;
    }

    if (decoder->image->depth > INT_MAX || decoder->imageCount > INT_MAX) {
        avifDecoderDestroy(decoder);
        return GD_META_ERR_LIMIT;
    }

    info->width = (int) decoder->image->width;
    info->height = (int) decoder->image->height;
    info->is_progressive = decoder->progressiveState != AVIF_PROGRESSIVE_STATE_UNAVAILABLE;
    info->is_animation = !info->is_progressive && decoder->imageCount > 1;
    info->frame_count = decoder->imageCount;
    info->duration = decoder->duration;
    info->has_alpha = decoder->alphaPresent == AVIF_TRUE;
    info->bit_depth = (int) decoder->image->depth;
    switch (decoder->image->yuvFormat) {
        case AVIF_PIXEL_FORMAT_YUV444:
            info->yuv_format = GD_AVIF_PIXEL_FORMAT_YUV444;
            break;
        case AVIF_PIXEL_FORMAT_YUV422:
            info->yuv_format = GD_AVIF_PIXEL_FORMAT_YUV422;
            break;
        case AVIF_PIXEL_FORMAT_YUV420:
            info->yuv_format = GD_AVIF_PIXEL_FORMAT_YUV420;
            break;
        case AVIF_PIXEL_FORMAT_YUV400:
            info->yuv_format = GD_AVIF_PIXEL_FORMAT_YUV400;
            break;
        default:
            info->yuv_format = GD_AVIF_PIXEL_FORMAT_NONE;
            break;
    }
    status = gd_avif_set_profile(metadata, "exif", &decoder->image->exif);
    if (status == GD_META_OK) {
        status = gd_avif_set_profile(metadata, "xmp", &decoder->image->xmp);
    }

    avifDecoderDestroy(decoder);
    return status;
}

int gdAvifApplyMetadata(avifImage *image, const gdImageMetadata *metadata)
{
    const unsigned char *data;
    size_t size;
    avifResult result;

    if (image == NULL || metadata == NULL) {
        return GD_META_OK;
    }

    data = gdImageMetadataGetProfile(metadata, "exif", &size);
    if (data != NULL) {
        if (size > INT_MAX) {
            return GD_META_ERR_LIMIT;
        }
        result = avifImageSetMetadataExif(image, data, size);
        if (result != AVIF_RESULT_OK) {
            return GD_META_ERR_INVALID;
        }
    }

    data = gdImageMetadataGetProfile(metadata, "xmp", &size);
    if (data != NULL) {
        if (size > INT_MAX) {
            return GD_META_ERR_LIMIT;
        }
        result = avifImageSetMetadataXMP(image, data, size);
        if (result != AVIF_RESULT_OK) {
            return GD_META_ERR_INVALID;
        }
    }

    return GD_META_OK;
}

#endif
