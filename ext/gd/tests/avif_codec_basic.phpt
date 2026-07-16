--TEST--
Gd\Avif\Codec still image operations
--EXTENSIONS--
gd
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Avif\Codec::class)) die('skip AVIF codec API not available');
?>
--FILE--
<?php
function sample_image(): GdImage {
    $im = imagecreatetruecolor(4, 3);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    for ($y = 0; $y < imagesy($im); $y++) {
        for ($x = 0; $x < imagesx($im); $x++) {
            imagesetpixel($im, $x, $y, imagecolorallocatealpha($im, $x * 40, $y * 50, 30 + $x + $y, 0));
        }
    }
    return $im;
}

function mse(GdImage $a, GdImage $b): float {
    $sum = 0;
    for ($y = 0; $y < imagesy($a); $y++) {
        for ($x = 0; $x < imagesx($a); $x++) {
            $ca = imagecolorsforindex($a, imagecolorat($a, $x, $y));
            $cb = imagecolorsforindex($b, imagecolorat($b, $x, $y));
            foreach (['red', 'green', 'blue', 'alpha'] as $channel) {
                $sum += ($ca[$channel] - $cb[$channel]) ** 2;
            }
        }
    }
    return $sum / (imagesx($a) * imagesy($a) * 4);
}

$im = sample_image();
$lossless = new Gd\Avif\WriteOptions(quality: 1, speed: -1, lossless: true);
$bytes = Gd\Avif\Codec::toString($im, $lossless);
var_dump(strlen($bytes) > 0);

$decoded = Gd\Avif\Codec::fromString($bytes, new Gd\Avif\ReadOptions());
var_dump(imagesx($decoded), imagesy($decoded));
var_dump(mse($im, $decoded));

foreach ([null, Gd\Avif\ChromaSubsampling::Yuv420, Gd\Avif\ChromaSubsampling::Yuv444] as $chroma) {
    $bytes = Gd\Avif\Codec::toString($im, new Gd\Avif\WriteOptions(
        quality: 70,
        chromaSubsampling: $chroma,
    ));
    var_dump(Gd\Avif\Codec::fromString($bytes) instanceof GdImage);
}

$file = tempnam(sys_get_temp_dir(), 'avif-api-');
Gd\Avif\Codec::toFile($im, $file, $lossless);
var_dump(Gd\Avif\Codec::fromFile($file) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Avif\Codec::toStream($im, $stream, $lossless);
rewind($stream);
var_dump(Gd\Avif\Codec::fromStream($stream) instanceof GdImage);

$palette = imagecreate(2, 2);
imagecolorallocate($palette, 1, 2, 3);
try {
    @Gd\Avif\Codec::toString($palette);
} catch (Throwable $e) {
    echo $e::class, "\n";
}

foreach ([
    fn() => @Gd\Avif\Codec::fromString('not avif'),
    fn() => @Gd\Avif\Codec::fromFile(__DIR__ . '/missing-avif-input.avif'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Avif\Codec::fromStream($empty);
    },
    fn() => @Gd\Avif\Codec::toFile(sample_image(), __DIR__),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Avif\Codec::toStream(sample_image(), $closed);
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
bool(true)
int(4)
int(3)
float(0)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
