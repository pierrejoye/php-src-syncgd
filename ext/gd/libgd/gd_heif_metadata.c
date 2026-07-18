#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gd_heif_metadata.h"
#include "gdhelpers.h"

#include <limits.h>
#include <string.h>

#ifdef HAVE_LIBHEIF

static int gd_heif_set_profile(gdImageMetadata *metadata, const char *key,
                                const void *data, size_t size)
{
    if (gdImageMetadataGetProfile(metadata, key, NULL) != NULL) {
        return GD_META_OK;
    }
    return gdImageMetadataSetProfile(metadata, key, (const unsigned char *) data, size);
}

static int gd_heif_collect_metadata(const struct heif_image_handle *handle,
                                     gdImageMetadata *metadata)
{
    int count, i;
    heif_item_id *ids = NULL;

    if (handle == NULL || metadata == NULL) {
        return GD_META_ERR_INVALID;
    }

    count = heif_image_handle_get_number_of_metadata_blocks(handle, NULL);
    if (count < 0) {
        return GD_META_ERR_PARSE;
    }
    if (count > 0) {
        ids = (heif_item_id *) gdMalloc((size_t) count * sizeof(*ids));
        if (ids == NULL) {
            return GD_META_ERR_NOMEM;
        }
        count = heif_image_handle_get_list_of_metadata_block_IDs(handle, NULL, ids, count);
    }

    for (i = 0; i < count; i++) {
        const char *type = heif_image_handle_get_metadata_type(handle, ids[i]);
        const char *content_type = heif_image_handle_get_metadata_content_type(handle, ids[i]);
        const char *key = NULL;
        size_t size;
        unsigned char *data;
        struct heif_error error;

        if (type != NULL && strcmp(type, "Exif") == 0) {
            key = "exif";
        } else if (type != NULL && strcmp(type, "iptc") == 0) {
            key = "iptc";
        } else if (type != NULL && strcmp(type, "mime") == 0 && content_type != NULL &&
                   strcmp(content_type, "application/rdf+xml") == 0) {
            key = "xmp";
        }
        if (key == NULL || gdImageMetadataGetProfile(metadata, key, NULL) != NULL) {
            continue;
        }

        size = heif_image_handle_get_metadata_size(handle, ids[i]);
        data = (unsigned char *) gdMalloc(size ? size : 1);
        if (data == NULL) {
            gdFree(ids);
            return GD_META_ERR_NOMEM;
        }
        error = heif_image_handle_get_metadata(handle, ids[i], data);
        if (error.code != heif_error_Ok) {
            gdFree(data);
            gdFree(ids);
            return GD_META_ERR_PARSE;
        }
        if (gd_heif_set_profile(metadata, key, data, size) != GD_META_OK) {
            gdFree(data);
            gdFree(ids);
            return GD_META_ERR_LIMIT;
        }
        gdFree(data);
    }
    gdFree(ids);

    size_t size = heif_image_handle_get_raw_color_profile_size(handle);
    if (size > 0 && gdImageMetadataGetProfile(metadata, "icc", NULL) == NULL) {
        unsigned char *data = (unsigned char *) gdMalloc(size);
        struct heif_error error;
        if (data == NULL) {
            return GD_META_ERR_NOMEM;
        }
        error = heif_image_handle_get_raw_color_profile(handle, data);
        if (error.code != heif_error_Ok) {
            gdFree(data);
            return GD_META_ERR_PARSE;
        }
        if (gdImageMetadataSetProfile(metadata, "icc", data, size) != GD_META_OK) {
            gdFree(data);
            return GD_META_ERR_LIMIT;
        }
        gdFree(data);
    }

    return GD_META_OK;
}

static int gd_heif_is_animation(const void *data, int size)
{
    static const char *const sequence_brands[] = {
        "hevc", "hevx", "hevm", "hevs", "avis", "msf1", "vvis", "evbs", "evms", "jpgs", "j2is"
    };
    size_t i;

    for (i = 0; i < sizeof(sequence_brands) / sizeof(sequence_brands[0]); i++) {
        if (heif_has_compatible_brand((const uint8_t *) data, size, sequence_brands[i])) {
            return 1;
        }
    }
    return 0;
}

int gdHeifReadMetadataFromPtr(int size, const void *data, gdHeifInfo *info,
                              gdImageMetadata *metadata)
{
    struct heif_context *context;
    struct heif_image_handle *handle = NULL;
    struct heif_error error;
    int status;

    if (size < 0 || data == NULL || info == NULL || metadata == NULL) {
        return GD_META_ERR_INVALID;
    }
    context = heif_context_alloc();
    if (context == NULL) {
        return GD_META_ERR_NOMEM;
    }
    error = heif_context_read_from_memory_without_copy(context, data, (size_t) size, NULL);
    if (error.code != heif_error_Ok) {
        heif_context_free(context);
        return GD_META_ERR_PARSE;
    }
    error = heif_context_get_primary_image_handle(context, &handle);
    if (error.code != heif_error_Ok || handle == NULL) {
        heif_context_free(context);
        return GD_META_ERR_PARSE;
    }
    info->width = heif_image_handle_get_width(handle);
    info->height = heif_image_handle_get_height(handle);
    info->top_level_image_count = heif_context_get_number_of_top_level_images(context);
    info->has_alpha = heif_image_handle_has_alpha_channel(handle) != 0;
    info->bit_depth = heif_image_handle_get_luma_bits_per_pixel(handle);
    info->is_animation = gd_heif_is_animation(data, size);
    status = gd_heif_collect_metadata(handle, metadata);
    heif_image_handle_release(handle);
    heif_context_free(context);
    return status;
}

static int gd_heif_apply_profile(const gdImageMetadata *metadata, const char *key,
                                 const unsigned char **data, size_t *size)
{
    *data = gdImageMetadataGetProfile(metadata, key, size);
    if (*data == NULL) {
        return GD_META_OK;
    }
    return *size > INT_MAX ? GD_META_ERR_LIMIT : GD_META_OK;
}

static int gd_heif_apply_icc(struct heif_image *image, const gdImageMetadata *metadata)
{
    const unsigned char *data;
    size_t size;
    struct heif_error error;

    if (metadata == NULL || image == NULL) {
        return GD_META_OK;
    }
    if (gd_heif_apply_profile(metadata, "icc", &data, &size) != GD_META_OK) {
        return GD_META_ERR_LIMIT;
    }
    if (data != NULL) {
        error = heif_image_set_raw_color_profile(image, "prof", data, size);
        if (error.code != heif_error_Ok) {
            return GD_META_ERR_INVALID;
        }
    }
    return GD_META_OK;
}

int gdHeifApplyImageMetadata(struct heif_image *image, const gdImageMetadata *metadata)
{
    return gd_heif_apply_icc(image, metadata);
}

int gdHeifApplyMetadata(struct heif_context *context, struct heif_image *image,
                        struct heif_image_handle *handle, const gdImageMetadata *metadata)
{
    const unsigned char *data;
    size_t size;
    struct heif_error error;

    (void) image;

    if (metadata == NULL) {
        return GD_META_OK;
    }
    if (handle == NULL || context == NULL) {
        return GD_META_ERR_INVALID;
    }

    if (gd_heif_apply_profile(metadata, "exif", &data, &size) != GD_META_OK) {
        return GD_META_ERR_LIMIT;
    }
    if (data != NULL) {
        /* libheif returns the stored EXIF item including its four-byte
         * TIFF-offset prefix, while the add API expects the EXIF payload. */
        if (size < 4) {
            return GD_META_ERR_INVALID;
        }
        error = heif_context_add_exif_metadata(context, handle, data + 4, (int) (size - 4));
        if (error.code != heif_error_Ok) {
            return GD_META_ERR_INVALID;
        }
    }
    if (gd_heif_apply_profile(metadata, "xmp", &data, &size) != GD_META_OK) {
        return GD_META_ERR_LIMIT;
    }
    if (data != NULL) {
        error = heif_context_add_XMP_metadata(context, handle, data, (int) size);
        if (error.code != heif_error_Ok) {
            return GD_META_ERR_INVALID;
        }
    }
    if (gd_heif_apply_profile(metadata, "iptc", &data, &size) != GD_META_OK) {
        return GD_META_ERR_LIMIT;
    }
    if (data != NULL) {
        error = heif_context_add_generic_metadata(context, handle, data, (int) size,
                                                   "iptc", NULL);
        if (error.code != heif_error_Ok) {
            return GD_META_ERR_INVALID;
        }
    }
    return GD_META_OK;
}

#endif
