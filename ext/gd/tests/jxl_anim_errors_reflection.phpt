--TEST--
Gd\Jxl animation errors and reflection
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Jxl\AnimWriter::class)) die('skip JXL animation API not available');
?>
--XLEAK--
libjxl 0.11 emits valgrind uninitialized-value reports while finalizing invalid/error JXL cases; no heap leak remains.
--FILE--
<?php
$write = new ReflectionClass(Gd\Jxl\WriteOptions::class);
$animWrite = new ReflectionClass(Gd\Jxl\AnimWriteOptions::class);
$reader = new ReflectionClass(Gd\Jxl\AnimReader::class);
$writer = new ReflectionClass(Gd\Jxl\AnimWriter::class);
var_dump($write->isReadOnly(), $animWrite->isReadOnly(), $reader->isFinal(), $writer->isFinal());

$wo = new Gd\Jxl\WriteOptions();
$awo = new Gd\Jxl\AnimWriteOptions();
var_dump($wo->lossless, $wo->distance, $wo->effort);
var_dump($awo->canvasWidth, $awo->canvasHeight, $awo->loopCount, $awo->lossless, $awo->distance, $awo->effort);

$im = imagecreatetruecolor(2, 2);
$palette = imagecreate(2, 2);
imagecolorallocate($palette, 0, 0, 0);

foreach ([
    fn() => @Gd\Jxl\AnimReader::fromString('not jxl'),
    fn() => @Gd\Jxl\AnimReader::fromFile(__DIR__ . '/missing-jxl-animation.jxl'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Jxl\AnimReader::fromStream($empty);
    },
    fn() => Gd\Jxl\AnimWriter::toMemory()->finish(),
    fn() => @Gd\Jxl\AnimWriter::toFile(__DIR__)->addFrame($im, 1),
    function () use ($im) {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Jxl\AnimWriter::toStream($closed)->addFrame($im, 1);
    },
    fn() => Gd\Jxl\AnimWriter::toMemory()->addFrame($im, -1),
    fn() => new Gd\Jxl\AnimWriteOptions(canvasWidth: -1),
    fn() => new Gd\Jxl\AnimWriteOptions(loopCount: 70000),
    fn() => new Gd\Jxl\AnimWriteOptions(distance: 26),
    fn() => new Gd\Jxl\AnimWriteOptions(effort: 0),
    fn() => Gd\Jxl\AnimWriter::toMemory()->addFrame($palette, 1),
    function () use ($im) {
        Gd\Jxl\AnimWriter::toMemory(new Gd\Jxl\AnimWriteOptions(canvasWidth: 3, canvasHeight: 3))->addFrame($im, 1);
    },
] as $fn) {
    try {
        $fn();
        echo "no exception\n";
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}

$done = Gd\Jxl\AnimWriter::toMemory();
$done->addFrame($im, 1);
$done->finish();
try {
    $done->addFrame($im, 1);
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
float(1)
int(7)
int(0)
int(0)
int(0)
bool(false)
float(1)
int(7)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
ValueError
ValueError
ValueError
ValueError
ValueError
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
