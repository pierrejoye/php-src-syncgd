--TEST--
Gd\Webp animation reflection
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Webp\AnimWriter::class)) die('skip WebP animation API not available');
?>
--FILE--
<?php
$options = new Gd\Webp\AnimWriteOptions();
var_dump($options);
var_dump(array_map(fn($c) => [$c->name, $c->value], Gd\Webp\DisposeMethod::cases()));
var_dump(array_map(fn($c) => [$c->name, $c->value], Gd\Webp\BlendMethod::cases()));

foreach ([Gd\Webp\Info::class, Gd\Webp\Frame::class, Gd\Webp\AnimWriteOptions::class] as $class) {
    $ref = new ReflectionClass($class);
    echo $ref->getShortName(), ':';
    var_dump($ref->isFinal(), $ref->isReadOnly());
}

foreach ([Gd\Webp\AnimReader::class, Gd\Webp\AnimWriter::class] as $class) {
    $ref = new ReflectionClass($class);
    echo $ref->getShortName(), ':';
    var_dump($ref->isFinal(), $ref->getConstructor()->isPrivate(), $ref->isCloneable());
    try {
        serialize($ref->newInstanceWithoutConstructor());
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

$writer = new ReflectionClass(Gd\Webp\AnimWriter::class);
foreach (['toFile', 'toStream', 'toMemory', 'addFrame', 'finish'] as $name) {
    $method = $writer->getMethod($name);
    echo $name, ':', $method->getReturnType(), ':', $method->getNumberOfRequiredParameters(), "\n";
}

$reader = new ReflectionClass(Gd\Webp\AnimReader::class);
foreach (['fromFile', 'fromString', 'fromStream', 'isAnimatedFile', 'isAnimatedString', 'isAnimatedStream', 'info', 'next'] as $name) {
    $method = $reader->getMethod($name);
    echo $name, ':', $method->getReturnType(), ':', $method->getNumberOfRequiredParameters(), "\n";
}
?>
--EXPECTF--
object(Gd\Webp\AnimWriteOptions)#%d (11) {
  ["canvasWidth"]=>
  int(0)
  ["canvasHeight"]=>
  int(0)
  ["loopCount"]=>
  int(0)
  ["backgroundColor"]=>
  int(0)
  ["quality"]=>
  int(-1)
  ["lossless"]=>
  bool(false)
  ["method"]=>
  int(4)
  ["minimizeSize"]=>
  bool(false)
  ["kmin"]=>
  int(9)
  ["kmax"]=>
  int(17)
  ["allowMixed"]=>
  bool(false)
}
array(2) {
  [0]=>
  array(2) {
    [0]=>
    string(4) "None"
    [1]=>
    int(0)
  }
  [1]=>
  array(2) {
    [0]=>
    string(10) "Background"
    [1]=>
    int(1)
  }
}
array(2) {
  [0]=>
  array(2) {
    [0]=>
    string(5) "Alpha"
    [1]=>
    int(0)
  }
  [1]=>
  array(2) {
    [0]=>
    string(4) "None"
    [1]=>
    int(1)
  }
}
Info:bool(true)
bool(true)
Frame:bool(true)
bool(true)
AnimWriteOptions:bool(true)
bool(true)
AnimReader:bool(true)
bool(true)
bool(false)
ReflectionException
AnimWriter:bool(true)
bool(true)
bool(false)
ReflectionException
toFile:Gd\Webp\AnimWriter:1
toStream:Gd\Webp\AnimWriter:1
toMemory:Gd\Webp\AnimWriter:0
addFrame:static:2
finish:?string:0
fromFile:Gd\Webp\AnimReader:1
fromString:Gd\Webp\AnimReader:1
fromStream:Gd\Webp\AnimReader:1
isAnimatedFile:bool:1
isAnimatedString:bool:1
isAnimatedStream:bool:1
info:Gd\Webp\Info:0
next:?Gd\Webp\Frame:0
