--TEST--
GdImage::autoCrop() reflection and options defaults
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_autocrop_supported.inc";
if (!method_exists(GdImage::class, "autoCrop")) {
    die("skip GdImage::autoCrop() not available.");
}
?>
--FILE--
<?php
$options = new Gd\AutoCropOptions();
var_dump($options->mode);
var_dump($options->threshold);
var_dump($options->color);

$class = new ReflectionClass(Gd\AutoCropOptions::class);
var_dump($class->isFinal());
var_dump($class->isReadOnly());

var_dump(array_map(fn($case) => $case->name, Gd\AutoCropMode::cases()));

$method = new ReflectionMethod(GdImage::class, 'autoCrop');
var_dump($method->getReturnType()->getName());
foreach ($method->getParameters() as $parameter) {
    printf(
        "%s nullable=%s default=%s\n",
        $parameter->getName(),
        $parameter->allowsNull() ? 'yes' : 'no',
        $parameter->isDefaultValueAvailable() ? var_export($parameter->getDefaultValue(), true) : 'none',
    );
}
?>
--EXPECT--
enum(Gd\AutoCropMode::Default)
float(0.5)
int(-1)
bool(true)
bool(true)
array(6) {
  [0]=>
  string(7) "Default"
  [1]=>
  string(11) "Transparent"
  [2]=>
  string(5) "Black"
  [3]=>
  string(5) "White"
  [4]=>
  string(5) "Sides"
  [5]=>
  string(9) "Threshold"
}
string(7) "GdImage"
options nullable=yes default=NULL
