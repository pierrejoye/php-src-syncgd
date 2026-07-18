#ifndef GD_AVIF_METADATA_H
#define GD_AVIF_METADATA_H

#include "gd.h"

#ifdef HAVE_LIBAVIF
#include <avif/avif.h>

int gdAvifReadMetadataFromPtr(int size, const void *data, gdAvifInfo *info,
                              gdImageMetadata *metadata);
int gdAvifApplyMetadata(avifImage *image, const gdImageMetadata *metadata);
#endif

#endif
