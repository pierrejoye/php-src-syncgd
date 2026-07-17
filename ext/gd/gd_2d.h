#ifndef PHP_GD_2D_H
#define PHP_GD_2D_H

#include "php.h"

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd_vector2d.h"
#else
# include <gd.h>
# include <gd_vector2d.h>
#endif

typedef struct _php_gd_context_object {
	gdContextPtr context;
	zval image;
	zval font_faces;
	zend_object std;
} php_gd_context_object;

void php_gd_2d_minit(void);
zend_class_entry *php_gd_get_context_ce(void);
zend_class_entry *php_gd_get_matrix_ce(void);
zend_class_entry *php_gd_get_rect_ce(void);
zend_class_entry *php_gd_get_composite_operator_ce(void);
php_gd_context_object *php_gd_context_object_from_zend_object(zend_object *zobj);
php_gd_context_object *php_gd_context_from_zval(zval *zv);
gdCompositeOperator php_gd_composite_operator_from_zval(zval *zv);
void php_gd_matrix_to_affine(zval *matrix_zv, double affine[6]);
bool php_gd_rect_to_gd_rect(zval *rect_zv, gdRect *rect);
bool php_gd_2d_validate_finite_arg(double value, uint32_t arg_num);

#endif
