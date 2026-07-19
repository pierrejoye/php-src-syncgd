--TEST--
Gd\Jpeg\Codec and Gd\Jpeg\Reader basic behavior
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Jpeg\Codec::class)) die('skip Gd\Jpeg\Codec not available');
?>
--FILE--
<?php
use Gd\Codec\CodecException;
use Gd\Jpeg\Codec;
use Gd\Jpeg\ColorSpace;
use Gd\Jpeg\DensityUnit;
use Gd\Jpeg\Reader;
use Gd\Jpeg\ReadOptions;
use Gd\Jpeg\WriteOptions;

function show_exception(callable $callback): void {
    try {
        $callback();
    } catch (Throwable $e) {
        echo $e::class, ': ', $e->getMessage(), "\n";
    }
}

$im = imagecreatetruecolor(4, 3);
imageresolution($im, 123, 234);
imagefill($im, 0, 0, 0x336699);

$xmp = "<x:xmpmeta/>";
$iptc = "\x1c\x02\x05\0\x04test";
$exif = "\x4d\x4d\x00\x2a\x00\x00\x00\x08";
$metadata = Gd\Metadata::create()
    ->with('exif', $exif)
    ->with('xmp', $xmp)
    ->with('icc', 'icc-profile')
    ->with('iptc', $iptc);

$bytes = Codec::toString($im, new WriteOptions(
    quality: 90,
    progressive: true,
    forceNoSubsampling: true,
    metadata: $metadata,
));

var_dump(str_starts_with($bytes, "\xff\xd8"));

$reader = Reader::fromString($bytes, new ReadOptions(ignoreWarnings: true));
$info = $reader->info();
var_dump($info->width, $info->height, $info->bitsPerSample, $info->components);
var_dump($info->colorSpaceTag, $info->colorSpace === ColorSpace::YCbCr);
var_dump($info->progressive);
var_dump($info->densityUnitTag, $info->densityUnit === DensityUnit::Dpi, $info->xDensity, $info->yDensity);
var_dump($info->metadata->has('exif'), $info->metadata->has('xmp'), $info->metadata->has('icc'), $info->metadata->has('iptc'));

function check_metadata(string $bytes): void {
    $metadata = Reader::fromString($bytes)->info()->metadata;
    var_dump($metadata->has('exif'), $metadata->has('xmp'), !$metadata->has('icc'), $metadata->has('iptc'));
}

$metadataFile = __DIR__ . '/jpeg_codec_metadata_tmp.jpg';
Codec::toFile($im, $metadataFile, new WriteOptions(metadata: $metadata));
check_metadata(file_get_contents($metadataFile));

$metadataStream = fopen('php://temp', 'w+b');
Codec::toStream($im, $metadataStream, new WriteOptions(metadata: $metadata));
rewind($metadataStream);
check_metadata(stream_get_contents($metadataStream));
fclose($metadataStream);
@unlink($metadataFile);

$decoded = $reader->read();
var_dump($decoded instanceof GdImage, imagesx($decoded), imagesy($decoded));
show_exception(fn() => $reader->read());

$tmp = __DIR__ . '/jpeg_codec_basic_tmp.jpg';
Codec::toFile($im, $tmp, new WriteOptions(quality: 80));
var_dump(Codec::fromFile($tmp) instanceof GdImage);

$stream = fopen('php://temp', 'w+b');
Codec::toStream($im, $stream, new WriteOptions(quality: 70));
rewind($stream);
$streamInfo = Reader::fromStream($stream)->info();
var_dump($streamInfo->width, $streamInfo->height, feof($stream));
rewind($stream);
var_dump(Codec::fromStream($stream) instanceof GdImage, feof($stream));
fclose($stream);

show_exception(fn() => Reader::fromString(''));
show_exception(fn() => @Reader::fromString('not jpeg'));
show_exception(fn() => @Codec::fromString('not jpeg'));
show_exception(fn() => new WriteOptions(quality: 101));
$iccOutput = Codec::toString($im, new WriteOptions(
    metadata: Gd\Metadata::create()->with('icc', str_repeat('x', 65519 * 255 + 1)),
));
var_dump(!Reader::fromString($iccOutput)->info()->metadata->has('icc'));

$defaultWrite = new WriteOptions();
$defaultRead = new ReadOptions();
$readerReflection = new ReflectionClass(Reader::class);
var_dump(ColorSpace::YCbCr->value, DensityUnit::Dpi->value);
var_dump($defaultWrite->quality, $defaultWrite->progressive, $defaultWrite->forceNoSubsampling);
var_dump($defaultRead->ignoreWarnings);
var_dump($readerReflection->isFinal(), $readerReflection->isInstantiable());
var_dump(property_exists(WriteOptions::class, 'exif'), property_exists(WriteOptions::class, 'xmp'), property_exists(WriteOptions::class, 'icc'), property_exists(WriteOptions::class, 'iptc'));

@unlink($tmp);
?>
--EXPECTF--
bool(true)
int(4)
int(3)
int(8)
int(3)
int(3)
bool(true)
bool(true)
int(1)
bool(true)
int(123)
int(234)
bool(true)
bool(true)
bool(false)
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
int(4)
int(3)
Gd\Codec\CodecException: JPEG image has already been read
bool(true)
int(4)
int(3)
bool(true)
bool(true)
bool(true)
ValueError: Gd\Jpeg\Reader::fromString(): Argument #1 ($bytes) must not be empty
Gd\Codec\CodecException: Failed to read JPEG info
Gd\Codec\CodecException: Failed to decode JPEG image
ValueError: Gd\Jpeg\WriteOptions::__construct(): Argument #1 ($quality) must be between -1 and 100
bool(true)
int(3)
int(1)
int(-1)
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
