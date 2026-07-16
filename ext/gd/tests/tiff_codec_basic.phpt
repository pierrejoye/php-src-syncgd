--TEST--
Gd\Tiff\Codec single-page operations and errors
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Codec::class)) die('skip TIFF codec API not available');
?>
--FILE--
<?php
function make_tiff_image(int $r = 20, int $g = 40, int $b = 60): GdImage {
    $im = imagecreatetruecolor(4, 3);
    imagealphablending($im, false);
    imagesavealpha($im, true);
    imagefill($im, 0, 0, imagecolorallocatealpha($im, $r, $g, $b, 0));
    return $im;
}

$im = make_tiff_image();
$options = new Gd\Tiff\WriteOptions(compression: Gd\Tiff\Compression::None);
$bytes = Gd\Tiff\Codec::toString($im, $options);
var_dump(strlen($bytes) > 8);

$decoded = Gd\Tiff\Codec::fromString($bytes);
var_dump(imagesx($decoded), imagesy($decoded));

$file = tempnam(sys_get_temp_dir(), 'tiff-codec-');
Gd\Tiff\Codec::toFile($im, $file, $options);
var_dump(Gd\Tiff\Codec::fromFile($file) instanceof GdImage);
unlink($file);

$stream = fopen('php://temp', 'w+b');
Gd\Tiff\Codec::toStream($im, $stream, $options);
var_dump(is_resource($stream));
rewind($stream);
var_dump(Gd\Tiff\Codec::fromStream($stream) instanceof GdImage);
var_dump(is_resource($stream));

$multi = Gd\Tiff\Writer::toMemory($options)
    ->addPage(make_tiff_image(1, 2, 3))
    ->addPage(make_tiff_image(4, 5, 6))
    ->finish();
try {
    Gd\Tiff\Codec::fromString($multi);
} catch (Throwable $e) {
    echo $e::class, "\n";
}

$ref = new ReflectionClass(Gd\Tiff\Codec::class);
var_dump($ref->isFinal(), $ref->getConstructor()->isPrivate());

foreach ([
    fn() => @Gd\Tiff\Codec::fromString('not tiff'),
    fn() => @Gd\Tiff\Codec::fromFile(__DIR__ . '/missing-tiff-codec.tiff'),
    function () {
        $empty = fopen('php://temp', 'w+b');
        @Gd\Tiff\Codec::fromStream($empty);
    },
    fn() => @Gd\Tiff\Codec::toFile(make_tiff_image(), __DIR__),
    function () {
        $closed = fopen('php://temp', 'w+b');
        fclose($closed);
        Gd\Tiff\Codec::toStream(make_tiff_image(), $closed);
    },
    function () {
        $palette = imagecreate(2, 2);
        imagecolorallocate($palette, 0, 0, 0);
        Gd\Tiff\Codec::toString($palette);
    },
] as $fn) {
    try {
        $fn();
    } catch (Throwable $e) {
        echo $e::class, "\n";
    }
}
?>
--EXPECTF--
bool(true)
int(4)
int(3)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Codec\CodecException
bool(true)
bool(true)
%sNot a TIFF%s
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
Gd\Codec\CodecException
TypeError
Gd\Codec\CodecException
