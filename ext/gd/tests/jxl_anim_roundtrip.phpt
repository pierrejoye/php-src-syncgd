--TEST--
Gd\Jxl animation writer and reader round trip
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Jxl\AnimReader::class)) die('skip JXL animation API not available');
?>
--XLEAK--
libjxl 0.11 emits valgrind uninitialized-value reports while encoding/decoding JXL frames; no heap leak remains.
--FILE--
<?php
function frame(int $r, int $g, int $b): GdImage {
    $im = imagecreatetruecolor(3, 3);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    imagefill($im, 0, 0, imagecolorallocatealpha($im, $r, $g, $b, 0));
    return $im;
}

$red = frame(255, 0, 0);
$green = frame(0, 255, 0);

$bytes = Gd\Jxl\AnimWriter::toMemory(new Gd\Jxl\AnimWriteOptions(loopCount: 3, lossless: true))
    ->addFrame($red, 40)
    ->addFrame($green, 60)
    ->finish();
var_dump(strlen($bytes) > 0);

$reader = Gd\Jxl\AnimReader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->animated, $info->loopCount);

$first = $reader->next();
$second = $reader->next();
var_dump($first->frameIndex, $first->delayMs, $first->timestampMs);
var_dump($second->frameIndex, $second->delayMs, $second->timestampMs);
var_dump($reader->next(), $reader->next());

$firstPixel = imagecolorat($first->image, 1, 1);
unset($reader, $second);
var_dump(imagecolorat($first->image, 1, 1) === $firstPixel);

$file = tempnam(sys_get_temp_dir(), 'jxl-anim-');
Gd\Jxl\AnimWriter::toFile($file, new Gd\Jxl\AnimWriteOptions(lossless: true))
    ->addFrame($red, 10)
    ->finish();
var_dump(Gd\Jxl\AnimReader::fromFile($file)->next() instanceof Gd\Jxl\Frame);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Jxl\AnimWriter::toStream($stream, new Gd\Jxl\AnimWriteOptions(lossless: true))
    ->addFrame($red, 10)
    ->finish();
rewind($stream);
var_dump(Gd\Jxl\AnimReader::fromStream($stream)->next() instanceof Gd\Jxl\Frame);
?>
--EXPECT--
bool(true)
int(3)
int(3)
bool(true)
int(3)
int(0)
int(40)
int(0)
int(1)
int(60)
int(40)
NULL
NULL
bool(true)
bool(true)
bool(true)
