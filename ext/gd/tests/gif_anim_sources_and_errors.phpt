--TEST--
Gd\Gif animation sources and errors
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Gif\AnimReader::class)) die('skip GIF animation API not available');
?>
--FILE--
<?php
function make_frame(int $r, int $g, int $b): GdImage {
    $im = imagecreate(3, 3);
    $color = imagecolorallocate($im, $r, $g, $b);
    imagefill($im, 0, 0, $color);
    return $im;
}

$im = make_frame(1, 2, 3);
$writer = Gd\Gif\AnimWriter::toMemory();
$writer->addFrame($im);
$bytes = $writer->finish();

$file = tempnam(sys_get_temp_dir(), 'gif-api-');
Gd\Gif\AnimWriter::toFile($file)->addFrame($im)->finish();
var_dump(Gd\Gif\AnimReader::fromFile($file)->next() instanceof Gd\Gif\Frame);
unlink($file);

$stream = fopen('php://temp', 'w+b');
$streamWriter = Gd\Gif\AnimWriter::toStream($stream);
$streamWriter->addFrame($im)->finish();
var_dump(is_resource($stream));
rewind($stream);
var_dump(Gd\Gif\AnimReader::isAnimatedStream($stream));
var_dump(feof($stream));
rewind($stream);
var_dump(Gd\Gif\AnimReader::fromStream($stream)->next() instanceof Gd\Gif\Frame);
var_dump(is_resource($stream));

foreach ([
    fn() => Gd\Gif\AnimReader::fromString('not gif'),
    fn() => Gd\Gif\AnimReader::isAnimatedString('not gif'),
    fn() => Gd\Gif\AnimWriter::toMemory(loops: 70000),
    fn() => new Gd\Gif\FrameOptions(delayMs: 25),
    fn() => Gd\Gif\AnimWriter::toMemory()->finish(),
    function () use ($im) {
        $w = Gd\Gif\AnimWriter::toMemory();
        $w->addFrame($im, new Gd\Gif\FrameOptions(x: 1));
    },
] as $fn) {
    try {
        $fn();
        echo "no exception\n";
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

$done = Gd\Gif\AnimWriter::toMemory();
$done->addFrame($im);
$done->finish();
try {
    $done->addFrame($im);
} catch (Throwable $e) {
    echo $e::class, "\n";
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
Gd\Codec\CodecException
ValueError
ValueError
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
