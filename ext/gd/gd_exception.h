#ifndef PHP_GD_EXCEPTION_H
#define PHP_GD_EXCEPTION_H

void php_gd_exception_minit(void);
zend_class_entry *php_gd_get_exception_ce(void);
zend_class_entry *php_gd_get_codec_exception_ce(void);

#endif
