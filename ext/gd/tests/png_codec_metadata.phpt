--TEST--
Gd PNG codec writes and reads metadata through WriteOptions
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Codec::class) || !class_exists(Gd\Metadata::class)) {
    die('skip PNG metadata API is unavailable');
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(2, 2);
$exif = "\x4d\x4d\x00\x2a\x00\x00\x00\x08";
$metadata = Gd\Metadata::create()
    ->with('exif', $exif)
    ->with('icc', 'icc profile')
    ->with('xmp', '<xmp>png test</xmp>');
$options = new Gd\Png\WriteOptions(
    metadata: $metadata,
    comments: ['Title' => 'PNG metadata test'],
);

$readInfo = static function (Gd\Png\Reader $reader): void {
    $info = $reader->info();
    var_dump($info->comments);
    var_dump(bin2hex($info->metadata->get('exif')));
    var_dump($info->metadata->get('icc'));
    var_dump(bin2hex($info->metadata->get('xmp')));
};

$readInfo(Gd\Png\Reader::fromString(Gd\Png\Codec::toString($image, $options)));

$stream = fopen('php://temp', 'w+b');
Gd\Png\Codec::toStream($image, $stream, $options);
rewind($stream);
$readInfo(Gd\Png\Reader::fromStream($stream));
var_dump(is_resource($stream));

$file = __DIR__ . '/png_codec_metadata.png';
Gd\Png\Codec::toFile($image, $file, $options);
$readInfo(Gd\Png\Reader::fromFile($file));
?>
--CLEAN--
<?php @unlink(__DIR__ . '/png_codec_metadata.png'); ?>
--EXPECT--
array(1) {
  ["Title"]=>
  string(17) "PNG metadata test"
}
string(28) "4578696600004d4d002a00000008"
NULL
string(38) "3c786d703e706e6720746573743c2f786d703e"
array(1) {
  ["Title"]=>
  string(17) "PNG metadata test"
}
string(28) "4578696600004d4d002a00000008"
NULL
string(38) "3c786d703e706e6720746573743c2f786d703e"
bool(true)
array(1) {
  ["Title"]=>
  string(17) "PNG metadata test"
}
string(28) "4578696600004d4d002a00000008"
NULL
string(38) "3c786d703e706e6720746573743c2f786d703e"
