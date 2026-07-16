--TEST--
Gd\Gif animation writer and reader round trip
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Gif\AnimReader::class)) die('skip GIF animation API not available');
?>
--FILE--
<?php
function make_palette_frame(int $r, int $g, int $b): GdImage {
    $im = imagecreate(4, 4);
    $color = imagecolorallocate($im, $r, $g, $b);
    imagefill($im, 0, 0, $color);
    return $im;
}

function rgb_at(GdImage $im): array {
    $c = imagecolorsforindex($im, imagecolorat($im, 0, 0));
    return [$c['red'], $c['green'], $c['blue']];
}

$red = make_palette_frame(255, 0, 0);
$green = make_palette_frame(0, 255, 0);

$writer = Gd\Gif\AnimWriter::toMemory(false, 2);
$writer->addFrame($red, new Gd\Gif\FrameOptions(
    delayMs: 20,
    disposal: Gd\Gif\DisposalMethod::None,
    localColorMap: true,
    optimizeAgainstPrevious: false,
));
$writer->addFrame($green, new Gd\Gif\FrameOptions(
    delayMs: 30,
    disposal: Gd\Gif\DisposalMethod::RestoreBackground,
    localColorMap: true,
    optimizeAgainstPrevious: false,
));
$bytes = $writer->finish();

var_dump(substr($bytes, 0, 6));
var_dump(Gd\Gif\AnimReader::isAnimatedString($bytes));

$reader = Gd\Gif\AnimReader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->globalColorTable, $info->loopCount);

$first = $reader->next();
$firstImage = $first->image;
$second = $reader->next();

var_dump($first->frameIndex, $first->delayMs, $first->disposal, $first->disposalTag, $first->localColorTable);
var_dump($second->frameIndex, $second->delayMs, $second->disposal, $second->disposalTag, $second->localColorTable);
var_dump(rgb_at($firstImage), rgb_at($second->image));
var_dump($reader->next(), $reader->next());
var_dump(rgb_at($firstImage));
unset($reader);
var_dump(rgb_at($firstImage));
?>
--EXPECT--
string(6) "GIF89a"
bool(true)
int(4)
int(4)
bool(false)
int(2)
int(0)
int(20)
enum(Gd\Gif\DisposalMethod::None)
int(1)
bool(true)
int(1)
int(30)
enum(Gd\Gif\DisposalMethod::RestoreBackground)
int(2)
bool(true)
array(3) {
  [0]=>
  int(255)
  [1]=>
  int(0)
  [2]=>
  int(0)
}
array(3) {
  [0]=>
  int(0)
  [1]=>
  int(255)
  [2]=>
  int(0)
}
NULL
NULL
array(3) {
  [0]=>
  int(255)
  [1]=>
  int(0)
  [2]=>
  int(0)
}
array(3) {
  [0]=>
  int(255)
  [1]=>
  int(0)
  [2]=>
  int(0)
}
