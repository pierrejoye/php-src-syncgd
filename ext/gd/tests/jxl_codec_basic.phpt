--TEST--
Gd\Jxl\Codec still image operations
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Jxl\Codec::class)) die('skip JXL codec API not available');
?>
--XLEAK--
libjxl 0.11 emits valgrind uninitialized-value reports after decoding valid input before invalid input; no heap leak remains.
--FILE--
<?php
function frame(int $r, int $g, int $b): GdImage {
    $im = imagecreatetruecolor(4, 3);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    imagefill($im, 0, 0, imagecolorallocatealpha($im, $r, $g, $b, 0));
    return $im;
}

$im = frame(11, 22, 33);
$bytes = Gd\Jxl\Codec::toString($im, new Gd\Jxl\WriteOptions(lossless: true));
var_dump(strlen(substr($bytes, 0, 2)) === 2);

$decoded = Gd\Jxl\Codec::fromString($bytes);
var_dump(imagesx($decoded), imagesy($decoded));

$file = tempnam(sys_get_temp_dir(), 'jxl-api-');
Gd\Jxl\Codec::toFile($im, $file, new Gd\Jxl\WriteOptions(lossless: true));
var_dump(Gd\Jxl\Codec::fromFile($file) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Jxl\Codec::toStream($im, $stream, new Gd\Jxl\WriteOptions(lossless: true));
var_dump(is_resource($stream));
rewind($stream);
var_dump(Gd\Jxl\Codec::fromStream($stream) instanceof GdImage);
var_dump(is_resource($stream));

foreach ([
    fn() => @Gd\Jxl\Codec::fromString('not jxl'),
    fn() => @Gd\Jxl\Codec::fromFile(__DIR__ . '/missing-jxl-input.jxl'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Jxl\Codec::fromStream($empty);
    },
    fn() => @Gd\Jxl\Codec::toFile(frame(1, 2, 3), __DIR__, new Gd\Jxl\WriteOptions()),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Jxl\Codec::toStream(frame(1, 2, 3), $closed);
    },
    fn() => new Gd\Jxl\WriteOptions(distance: -0.1),
    fn() => new Gd\Jxl\WriteOptions(effort: 10),
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
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
ValueError
ValueError
