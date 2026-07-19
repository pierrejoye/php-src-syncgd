--TEST--
GD base exception reflection
--EXTENSIONS--
gd
--FILE--
<?php
var_dump(class_exists(Gd\GdException::class));
var_dump(get_parent_class(Gd\GdException::class));
var_dump(is_subclass_of(Gd\GdException::class, Throwable::class));
var_dump((new ReflectionClass(Gd\GdException::class))->isFinal());
?>
--EXPECT--
bool(true)
string(9) "Exception"
bool(true)
bool(false)
