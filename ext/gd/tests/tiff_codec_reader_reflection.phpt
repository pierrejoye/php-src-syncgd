--TEST--
Gd\Tiff reader API reflection and object restrictions
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Reader::class)) die('skip TIFF reader API unavailable');
?>
--FILE--
<?php
foreach ([Gd\Tiff\Info::class, Gd\Tiff\Page::class] as $class) {
    $reflection = new ReflectionClass($class);
    var_dump($reflection->isFinal(), $reflection->isReadOnly());
}

$readerReflection = new ReflectionClass(Gd\Tiff\Reader::class);
var_dump($readerReflection->isFinal());
var_dump($readerReflection->getConstructor()->isPrivate());
var_dump($readerReflection->implementsInterface(Iterator::class));
var_dump(array_column(Gd\Tiff\Photometric::cases(), 'name'));
var_dump(array_column(Gd\Tiff\Photometric::cases(), 'value'));
var_dump(array_column(Gd\Tiff\PlanarConfiguration::cases(), 'name'));
var_dump(array_column(Gd\Tiff\PlanarConfiguration::cases(), 'value'));

$image = imagecreatetruecolor(1, 1);
$writer = Gd\Tiff\Writer::toMemory(new Gd\Tiff\WriteOptions(compression: Gd\Tiff\Compression::AdobeDeflate));
$reader = Gd\Tiff\Reader::fromString($writer->addPage($image)->finish());
foreach ([
    fn() => clone $reader,
    fn() => serialize($reader),
] as $operation) {
    try {
        $operation();
    } catch (Throwable $e) {
        echo get_class($e), "\n";
    }
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
array(8) {
  [0]=>
  string(10) "MinIsWhite"
  [1]=>
  string(10) "MinIsBlack"
  [2]=>
  string(3) "Rgb"
  [3]=>
  string(7) "Palette"
  [4]=>
  string(16) "TransparencyMask"
  [5]=>
  string(9) "Separated"
  [6]=>
  string(5) "YCbCr"
  [7]=>
  string(6) "CieLab"
}
array(8) {
  [0]=>
  int(0)
  [1]=>
  int(1)
  [2]=>
  int(2)
  [3]=>
  int(3)
  [4]=>
  int(4)
  [5]=>
  int(5)
  [6]=>
  int(6)
  [7]=>
  int(8)
}
array(2) {
  [0]=>
  string(10) "Contiguous"
  [1]=>
  string(8) "Separate"
}
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
Error
Exception
