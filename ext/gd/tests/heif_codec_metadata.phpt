--TEST--
Gd\Heif metadata read and write round trip
--EXTENSIONS--
gd
--XLEAK--
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Heif\Reader::class)) die('skip HEIF metadata API not available');
if (!is_file(__DIR__ . '/heif_metadata.heic')) die('skip HEIF metadata fixture not available');
try {
    Gd\Heif\Reader::fromFile(__DIR__ . '/heif_metadata.heic')->read();
} catch (Throwable $e) {
    die('skip HEIF decoder unavailable: ' . $e->getMessage());
}
?>
--FILE--
<?php
$reader = Gd\Heif\Reader::fromFile(__DIR__ . '/heif_metadata.heic');
$info = $reader->info();
var_dump($info->width, $info->height);
var_dump($info->topLevelImageCount, $info->hasAlpha, $info->bitDepth, $info->isAnimation);
$bytes = file_get_contents(__DIR__ . '/heif_metadata.heic');
var_dump(Gd\Heif\Reader::fromString($bytes)->info()->width === 924);
$stream = fopen('php://memory', 'w+b');
fwrite($stream, $bytes);
rewind($stream);
var_dump(Gd\Heif\Reader::fromStream($stream)->info()->height === 941);
fclose($stream);

$metadata = $info->metadata;
$keys = $metadata->keys();
sort($keys);
var_dump($keys);

foreach ([
    'exif' => '995837e4b7a256c310a3fbebf1cb8dcbd214e31275ba99405683dbd3d63041b1',
    'xmp' => '4f64a8160825b493396d9873aff61c0f95956dd2c6553fd28b9ca8c48207bdb5',
    'iptc' => 'cd275e5a511b2d3b1fffdc9513e28d11a7eca0286767246a5c471ef4f4839fc3',
] as $key => $hash) {
    var_dump(hash('sha256', $metadata->get($key)) === $hash);
}
var_dump($metadata->has('icc'));

/* HEIF's four-byte EXIF container prefix is not part of the public value. */
var_dump(bin2hex(substr($metadata->get('exif'), 0, 4)));

$changed = $metadata
    ->with('iptc', 'changed opaque IPTC')
    ->with('icc', 'ignored color profile');
$file = tempnam(sys_get_temp_dir(), 'heif-metadata-');
Gd\Heif\Codec::toFile($reader->read(), $file, new Gd\Heif\WriteOptions(
    quality: 20,
    metadata: $changed,
));

$roundTrip = Gd\Heif\Reader::fromFile($file)->info()->metadata;
var_dump($roundTrip->get('exif') === $changed->get('exif'));
var_dump($roundTrip->get('xmp') === $changed->get('xmp'));
var_dump($roundTrip->has('icc'));
var_dump($roundTrip->get('iptc') === $changed->get('iptc'));
unlink($file);
?>
--EXPECT--
int(924)
int(941)
int(1)
bool(true)
int(8)
bool(false)
bool(true)
bool(true)
array(3) {
  [0]=>
  string(4) "exif"
  [1]=>
  string(4) "iptc"
  [2]=>
  string(3) "xmp"
}
bool(true)
bool(true)
bool(true)
bool(false)
string(8) "4d4d002a"
bool(true)
bool(true)
bool(false)
bool(true)
