--TEST--
Gd\Gif generic Reader and Info
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Gif\Reader::class)) die('skip bundled GIF Reader not available');
?>
--FILE--
<?php
$im = imagecreate(2, 3);
imagecolorallocate($im, 1, 2, 3);
ob_start();
imagegif($im);
$gif87 = ob_get_clean();

$info = Gd\Gif\Reader::fromString($gif87)->info();
var_dump($info->version, $info->width, $info->height, $info->globalColorTable,
    $info->colorResolution, $info->pixelAspectRatio, $info->loopCount);

$gifAspect = substr_replace($gif87, chr(49), 12, 1);
$aspectInfo = Gd\Gif\Reader::fromString($gifAspect)->info();
var_dump($aspectInfo->pixelAspectRatio);

$gif89 = "GIF89a" . substr($gif87, 6);
var_dump(Gd\Gif\Reader::fromString($gif89)->info()->version);

$path = tempnam(sys_get_temp_dir(), 'gif-info-');
file_put_contents($path, $gif87);
var_dump(Gd\Gif\Reader::fromFile($path)->read() instanceof GdImage);
unlink($path);

$stream = fopen('php://temp', 'w+b');
fwrite($stream, $gif87);
rewind($stream);
var_dump(Gd\Gif\Reader::fromStream($stream)->read() instanceof GdImage);

$writer = Gd\Gif\AnimWriter::toMemory(false, 2);
$writer->addFrame($im);
$animated = $writer->finish();
$animatedInfo = Gd\Gif\Reader::fromString($animated)->info();
var_dump($animatedInfo->loopCount);

$animInfo = Gd\Gif\AnimReader::fromString($animated)->info();
var_dump($animInfo->version, $animInfo->loopCount, $animInfo->pixelAspectRatio);
?>
--EXPECT--
string(3) "87a"
int(2)
int(3)
bool(true)
int(1)
float(1)
NULL
float(1)
string(3) "89a"
bool(true)
bool(true)
int(2)
string(3) "89a"
int(2)
float(1)
