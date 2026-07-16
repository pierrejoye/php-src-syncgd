--TEST--
Gd TIFF writer API shape and defaults
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Writer::class)) {
    die('skip TIFF writer API is unavailable');
}
?>
--FILE--
<?php
$options = new Gd\Tiff\WriteOptions();
var_dump($options);

var_dump(array_column(Gd\Tiff\ColorSpace::cases(), 'name'));
var_dump(array_column(Gd\Tiff\Compression::cases(), 'name'));
var_dump(array_column(Gd\Tiff\Compression::cases(), 'value'));
var_dump(array_column(Gd\Tiff\ResolutionUnit::cases(), 'name'));

$optionsReflection = new ReflectionClass(Gd\Tiff\WriteOptions::class);
$writerReflection = new ReflectionClass(Gd\Tiff\Writer::class);
var_dump($optionsReflection->isFinal(), $optionsReflection->isReadOnly());
var_dump($writerReflection->isFinal(), $writerReflection->isCloneable());
var_dump($writerReflection->getConstructor()->isPrivate());

$writer = Gd\Tiff\Writer::toMemory();
try {
    serialize($writer);
} catch (Exception $e) {
    echo $e->getMessage(), "\n";
}

foreach (['toFile', 'toStream', 'toMemory', 'addPage', 'finish'] as $name) {
    $method = $writerReflection->getMethod($name);
    printf("%s:%s:%d\n", $name, $method->getReturnType(), $method->getNumberOfRequiredParameters());
}
?>
--EXPECT--
object(Gd\Tiff\WriteOptions)#1 (6) {
  ["compression"]=>
  enum(Gd\Tiff\Compression::Deflate)
  ["colorSpace"]=>
  enum(Gd\Tiff\ColorSpace::Rgba)
  ["minIsWhite"]=>
  bool(false)
  ["resolutionUnit"]=>
  enum(Gd\Tiff\ResolutionUnit::Inch)
  ["xResolution"]=>
  NULL
  ["yResolution"]=>
  NULL
}
array(3) {
  [0]=>
  string(3) "Rgb"
  [1]=>
  string(4) "Rgba"
  [2]=>
  string(4) "Gray"
}
array(9) {
  [0]=>
  string(4) "None"
  [1]=>
  string(8) "CcittRle"
  [2]=>
  string(9) "CcittFax3"
  [3]=>
  string(9) "CcittFax4"
  [4]=>
  string(3) "Lzw"
  [5]=>
  string(4) "Jpeg"
  [6]=>
  string(12) "AdobeDeflate"
  [7]=>
  string(7) "Deflate"
  [8]=>
  string(8) "PackBits"
}
array(9) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
  [4]=>
  int(5)
  [5]=>
  int(7)
  [6]=>
  int(8)
  [7]=>
  int(32946)
  [8]=>
  int(32773)
}
array(3) {
  [0]=>
  string(4) "None"
  [1]=>
  string(4) "Inch"
  [2]=>
  string(10) "Centimeter"
}
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
Serialization of 'Gd\Tiff\Writer' is not allowed
toFile:Gd\Tiff\Writer:1
toStream:Gd\Tiff\Writer:1
toMemory:Gd\Tiff\Writer:0
addPage:static:1
finish:?string:0
