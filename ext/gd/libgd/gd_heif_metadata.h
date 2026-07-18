#ifndef GD_HEIF_METADATA_H
#define GD_HEIF_METADATA_H

#include "gd.h"

#ifdef HAVE_LIBHEIF
#include <libheif/heif.h>

int gdHeifReadMetadataFromPtr(int size, const void *data, gdHeifInfo *info,
                              gdImageMetadata *metadata);
int gdHeifApplyImageMetadata(struct heif_image *image, const gdImageMetadata *metadata);
int gdHeifApplyMetadata(struct heif_context *context, struct heif_image *image,
                        struct heif_image_handle *handle, const gdImageMetadata *metadata);
#endif

#endif
