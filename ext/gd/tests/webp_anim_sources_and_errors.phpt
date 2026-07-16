--TEST--
Gd\Webp animation sources and errors
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Webp\AnimReader::class)) die('skip WebP animation API not available');
?>
--FILE--
<?php
function make_truecolor_frame(int $r, int $g, int $b): GdImage {
    $im = imagecreatetruecolor(3, 3);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    $color = imagecolorallocatealpha($im, $r, $g, $b, 0);
    imagefill($im, 0, 0, $color);
    return $im;
}

$im = make_truecolor_frame(1, 2, 3);
$bytes = Gd\Webp\AnimWriter::toMemory(new Gd\Webp\AnimWriteOptions(lossless: true))
    ->addFrame($im, 10)
    ->finish();

$static = null;
ob_start();
imagewebp($im);
$static = ob_get_clean();
var_dump(Gd\Webp\AnimReader::isAnimatedString($static));

$file = tempnam(sys_get_temp_dir(), 'webp-api-');
Gd\Webp\AnimWriter::toFile($file, new Gd\Webp\AnimWriteOptions(lossless: true))->addFrame($im, 10)->finish();
var_dump(Gd\Webp\AnimReader::fromFile($file)->next() instanceof Gd\Webp\Frame);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Webp\AnimWriter::toStream($stream, new Gd\Webp\AnimWriteOptions(lossless: true))->addFrame($im, 10)->finish();
var_dump(is_resource($stream));
rewind($stream);
var_dump(Gd\Webp\AnimReader::isAnimatedStream($stream));
var_dump(feof($stream));
rewind($stream);
var_dump(Gd\Webp\AnimReader::fromStream($stream)->next() instanceof Gd\Webp\Frame);
var_dump(is_resource($stream));

foreach ([
    fn() => Gd\Webp\AnimReader::fromString('not webp'),
    fn() => Gd\Webp\AnimReader::isAnimatedString('not webp'),
    fn() => Gd\Webp\AnimWriter::toMemory()->finish(),
    fn() => Gd\Webp\AnimWriter::toMemory()->addFrame($im, -1),
    fn() => new Gd\Webp\AnimWriteOptions(canvasWidth: -1),
    fn() => new Gd\Webp\AnimWriteOptions(loopCount: 70000),
    fn() => new Gd\Webp\AnimWriteOptions(quality: 101),
    fn() => new Gd\Webp\AnimWriteOptions(method: 7),
    function () {
        $palette = imagecreate(2, 2);
        imagecolorallocate($palette, 0, 0, 0);
        Gd\Webp\AnimWriter::toMemory()->addFrame($palette, 10);
    },
    function () use ($im) {
        $w = Gd\Webp\AnimWriter::toMemory(new Gd\Webp\AnimWriteOptions(canvasWidth: 4, canvasHeight: 4));
        $w->addFrame($im, 10);
    },
] as $fn) {
    try {
        $fn();
        echo "no exception\n";
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

$done = Gd\Webp\AnimWriter::toMemory();
$done->addFrame($im, 10);
$done->finish();
try {
    $done->addFrame($im, 10);
} catch (Throwable $e) {
    echo $e::class, "\n";
}
try {
    $done->finish();
} catch (Throwable $e) {
    echo $e::class, "\n";
}
?>
--EXPECTF--
bool(false)
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
ValueError
ValueError
ValueError
ValueError
ValueError
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
