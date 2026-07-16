--TEST--
Gd PNG codec writes files, streams, and strings with options
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Codec::class)) {
    die('skip PNG write options API is unavailable');
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(3, 2);
$red = imagecolorallocate($image, 255, 0, 0);
imagefilledrectangle($image, 0, 0, 2, 1, $red);

$options = new Gd\Png\WriteOptions(
    compressionLevel: 6,
    filters: [Gd\Png\Filter::Sub, Gd\Png\Filter::Paeth],
    compressionStrategy: Gd\Png\CompressionStrategy::Filtered,
);

$bytes = Gd\Png\Codec::toString($image, $options);
$decoded = imagecreatefromstring($bytes);
var_dump(substr($bytes, 0, 8) === "\x89PNG\r\n\x1a\n");
var_dump(imagesx($decoded), imagesy($decoded), imagecolorat($decoded, 1, 1) === $red);

$stream = fopen('php://temp', 'w+b');
Gd\Png\Codec::toStream($image, $stream);
var_dump(is_resource($stream), ftell($stream) > 8);

$file = __DIR__ . '/png_codec_encode.png';
Gd\Png\Codec::toFile($image, $file);
$decoded = imagecreatefrompng($file);
var_dump(imagesx($decoded), imagesy($decoded));

foreach (Gd\Png\CompressionStrategy::cases() as $strategy) {
    $encoded = Gd\Png\Codec::toString($image, new Gd\Png\WriteOptions(compressionStrategy: $strategy));
    var_dump(imagecreatefromstring($encoded) instanceof GdImage);
}

foreach (Gd\Png\Filter::cases() as $filter) {
    $encoded = Gd\Png\Codec::toString($image, new Gd\Png\WriteOptions(filters: [$filter]));
    var_dump(imagecreatefromstring($encoded) instanceof GdImage);
}
?>
--CLEAN--
<?php @unlink(__DIR__ . '/png_codec_encode.png'); ?>
--EXPECT--
bool(true)
int(3)
int(2)
bool(true)
bool(true)
bool(true)
int(3)
int(2)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
