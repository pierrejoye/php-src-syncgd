--TEST--
Gd\Jpeg\ReadOptions scaling and DCT behavior
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
use Gd\Jpeg\DctMethod;
use Gd\Jpeg\Reader;
use Gd\Jpeg\ReadOptions;
use Gd\Jpeg\WriteOptions;

function dims(GdImage $image): array {
    return [imagesx($image), imagesy($image)];
}

function show_exception(callable $callback): void {
    try {
        $callback();
    } catch (Throwable $e) {
        echo $e::class, ': ', $e->getMessage(), "\n";
    }
}

$im = imagecreatetruecolor(16, 8);
for ($y = 0; $y < 8; $y++) {
    for ($x = 0; $x < 16; $x++) {
        imagesetpixel($im, $x, $y, (($x * 16) << 16) | (($y * 32) << 8) | 0x55);
    }
}

$bytes = Codec::toString($im, new WriteOptions(quality: 90));

var_dump(dims(Codec::fromString($bytes)));
var_dump(dims(Codec::fromString($bytes, new ReadOptions(scaleNumerator: 1, scaleDenominator: 2))));
var_dump(dims(Codec::fromString($bytes, new ReadOptions(scaleNumerator: 1, scaleDenominator: 4))));
var_dump(dims(Codec::fromString($bytes, new ReadOptions(scaleNumerator: 1, scaleDenominator: 8))));
var_dump(dims(Codec::fromString($bytes, new ReadOptions(scaleNumerator: 4, scaleDenominator: 8))));
var_dump(dims(Codec::fromString($bytes, new ReadOptions(scaleNumerator: 3, scaleDenominator: 8))));
var_dump(dims(Codec::fromString($bytes, new ReadOptions(scaleNumerator: 2, scaleDenominator: 1))));

$reader = Reader::fromString($bytes, new ReadOptions(scaleNumerator: 1, scaleDenominator: 2));
$info = $reader->info();
var_dump($info->width, $info->height);
var_dump(dims($reader->read()));

$tmp = __DIR__ . '/jpeg_codec_read_options_tmp.jpg';
file_put_contents($tmp, $bytes);
var_dump(dims(Codec::fromFile($tmp, new ReadOptions(scaleNumerator: 3, scaleDenominator: 8))));

$stream = fopen('php://temp', 'w+b');
fwrite($stream, $bytes);
rewind($stream);
var_dump(dims(Codec::fromStream($stream, new ReadOptions(scaleNumerator: 3, scaleDenominator: 8))), feof($stream));
fclose($stream);

foreach ([DctMethod::Default, DctMethod::Slow, DctMethod::Fast, DctMethod::Float] as $method) {
    $decoded = Codec::fromString($bytes, new ReadOptions(dctMethod: $method));
    var_dump($method->value, dims($decoded));
}

$defaultRead = new ReadOptions();
var_dump($defaultRead->ignoreWarnings, $defaultRead->scaleNumerator, $defaultRead->scaleDenominator, $defaultRead->dctMethod === DctMethod::Default);
var_dump(DctMethod::Default->value, DctMethod::Slow->value, DctMethod::Fast->value, DctMethod::Float->value);

show_exception(fn() => new ReadOptions(scaleNumerator: 0));
show_exception(fn() => new ReadOptions(scaleDenominator: 0));
show_exception(fn() => @Codec::fromString($bytes, new ReadOptions(scaleNumerator: 1, scaleDenominator: 99)));
if (PHP_INT_SIZE >= 8) {
    show_exception(fn() => @Codec::fromString($bytes, new ReadOptions(scaleNumerator: 4294967295, scaleDenominator: 1)));
}
show_exception(fn() => @Codec::fromString('not jpeg', new ReadOptions(scaleNumerator: 1, scaleDenominator: 2, dctMethod: DctMethod::Fast)));

@unlink($tmp);
?>
--EXPECTF--
array(2) {
  [0]=>
  int(16)
  [1]=>
  int(8)
}
array(2) {
  [0]=>
  int(8)
  [1]=>
  int(4)
}
array(2) {
  [0]=>
  int(4)
  [1]=>
  int(2)
}
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(1)
}
array(2) {
  [0]=>
  int(8)
  [1]=>
  int(4)
}
array(2) {
  [0]=>
  int(6)
  [1]=>
  int(3)
}
array(2) {
  [0]=>
  int(32)
  [1]=>
  int(16)
}
int(16)
int(8)
array(2) {
  [0]=>
  int(8)
  [1]=>
  int(4)
}
array(2) {
  [0]=>
  int(6)
  [1]=>
  int(3)
}
array(2) {
  [0]=>
  int(6)
  [1]=>
  int(3)
}
bool(true)
int(0)
array(2) {
  [0]=>
  int(16)
  [1]=>
  int(8)
}
int(1)
array(2) {
  [0]=>
  int(16)
  [1]=>
  int(8)
}
int(2)
array(2) {
  [0]=>
  int(16)
  [1]=>
  int(8)
}
int(3)
array(2) {
  [0]=>
  int(16)
  [1]=>
  int(8)
}
bool(true)
int(1)
int(1)
bool(true)
int(0)
int(1)
int(2)
int(3)
ValueError: Gd\Jpeg\ReadOptions::__construct(): Argument #2 ($scaleNumerator) must be between 1 and %d
ValueError: Gd\Jpeg\ReadOptions::__construct(): Argument #3 ($scaleDenominator) must be between 1 and %d
Gd\Codec\CodecException: Failed to decode JPEG image
Gd\Codec\CodecException: Failed to decode JPEG image
Gd\Codec\CodecException: Failed to decode JPEG image
