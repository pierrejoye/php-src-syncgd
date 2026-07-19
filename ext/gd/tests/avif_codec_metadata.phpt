--TEST--
Gd\Avif metadata read and write round trip
--EXTENSIONS--
gd
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Avif\Reader::class)) die('skip AVIF metadata API not available');
if (!is_file(__DIR__ . '/avif_metadata.avif')) die('skip AVIF metadata fixture not available');
try {
    Gd\Avif\Reader::fromFile(__DIR__ . '/avif_metadata.avif')->read();
} catch (Throwable $e) {
    die('skip AVIF decoder unavailable: ' . $e->getMessage());
}
?>
--FILE--
<?php
$reader = Gd\Avif\Reader::fromFile(__DIR__ . '/avif_metadata.avif');
$info = $reader->info();
var_dump($info->width, $info->height);
var_dump($info->isAnimation, $info->isProgressive, $info->frameCount);
var_dump($info->duration, $info->hasAlpha, $info->bitDepth, $info->yuvFormat);

$bytes = file_get_contents(__DIR__ . '/avif_metadata.avif');
var_dump(Gd\Avif\Reader::fromString($bytes)->info()->width === 924);
$stream = fopen('php://memory', 'w+b');
fwrite($stream, $bytes);
rewind($stream);
var_dump(Gd\Avif\Reader::fromStream($stream)->info()->height === 942);
fclose($stream);

$metadata = $info->metadata;
$keys = $metadata->keys();
sort($keys);
var_dump($keys);
var_dump(hash('sha256', $metadata->get('exif')));
var_dump(hash('sha256', $metadata->get('xmp')));

/* AVIF's four-byte EXIF container prefix is not part of the public value. */
var_dump(bin2hex(substr($metadata->get('exif'), 0, 4)));

$changed = $metadata
    ->with('xmp', '<x:xmpmeta xmlns:x="adobe:ns:meta/"><rdf:RDF/></x:xmpmeta>')
    ->with('icc', 'ignored color profile');
$file = tempnam(sys_get_temp_dir(), 'avif-metadata-');
Gd\Avif\Codec::toFile($reader->read(), $file, new Gd\Avif\WriteOptions(metadata: $changed));

$roundTrip = Gd\Avif\Reader::fromFile($file)->info()->metadata;
var_dump($roundTrip->get('exif') === $changed->get('exif'));
var_dump($roundTrip->get('xmp') === $changed->get('xmp'));
var_dump($roundTrip->has('icc'), $roundTrip->has('iptc'));
unlink($file);
?>
--EXPECT--
int(924)
int(942)
bool(false)
bool(false)
int(1)
float(1)
bool(false)
int(8)
int(3)
bool(true)
bool(true)
array(2) {
  [0]=>
  string(4) "exif"
  [1]=>
  string(3) "xmp"
}
string(64) "550b46a1380ab1730a3a32b068db8a19e50bfd60c55b3e9c49f03f0d17f96d97"
string(64) "4f64a8160825b493396d9873aff61c0f95956dd2c6553fd28b9ca8c48207bdb5"
string(8) "00000006"
bool(true)
bool(true)
bool(false)
bool(false)
