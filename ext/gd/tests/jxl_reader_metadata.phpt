--TEST--
Gd\Jxl Reader, top-level Info, and still metadata
--EXTENSIONS--
gd
--XLEAK--
--SKIPIF--
<?php
if (!class_exists(Gd\Jxl\Reader::class)) die('skip JXL still Reader API not available');
if (!class_exists(Gd\Metadata::class)) die('skip metadata API not available');
?>
--FILE--
<?php
$image = imagecreatetruecolor(2, 2);
$metadata = Gd\Metadata::create()->with('xmp', '<x>ok</x>');
$bytes = Gd\Jxl\Codec::toString($image, new Gd\Jxl\WriteOptions(
    lossless: true,
    metadata: $metadata,
));

$reader = Gd\Jxl\Reader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->animated, $info->loopCount);
var_dump($info->metadata->keys(), $info->metadata->get('xmp'));
var_dump($reader->read() instanceof GdImage);
try {
    $reader->read();
} catch (Throwable $e) {
    echo $e::class, "\n";
}

$stream = fopen('php://temp', 'w+b');
Gd\Jxl\Codec::toStream($image, $stream, new Gd\Jxl\WriteOptions(lossless: true));
rewind($stream);
var_dump(Gd\Jxl\Reader::fromStream($stream)->info() instanceof Gd\Jxl\Info);
?>
--EXPECT--
int(2)
int(2)
bool(false)
int(0)
array(1) {
  [0]=>
  string(3) "xmp"
}
string(9) "<x>ok</x>"
bool(true)
Gd\Codec\CodecException
bool(true)
