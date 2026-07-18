--TEST--
Gd\Bmp\Info and Reader
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists('Gd\\Bmp\\Reader')) die('skip bundled BMP Reader API not available');
?>
--FILE--
<?php
$image = imagecreatetruecolor(3, 2);
$bytes = Gd\Bmp\Codec::toString($image, new Gd\Bmp\WriteOptions(bitsPerPixel: 24));
$reader = Gd\Bmp\Reader::fromString($bytes, new Gd\Bmp\ReadOptions());
$info = $reader->info();

var_dump($info instanceof Gd\Bmp\Info);
var_dump($info->width, $info->height, $info->bitsPerPixel);
var_dump($info->compressionTag, $info->topDown, $info->headerSize, $info->headerType);
var_dump($info->pixelOffset > $info->headerSize, $info->fileSize === strlen($bytes));
var_dump($reader->info() === $reader->info());
var_dump($reader->read() instanceof GdImage);
try {
    $reader->read();
} catch (Throwable $e) {
    echo $e::class, "\n";
}

$metadata = Gd\Metadata::create()->with('xmp', 'ignored');
$withMetadata = Gd\Bmp\Codec::toString($image, new Gd\Bmp\WriteOptions(metadata: $metadata));
var_dump($withMetadata === Gd\Bmp\Codec::toString($image));
?>
--EXPECT--
bool(true)
int(3)
int(2)
int(24)
int(0)
bool(false)
int(40)
int(1)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
bool(true)
