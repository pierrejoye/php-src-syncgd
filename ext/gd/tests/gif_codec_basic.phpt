--TEST--
Gd\Gif\Codec single-image operations and errors
--EXTENSIONS--
gd
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Gif\Codec::class)) die('skip GIF codec API not available');
?>
--FILE--
<?php
function make_gif_image(): GdImage {
    $im = imagecreatetruecolor(4, 3);
    imagefill($im, 0, 0, imagecolorallocate($im, 255, 0, 0));
    return $im;
}

$im = make_gif_image();
$bytes = Gd\Gif\Codec::toString($im);
var_dump(substr($bytes, 0, 6));

$decoded = Gd\Gif\Codec::fromString($bytes);
var_dump(imagesx($decoded), imagesy($decoded));

$file = tempnam(sys_get_temp_dir(), 'gif-codec-');
Gd\Gif\Codec::toFile($im, $file);
var_dump(Gd\Gif\Codec::fromFile($file) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Gif\Codec::toStream($im, $stream);
var_dump(is_resource($stream));
rewind($stream);
var_dump(Gd\Gif\Codec::fromStream($stream) instanceof GdImage);
var_dump(is_resource($stream));

if (class_exists(Gd\Gif\AnimWriter::class)) {
    $animated = Gd\Gif\AnimWriter::toMemory()
        ->addFrame($im)
        ->addFrame($im)
        ->finish();
    try {
        Gd\Gif\Codec::fromString($animated);
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

$ref = new ReflectionClass(Gd\Gif\Codec::class);
var_dump($ref->isFinal(), $ref->getConstructor()->isPrivate());

foreach ([
    fn() => @Gd\Gif\Codec::fromString('not gif'),
    fn() => @Gd\Gif\Codec::fromFile(__DIR__ . '/missing-gif-codec.gif'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Gif\Codec::fromStream($empty);
    },
    fn() => @Gd\Gif\Codec::toFile(make_gif_image(), __DIR__),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Gif\Codec::toStream(make_gif_image(), $closed);
    },
] as $fn) {
    try {
        $fn();
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}
?>
--EXPECT--
string(6) "GIF87a"
int(4)
int(3)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
bool(true)
bool(true)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
