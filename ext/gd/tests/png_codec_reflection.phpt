--TEST--
Gd PNG codec API shape
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Codec::class)) {
    die('skip PNG write options API is unavailable');
}
?>
--FILE--
<?php
var_dump(get_parent_class(Gd\GdException::class));
var_dump(is_subclass_of(Gd\Codec\CodecException::class, Gd\GdException::class));

$options = new Gd\Png\WriteOptions();
var_dump($options->compressionLevel);
var_dump($options->filters);
var_dump($options->compressionStrategy);
var_dump(new Gd\Png\ReadOptions() instanceof Gd\Png\ReadOptions);

$optionsReflection = new ReflectionClass(Gd\Png\WriteOptions::class);
$codecReflection = new ReflectionClass(Gd\Png\Codec::class);
var_dump($optionsReflection->isFinal(), $optionsReflection->isReadOnly());
var_dump($codecReflection->isFinal(), $codecReflection->getConstructor()->isPrivate());

var_dump(array_column(Gd\Png\Filter::cases(), 'name'));
var_dump(array_column(Gd\Png\CompressionStrategy::cases(), 'name'));

foreach (['toFile', 'toStream', 'toString'] as $method) {
    $reflection = $codecReflection->getMethod($method);
    echo $method, ':', $reflection->getReturnType(), ':', $reflection->getNumberOfRequiredParameters(), "\n";
}

foreach ([Gd\Png\Reader::class, Gd\Png\Codec::class] as $class) {
    foreach (['fromFile', 'fromString', 'fromStream'] as $method) {
        $reflection = new ReflectionMethod($class, $method);
        $parameter = $reflection->getParameters()[1];
        echo $class, '::', $method, ':', $parameter->getType(), ':', $parameter->isDefaultValueAvailable(), "\n";
    }
}
?>
--EXPECT--
string(9) "Exception"
bool(true)
int(-1)
array(0) {
}
enum(Gd\Png\CompressionStrategy::Default)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
array(5) {
  [0]=>
  string(4) "None"
  [1]=>
  string(3) "Sub"
  [2]=>
  string(2) "Up"
  [3]=>
  string(7) "Average"
  [4]=>
  string(5) "Paeth"
}
array(5) {
  [0]=>
  string(7) "Default"
  [1]=>
  string(8) "Filtered"
  [2]=>
  string(11) "HuffmanOnly"
  [3]=>
  string(3) "Rle"
  [4]=>
  string(5) "Fixed"
}
toFile:void:2
toStream:void:2
toString:string:1
Gd\Png\Reader::fromFile:Gd\Png\ReadOptions:1
Gd\Png\Reader::fromString:Gd\Png\ReadOptions:1
Gd\Png\Reader::fromStream:Gd\Png\ReadOptions:1
Gd\Png\Codec::fromFile:Gd\Png\ReadOptions:1
Gd\Png\Codec::fromString:Gd\Png\ReadOptions:1
Gd\Png\Codec::fromStream:Gd\Png\ReadOptions:1
