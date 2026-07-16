--TEST--
Gd\Bmp\Codec still image operations
--EXTENSIONS--
gd
--SKIPIF--
<?php
require __DIR__ . '/skipif_gd_codec_api_supported.inc';
if (!class_exists(Gd\Bmp\Codec::class)) die('skip BMP codec API not available');
?>
--FILE--
<?php
function truecolor_image(): GdImage {
    $im = imagecreatetruecolor(5, 4);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    for ($y = 0; $y < imagesy($im); $y++) {
        for ($x = 0; $x < imagesx($im); $x++) {
            imagesetpixel($im, $x, $y, imagecolorallocatealpha($im, $x * 40, $y * 50, 30 + $x + $y, $x === $y ? 30 : 0));
        }
    }
    return $im;
}

function palette_image(int $colors): GdImage {
    $im = imagecreate(8, 8);
    for ($i = 0; $i < $colors; $i++) {
        imagecolorallocate($im, ($i * 17) % 256, ($i * 31) % 256, ($i * 47) % 256);
    }
    for ($y = 0; $y < imagesy($im); $y++) {
        for ($x = 0; $x < imagesx($im); $x++) {
            imagesetpixel($im, $x, $y, ($x + $y) % $colors);
        }
    }
    return $im;
}

function u16le(string $bytes, int $offset): int {
    return unpack('v', substr($bytes, $offset, 2))[1];
}

function u32le(string $bytes, int $offset): int {
    return unpack('V', substr($bytes, $offset, 4))[1];
}

function header_summary(string $bytes): array {
    return [
        substr($bytes, 0, 2),
        u32le($bytes, 14),
        u16le($bytes, 28),
        u32le($bytes, 30),
    ];
}

$im = truecolor_image();
$bytes = Gd\Bmp\Codec::toString($im);
var_dump(strlen($bytes) > 0);
var_dump(header_summary($bytes));

$decoded = Gd\Bmp\Codec::fromString($bytes, new Gd\Bmp\ReadOptions());
var_dump(imagesx($decoded), imagesy($decoded), imageistruecolor($decoded));

$file = tempnam(sys_get_temp_dir(), 'bmp-api-');
Gd\Bmp\Codec::toFile($im, $file, new Gd\Bmp\WriteOptions(bitsPerPixel: 24, compression: Gd\Bmp\Compression::None));
var_dump(Gd\Bmp\Codec::fromFile($file) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Bmp\Codec::toStream($im, $stream, new Gd\Bmp\WriteOptions(bitsPerPixel: 32));
rewind($stream);
var_dump(Gd\Bmp\Codec::fromStream($stream) instanceof GdImage);

foreach ([1, 4, 8] as $bitsPerPixel) {
    $bytes = Gd\Bmp\Codec::toString(palette_image(1 << $bitsPerPixel), new Gd\Bmp\WriteOptions(bitsPerPixel: $bitsPerPixel));
    var_dump(header_summary($bytes));
}

foreach ([16, 24, 32] as $bitsPerPixel) {
    $bytes = Gd\Bmp\Codec::toString($im, new Gd\Bmp\WriteOptions(bitsPerPixel: $bitsPerPixel));
    var_dump(header_summary($bytes));
}

$bytes = Gd\Bmp\Codec::toString(palette_image(16), new Gd\Bmp\WriteOptions(bitsPerPixel: 4, compression: Gd\Bmp\Compression::Rle4));
var_dump(header_summary($bytes));
$bytes = Gd\Bmp\Codec::toString(palette_image(32), new Gd\Bmp\WriteOptions(bitsPerPixel: 8, compression: Gd\Bmp\Compression::Rle8));
var_dump(header_summary($bytes));
$bytes = Gd\Bmp\Codec::toString(palette_image(16), new Gd\Bmp\WriteOptions(bitsPerPixel: 4, compression: Gd\Bmp\Compression::None));
var_dump(header_summary($bytes));

$bytes = Gd\Bmp\Codec::toString($im, new Gd\Bmp\WriteOptions(bitsPerPixel: 8, quantize: true));
var_dump(header_summary($bytes));

$bytes = Gd\Bmp\Codec::toString($im, new Gd\Bmp\WriteOptions(bitsPerPixel: 24, forceV4Header: true));
var_dump(header_summary($bytes));

$bytes = Gd\Bmp\Codec::toString($im, new Gd\Bmp\WriteOptions(bitsPerPixel: 16, rgb555: true));
var_dump(u32le($bytes, 54), u32le($bytes, 58), u32le($bytes, 62));

foreach ([
    fn() => @Gd\Bmp\Codec::fromString('not bmp'),
    fn() => @Gd\Bmp\Codec::fromFile(__DIR__ . '/missing-bmp-input.bmp'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Bmp\Codec::fromStream($empty);
    },
    fn() => Gd\Bmp\Codec::toString(truecolor_image(), new Gd\Bmp\WriteOptions(bitsPerPixel: 8)),
    fn() => Gd\Bmp\Codec::toString(palette_image(16), new Gd\Bmp\WriteOptions(bitsPerPixel: 4, compression: Gd\Bmp\Compression::Rle8)),
    fn() => @Gd\Bmp\Codec::toFile(truecolor_image(), __DIR__),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Bmp\Codec::toStream(truecolor_image(), $closed);
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
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(108)
  [2]=>
  int(32)
  [3]=>
  int(3)
}
int(5)
int(4)
bool(true)
bool(true)
bool(true)
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(1)
  [3]=>
  int(0)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(4)
  [3]=>
  int(2)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(8)
  [3]=>
  int(1)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(16)
  [3]=>
  int(3)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(24)
  [3]=>
  int(0)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(108)
  [2]=>
  int(32)
  [3]=>
  int(3)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(4)
  [3]=>
  int(2)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(8)
  [3]=>
  int(1)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(4)
  [3]=>
  int(0)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(40)
  [2]=>
  int(8)
  [3]=>
  int(1)
}
array(4) {
  [0]=>
  string(2) "BM"
  [1]=>
  int(108)
  [2]=>
  int(24)
  [3]=>
  int(0)
}
int(31744)
int(992)
int(31)
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
