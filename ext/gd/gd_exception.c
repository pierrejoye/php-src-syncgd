/*
   +----------------------------------------------------------------------+
   | Copyright © The PHP Group and Contributors.                          |
   +----------------------------------------------------------------------+
   | This source file is subject to the Modified BSD License and is        |
   | available through the world-wide-web at the following url:            |
   | https://www.php.net/license/                                             |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_exceptions.h"
#include "php_gd.h"
#include "gd_exception.h"
#include "gd_exception_arginfo.h"

static zend_class_entry *php_gd_exception_ce;

void php_gd_exception_minit(void)
{
	php_gd_exception_ce = register_class_Gd_GdException(zend_ce_exception);
}

zend_class_entry *php_gd_get_exception_ce(void)
{
	return php_gd_exception_ce;
}

zend_class_entry *php_gd_get_codec_exception_ce(void)
{
	static zend_class_entry *codec_exception_ce;

	if (codec_exception_ce == NULL) {
		codec_exception_ce = register_class_Gd_Codec_CodecException(php_gd_exception_ce);
	}

	return codec_exception_ce;
}
