/* WBMP
 * ----
 * WBMP Level 0: B/W, Uncompressed
 * This implements the WBMP format as specified in WAPSpec 1.1 and 1.2.
 * It does not support ExtHeaders as defined in the spec. The spec states
 * that a WAP client does not need to implement ExtHeaders.
 *
 * (c) 2000 Johan Van den Brande <johan@vandenbrande.com>
 *
 * Header file
*/
#ifndef __WBMP_H
#define __WBMP_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_compat.h"

/* WBMP struct
 * -----------
 * A Wireless bitmap structure
*/

typedef struct Wbmp_ {
    int type;    /* type of the wbmp */
    int width;   /* width of the image */
    int height;  /* height of the image */
    int *bitmap; /* pointer to data: 0 = WHITE , 1 = BLACK */
} Wbmp;

#define WBMP_WHITE 1
#define WBMP_BLACK 0

// Use to cap the maximum width and height using commonly used cap
// while common usages are much smaller, the spec allows for 65535x65535, which is too large for practical use and can cause memory issues.
#define WBMP_MAX_WIDTH 65535
#define WBMP_MAX_HEIGHT 65535

/* Proto's
 * -------
*/
void putmbi(int i, void (*putout)(int c, void *out), void *out);
int getmbi(int (*getin)(void *in), void *in);
int skipheader(int (*getin)(void *in), void *in);
Wbmp *createwbmp(int width, int height, int color);
int readwbmp(int (*getin)(void *in), void *in, Wbmp **wbmp);
int writewbmp(Wbmp *wbmp, void (*putout)(int c, void *out), void *out);
void freewbmp(Wbmp *wbmp);
void printwbmp(Wbmp *wbmp);

#endif
