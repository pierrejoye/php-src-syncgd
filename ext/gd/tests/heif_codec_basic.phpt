--TEST--
Gd\Heif\Codec still image operations
--EXTENSIONS--
gd
--XLEAK--
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Heif\Codec::class)) die('skip HEIF codec API not available');
$im = imagecreatetruecolor(2, 2);
imagealphablending($im, false);
imagesavealpha($im, true);
imagefill($im, 0, 0, imagecolorallocatealpha($im, 10, 20, 30, 0));
try {
    Gd\Heif\Codec::toString($im, new Gd\Heif\WriteOptions(lossless: true));
} catch (Throwable $e) {
    die('skip HEIF encoder unavailable: ' . $e->getMessage());
}
?>
--FILE--
<?php
function sample_image(): GdImage {
    $im = imagecreatetruecolor(64, 64);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    for ($y = 0; $y < imagesy($im); $y++) {
        for ($x = 0; $x < imagesx($im); $x++) {
            imagesetpixel($im, $x, $y, imagecolorallocatealpha($im, ($x * 7) % 256, ($y * 5) % 256, (30 + $x + $y) % 256, 0));
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
$lossless = new Gd\Heif\WriteOptions(quality: 1, lossless: true);
$bytes = Gd\Heif\Codec::toString($im, $lossless);
var_dump(strlen($bytes) > 0);

$decoded = Gd\Heif\Codec::fromString($bytes, new Gd\Heif\ReadOptions());
var_dump(imagesx($decoded), imagesy($decoded));
var_dump(mse($im, $decoded) < 1.0);

foreach ([Gd\Heif\ChromaSubsampling::Yuv420, Gd\Heif\ChromaSubsampling::Yuv422, Gd\Heif\ChromaSubsampling::Yuv444] as $chroma) {
    $bytes = Gd\Heif\Codec::toString($im, new Gd\Heif\WriteOptions(
        quality: 70,
        chromaSubsampling: $chroma,
    ));
    var_dump(Gd\Heif\Codec::fromString($bytes) instanceof GdImage);
}

$file = tempnam(sys_get_temp_dir(), 'heif-api-');
Gd\Heif\Codec::toFile($im, $file, $lossless);
var_dump(Gd\Heif\Codec::fromFile($file, new Gd\Heif\ReadOptions(ignoreTransformations: false)) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Heif\Codec::toStream($im, $stream, $lossless);
rewind($stream);
var_dump(Gd\Heif\Codec::fromStream($stream) instanceof GdImage);

$palette = imagecreate(2, 2);
imagecolorallocate($palette, 1, 2, 3);
try {
    @Gd\Heif\Codec::toString($palette);
} catch (Throwable $e) {
    echo $e::class, "\n";
}

foreach ([
    fn() => @Gd\Heif\Codec::fromString('not heif'),
    fn() => @Gd\Heif\Codec::fromFile(__DIR__ . '/missing-heif-input.heic'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Heif\Codec::fromStream($empty);
    },
    fn() => @Gd\Heif\Codec::toFile(sample_image(), __DIR__),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Heif\Codec::toStream(sample_image(), $closed);
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
int(64)
int(64)
bool(true)
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
