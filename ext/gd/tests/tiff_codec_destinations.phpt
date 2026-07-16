--TEST--
Gd TIFF writer file and stream destinations and lifecycle
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Writer::class)) {
    die('skip TIFF writer API is unavailable');
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(2, 2);
$green = imagecolorallocate($image, 0, 255, 0);
imagefilledrectangle($image, 0, 0, 1, 1, $green);
$quietOptions = new Gd\Tiff\WriteOptions(compression: Gd\Tiff\Compression::AdobeDeflate);

$stream = fopen('php://temp', 'w+b');
$writer = Gd\Tiff\Writer::toStream($stream, $quietOptions);
$writer->addPage($image);
var_dump($writer->finish());
var_dump(is_resource($stream), ftell($stream) > 8);

$path = __DIR__ . '/tiff_codec_destinations.tiff';
$writer = Gd\Tiff\Writer::toFile($path, $quietOptions);
$writer->addPage($image);
var_dump($writer->finish());
var_dump(substr(file_get_contents($path), 0, 2));

try {
    $writer->finish();
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}
try {
    $writer->addPage($image);
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}

$empty = Gd\Tiff\Writer::toMemory($quietOptions);
try {
    $empty->finish();
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}
$empty->addPage($image);
var_dump(is_string($empty->finish()));
?>
--CLEAN--
<?php @unlink(__DIR__ . '/tiff_codec_destinations.tiff'); ?>
--EXPECT--
NULL
bool(true)
bool(true)
NULL
string(2) "II"
TIFF writer has already been finalized
TIFF writer has already been finalized
Cannot finalize a TIFF writer without pages
bool(true)
