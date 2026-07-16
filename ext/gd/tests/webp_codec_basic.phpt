--TEST--
Gd\Webp\Codec single-image operations and errors
--EXTENSIONS--
gd
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Webp\Codec::class)) die('skip WebP codec API not available');
?>
--FILE--
<?php
function make_webp_image(): GdImage {
    $im = imagecreatetruecolor(4, 3);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    imagefill($im, 0, 0, imagecolorallocatealpha($im, 10, 20, 30, 0));
    return $im;
}

$im = make_webp_image();
$bytes = Gd\Webp\Codec::toString($im, new Gd\Webp\WriteOptions(quality: 90));
var_dump(substr($bytes, 0, 4));

$decoded = Gd\Webp\Codec::fromString($bytes);
var_dump(imagesx($decoded), imagesy($decoded));

$file = tempnam(sys_get_temp_dir(), 'webp-codec-');
Gd\Webp\Codec::toFile($im, $file);
var_dump(Gd\Webp\Codec::fromFile($file) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Webp\Codec::toStream($im, $stream);
var_dump(is_resource($stream));
rewind($stream);
var_dump(Gd\Webp\Codec::fromStream($stream) instanceof GdImage);
var_dump(is_resource($stream));

if (class_exists(Gd\Webp\AnimWriter::class)) {
    $second = make_webp_image();
    imagefill($second, 0, 0, imagecolorallocatealpha($second, 200, 10, 20, 0));
    $animated = Gd\Webp\AnimWriter::toMemory(new Gd\Webp\AnimWriteOptions(lossless: true))
        ->addFrame($im, 10)
        ->addFrame($second, 10)
        ->finish();
    try {
        Gd\Webp\Codec::fromString($animated);
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

$options = new Gd\Webp\WriteOptions();
var_dump($options->quality);
$ref = new ReflectionClass(Gd\Webp\Codec::class);
var_dump($ref->isFinal(), $ref->getConstructor()->isPrivate());

foreach ([
    fn() => @Gd\Webp\Codec::fromString('not webp'),
    fn() => @Gd\Webp\Codec::fromFile(__DIR__ . '/missing-webp-codec.webp'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Webp\Codec::fromStream($empty);
    },
    fn() => @Gd\Webp\Codec::toFile(make_webp_image(), __DIR__),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Webp\Codec::toStream(make_webp_image(), $closed);
    },
    fn() => new Gd\Webp\WriteOptions(quality: -2),
] as $fn) {
    try {
        $fn();
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}
?>
--EXPECT--
string(4) "RIFF"
int(4)
int(3)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
int(-1)
bool(true)
bool(true)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
ValueError
