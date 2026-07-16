--TEST--
Gd\Webp animation writer and reader round trip
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Webp\AnimReader::class)) die('skip WebP animation API not available');
?>
--FILE--
<?php
function make_webp_frame(int $r, int $g, int $b): GdImage {
    $im = imagecreatetruecolor(4, 4);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    $color = imagecolorallocatealpha($im, $r, $g, $b, 0);
    imagefill($im, 0, 0, $color);
    return $im;
}

function rgb_at(GdImage $im): array {
    $c = imagecolorsforindex($im, imagecolorat($im, 0, 0));
    return [$c['red'], $c['green'], $c['blue']];
}

$red = make_webp_frame(255, 0, 0);
$blue = make_webp_frame(0, 0, 255);

$writer = Gd\Webp\AnimWriter::toMemory(new Gd\Webp\AnimWriteOptions(
    loopCount: 2,
    lossless: true,
));
$writer->addFrame($red, 100);
$writer->addFrame($blue, 80);
$bytes = $writer->finish();

var_dump(substr($bytes, 0, 4));
var_dump(Gd\Webp\AnimReader::isAnimatedString($bytes));

$reader = Gd\Webp\AnimReader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->frameCount, $info->loopCount);

$first = $reader->next();
$firstImage = $first->image;
$second = $reader->next();

var_dump($first->frameIndex, $first->durationMs, $first->timestampMs, $first->dispose, $first->blend);
var_dump($second->frameIndex, $second->durationMs, $second->timestampMs, $second->dispose, $second->blend);
var_dump(rgb_at($firstImage), rgb_at($second->image));
var_dump($reader->next(), $reader->next());
var_dump(rgb_at($firstImage));
unset($reader);
var_dump(rgb_at($firstImage));
?>
--EXPECT--
string(4) "RIFF"
bool(true)
int(4)
int(4)
int(2)
int(2)
int(0)
int(100)
int(0)
enum(Gd\Webp\DisposeMethod::None)
enum(Gd\Webp\BlendMethod::None)
int(1)
int(80)
int(100)
enum(Gd\Webp\DisposeMethod::None)
enum(Gd\Webp\BlendMethod::Alpha)
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
  int(0)
  [2]=>
  int(255)
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
