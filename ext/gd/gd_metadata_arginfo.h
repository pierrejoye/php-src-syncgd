/* This is a generated file, edit gd_metadata.stub.php instead.
 * Stub hash: 3c841a726bcb8eac42da16e87351f7bed4f08988 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Gd_Metadata___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_create, 0, 0, IS_STATIC, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_get, 0, 1, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_keys, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_count, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Gd_Metadata_getIterator, 0, 0, Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_with, 0, 2, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Gd_Metadata_without, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Gd_Metadata, __construct);
ZEND_METHOD(Gd_Metadata, create);
ZEND_METHOD(Gd_Metadata, has);
ZEND_METHOD(Gd_Metadata, get);
ZEND_METHOD(Gd_Metadata, keys);
ZEND_METHOD(Gd_Metadata, count);
ZEND_METHOD(Gd_Metadata, getIterator);
ZEND_METHOD(Gd_Metadata, with);
ZEND_METHOD(Gd_Metadata, without);

static const zend_function_entry class_Gd_Metadata_methods[] = {
	ZEND_ME(Gd_Metadata, __construct, arginfo_class_Gd_Metadata___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(Gd_Metadata, create, arginfo_class_Gd_Metadata_create, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Gd_Metadata, has, arginfo_class_Gd_Metadata_has, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Metadata, get, arginfo_class_Gd_Metadata_get, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Metadata, keys, arginfo_class_Gd_Metadata_keys, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Metadata, count, arginfo_class_Gd_Metadata_count, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Metadata, getIterator, arginfo_class_Gd_Metadata_getIterator, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Metadata, with, arginfo_class_Gd_Metadata_with, ZEND_ACC_PUBLIC)
	ZEND_ME(Gd_Metadata, without, arginfo_class_Gd_Metadata_without, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Gd_Metadata(zend_class_entry *class_entry_Countable, zend_class_entry *class_entry_IteratorAggregate)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "Metadata", class_Gd_Metadata_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);
	zend_class_implements(class_entry, 2, class_entry_Countable, class_entry_IteratorAggregate);

	return class_entry;
}
