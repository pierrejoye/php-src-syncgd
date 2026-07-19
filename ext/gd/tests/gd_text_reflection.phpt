--TEST--
GD text API reflection
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\FontFace::class)) {
    die("skip GD text API not available");
}
?>
--FILE--
<?php
$classes = [
    Gd\GdException::class,
    Gd\FontFace::class,
    Gd\TextOptions::class,
    Gd\TextExtents::class,
    Gd\Text\TextException::class,
    Gd\Text\InvalidTextException::class,
    Gd\Text\UnavailableException::class,
    Gd\Text\FontException::class,
    Gd\Text\LayoutException::class,
    Gd\Text\MemoryException::class,
];

foreach ($classes as $class) {
    var_dump(class_exists($class));
}

var_dump(enum_exists(Gd\TextShaping::class));
var_dump(Gd\TextShaping::cases());

$font = new ReflectionClass(Gd\FontFace::class);
var_dump($font->isFinal());
var_dump($font->getConstructor()->isPrivate());
foreach (['fromFile', 'fromString'] as $method) {
    $rm = $font->getMethod($method);
    var_dump($rm->isStatic(), (string) $rm->getReturnType());
}

$options = new ReflectionClass(Gd\TextOptions::class);
var_dump($options->isFinal(), $options->isReadOnly());
$ctor = $options->getConstructor();
var_dump($ctor->getNumberOfParameters());

$extents = new ReflectionClass(Gd\TextExtents::class);
var_dump($extents->isFinal(), $extents->isReadOnly());

var_dump(get_parent_class(Gd\Text\TextException::class));
foreach ([Gd\Text\InvalidTextException::class, Gd\Text\UnavailableException::class, Gd\Text\FontException::class, Gd\Text\LayoutException::class, Gd\Text\MemoryException::class] as $class) {
    var_dump(is_subclass_of($class, Gd\Text\TextException::class));
}

$context = new ReflectionClass(Gd\Context::class);
foreach (['setFontFace', 'setFontSize', 'textPath', 'showText', 'textExtents'] as $method) {
    var_dump($context->hasMethod($method));
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
array(2) {
  [0]=>
  enum(Gd\TextShaping::None)
  [1]=>
  enum(Gd\TextShaping::Raqm)
}
bool(true)
bool(true)
bool(true)
string(11) "Gd\FontFace"
bool(true)
string(11) "Gd\FontFace"
bool(true)
bool(true)
int(2)
bool(true)
bool(true)
string(14) "Gd\GdException"
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
