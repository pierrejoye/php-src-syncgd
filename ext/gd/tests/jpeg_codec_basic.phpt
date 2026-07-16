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

$xmp = "http://ns.adobe.com/xap/1.0/\0<x:xmpmeta/>";
$iptc = "Photoshop 3.0\08BIM";

$bytes = Codec::toString($im, new WriteOptions(
    quality: 90,
    progressive: true,
    forceNoSubsampling: true,
    exif: "Exif\0\0abc",
    xmp: $xmp,
    icc: "icc-profile",
    iptc: $iptc,
));

var_dump(str_starts_with($bytes, "\xff\xd8"));

$reader = Reader::fromString($bytes, new ReadOptions(ignoreWarnings: true));
$info = $reader->info();
var_dump($info->width, $info->height, $info->bitsPerSample, $info->components);
var_dump($info->colorSpaceTag, $info->colorSpace === ColorSpace::YCbCr);
var_dump($info->progressive);
var_dump($info->densityUnitTag, $info->densityUnit === DensityUnit::Dpi, $info->xDensity, $info->yDensity);
var_dump($info->hasExif, $info->hasXmp, $info->hasIcc, $info->hasIptc);

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
show_exception(fn() => @Codec::toString($im, new WriteOptions(icc: str_repeat('x', 65519 * 255 + 1))));

$defaultWrite = new WriteOptions();
$defaultRead = new ReadOptions();
$readerReflection = new ReflectionClass(Reader::class);
var_dump(ColorSpace::YCbCr->value, DensityUnit::Dpi->value);
var_dump($defaultWrite->quality, $defaultWrite->progressive, $defaultWrite->forceNoSubsampling);
var_dump($defaultRead->ignoreWarnings);
var_dump($readerReflection->isFinal(), $readerReflection->isInstantiable());

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
Gd\Codec\CodecException: Failed to encode JPEG image
int(3)
int(1)
int(-1)
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
