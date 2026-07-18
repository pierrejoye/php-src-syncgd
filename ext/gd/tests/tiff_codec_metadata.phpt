--TEST--
TIFF opaque metadata round trip
--EXTENSIONS--
gd
--FILE--
<?php
$payload = pack('e*', 1.5, 2.5, 3.5);
$profile = pack('a4CCvVV', 'GDTF', 1, 1, 12, 3, strlen($payload)) . $payload;
$metadata = Gd\Metadata::create()->with('tiff:tag:33550', $profile);
$image = Gd\Image::create(2, 2);
$bytes = Gd\Tiff\Codec::toString($image, new Gd\Tiff\WriteOptions(
    compression: Gd\Tiff\Compression::AdobeDeflate,
    metadata: $metadata,
));
$roundTrip = Gd\Tiff\Reader::fromString($bytes)->info()->metadata;

var_dump($roundTrip->keys(), $roundTrip->get('tiff:tag:33550') === $profile);
--EXPECT--
array(1) {
  [0]=>
  string(14) "tiff:tag:33550"
}
bool(true)
