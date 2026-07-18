--TEST--
Gd\Webp\Reader inspects WebP containers and metadata
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Webp\Reader::class)) die('skip bundled WebP reader not available');
?>
--FILE--
<?php
$image = imagecreatetruecolor(4, 3);
$metadata = Gd\Metadata::create()
    ->with('exif', "Exif\0\0opaque")
    ->with('xmp', '<xmp>opaque</xmp>')
    ->with('icc', "\x01\x02\x03\x04");
$bytes = Gd\Webp\Codec::toString($image, new Gd\Webp\WriteOptions(metadata: $metadata));

$reader = Gd\Webp\Reader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->frameCount);
var_dump($info->metadata->keys());
var_dump($info->metadata->get('exif') === $metadata->get('exif'));
var_dump($info->metadata->get('xmp') === $metadata->get('xmp'));
var_dump($info->metadata->get('icc') === $metadata->get('icc'));
var_dump($reader->read() instanceof GdImage);
try {
    $reader->read();
} catch (Throwable $e) {
    echo $e::class, "\n";
}

$file = tempnam(sys_get_temp_dir(), 'webp-reader-');
file_put_contents($file, $bytes);
var_dump(Gd\Webp\Reader::fromFile($file)->info()->width);
$stream = fopen('php://memory', 'w+b');
fwrite($stream, $bytes);
rewind($stream);
var_dump(Gd\Webp\Reader::fromStream($stream)->info()->height);
fclose($stream);
unlink($file);

foreach (['not webp', substr($bytes, 0, 16)] as $invalid) {
    try {
        Gd\Webp\Reader::fromString($invalid);
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

if (class_exists(Gd\Webp\AnimWriter::class)) {
    $second = imagecreatetruecolor(4, 3);
    imagefill($second, 0, 0, 0xff0000);
    $animated = Gd\Webp\AnimWriter::toMemory(new Gd\Webp\AnimWriteOptions(loopCount: 2, lossless: true))
        ->addFrame($image, 10)
        ->addFrame($second, 20)
        ->finish();
    $genericInfo = Gd\Webp\Reader::fromString($animated)->info();
    $animInfo = Gd\Webp\AnimReader::fromString($animated)->info();
    var_dump($genericInfo->frameCount, $genericInfo->loopCount);
    var_dump($animInfo->frameCount, $animInfo->loopCount, $genericInfo->metadata->count());
    var_dump(Gd\Webp\Reader::fromString($animated)->read() instanceof GdImage);
}
?>
--EXPECT--
int(4)
int(3)
int(1)
array(3) {
  [0]=>
  string(3) "icc"
  [1]=>
  string(3) "xmp"
  [2]=>
  string(4) "exif"
}
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
int(4)
int(3)
Gd\Codec\CodecException
Gd\Codec\CodecException
int(2)
int(2)
int(2)
int(2)
int(0)
bool(true)
