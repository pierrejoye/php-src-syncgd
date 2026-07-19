/* This is a generated file, edit gd_exception.stub.php instead.
 * Stub hash: 5495ea2566b44f00d30be14e42a556b5940bb652 */

static zend_class_entry *register_class_Gd_GdException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd", "GdException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Exception, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}

static zend_class_entry *register_class_Gd_Codec_CodecException(zend_class_entry *class_entry_Gd_GdException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Gd\\Codec", "CodecException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Gd_GdException, ZEND_ACC_NO_DYNAMIC_PROPERTIES);

	return class_entry;
}
