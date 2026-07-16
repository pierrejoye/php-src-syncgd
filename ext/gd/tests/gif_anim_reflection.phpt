--TEST--
Gd\Gif animation API reflection
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Gif\AnimReader::class)) die('skip GIF animation API not available');
?>
--FILE--
<?php
var_dump(Gd\Gif\DisposalMethod::Unknown->value);
var_dump(Gd\Gif\DisposalMethod::None->value);
var_dump(Gd\Gif\DisposalMethod::RestoreBackground->value);
var_dump(Gd\Gif\DisposalMethod::RestorePrevious->value);

$info = new ReflectionClass(Gd\Gif\Info::class);
$frame = new ReflectionClass(Gd\Gif\Frame::class);
$options = new ReflectionClass(Gd\Gif\FrameOptions::class);
$reader = new ReflectionClass(Gd\Gif\AnimReader::class);
$writer = new ReflectionClass(Gd\Gif\AnimWriter::class);

var_dump($info->isReadOnly(), $frame->isReadOnly(), $options->isReadOnly());
var_dump($reader->isFinal(), $writer->isFinal());
var_dump($reader->getConstructor()->isPrivate(), $writer->getConstructor()->isPrivate());

$defaults = new Gd\Gif\FrameOptions();
var_dump($defaults);
?>
--EXPECTF--
int(0)
int(1)
int(2)
int(3)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
object(Gd\Gif\FrameOptions)#%d (6) {
  ["x"]=>
  int(0)
  ["y"]=>
  int(0)
  ["delayMs"]=>
  int(0)
  ["disposal"]=>
  enum(Gd\Gif\DisposalMethod::None)
  ["localColorMap"]=>
  bool(false)
  ["optimizeAgainstPrevious"]=>
  bool(true)
}
