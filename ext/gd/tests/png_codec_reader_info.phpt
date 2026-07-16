--TEST--
Gd\Png\Reader and Codec read APIs with info and comments
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Reader::class)) die('skip Gd\Png\Reader not available');
?>
--FILE--
<?php
use Gd\Codec\CodecException;
use Gd\Png\Codec;
use Gd\Png\ColorType;
use Gd\Png\CompressionStrategy;
use Gd\Png\InterlaceMethod;
use Gd\Png\PhysicalUnit;
use Gd\Png\Reader;
use Gd\Png\WriteOptions;

$im = imagecreatetruecolor(3, 2);
imageresolution($im, 96, 96);
imagesavealpha($im, true);
$transparent = imagecolorallocatealpha($im, 0, 0, 0, 127);
imagefill($im, 0, 0, $transparent);
$red = imagecolorallocatealpha($im, 255, 0, 0, 0);
imagesetpixel($im, 1, 1, $red);

$options = new WriteOptions(
    compressionLevel: 0,
    filters: [Gd\Png\Filter::None],
    compressionStrategy: CompressionStrategy::Default,
    comments: [
        'Title' => 'PNG Reader Test',
        'Comment' => 'hello gd',
    ],
);
$bytes = Codec::toString($im, $options);

$reader = Reader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->bitDepth);
var_dump($info->colorTypeTag, $info->colorType === ColorType::Rgba);
var_dump($info->hasAlpha, $info->hasTransparency);
var_dump($info->paletteEntries);
var_dump($info->interlaceMethodTag, $info->interlaceMethod === InterlaceMethod::None);
var_dump($info->resolutionX, $info->resolutionY);
var_dump($info->physicalUnit === PhysicalUnit::Meter);
var_dump($info->comments);
var_dump($info->decodedTrueColor);

$decoded = $reader->read();
var_dump(imagesx($decoded), imagesy($decoded));
var_dump(imagecolorat($decoded, 1, 1) & 0xffffff);
var_dump($reader->info()->comments['Title']);
try {
    $reader->read();
} catch (CodecException $e) {
    echo $e->getMessage(), "\n";
}
unset($reader);
var_dump(imagesx($decoded));

$file = __DIR__ . '/png_codec_reader_info_tmp.png';
Codec::toFile($im, $file, $options);
var_dump(imagesx(Reader::fromFile($file)->read()));
var_dump(imagesx(Codec::fromFile($file)));
@unlink($file);

$stream = fopen('php://temp', 'w+b');
Codec::toStream($im, $stream, $options);
rewind($stream);
$streamReader = Reader::fromStream($stream);
var_dump(ftell($stream) === strlen($bytes));
var_dump($streamReader->info()->comments['Comment']);
var_dump(imagesy($streamReader->read()));
var_dump(is_resource($stream));
fclose($stream);

var_dump(imagesx(Codec::fromString($bytes)));
$stream = fopen('php://temp', 'w+b');
fwrite($stream, $bytes);
rewind($stream);
var_dump(imagesy(Codec::fromStream($stream)));
var_dump(is_resource($stream));
fclose($stream);

foreach ([
    fn() => Reader::fromString(''),
    fn() => Codec::fromString(''),
    fn() => Reader::fromString('not png'),
] as $callable) {
    try {
        $callable();
    } catch (Throwable $e) {
        echo get_class($e), ': ', $e->getMessage(), "\n";
    }
}

foreach ([
    fn() => new WriteOptions(comments: ['' => 'x']),
    fn() => new WriteOptions(comments: ["bad\0key" => 'x']),
    fn() => new WriteOptions(comments: ['Title' => "bad\0value"]),
] as $callable) {
    try {
        $callable();
    } catch (Throwable $e) {
        echo get_class($e), ': ', $e->getMessage(), "\n";
    }
}

$readerReflection = new ReflectionClass(Reader::class);
$codecReflection = new ReflectionClass(Codec::class);
var_dump($readerReflection->isFinal(), $readerReflection->getConstructor()->isPrivate());
var_dump($codecReflection->getMethod('fromFile')->getReturnType()->getName());
var_dump($codecReflection->getMethod('toString')->getNumberOfParameters());
?>
--EXPECTF--
int(3)
int(2)
int(8)
int(6)
bool(true)
bool(true)
bool(false)
NULL
int(0)
bool(true)
int(96)
int(96)
bool(true)
array(2) {
  ["Title"]=>
  string(15) "PNG Reader Test"
  ["Comment"]=>
  string(8) "hello gd"
}
bool(true)
int(3)
int(2)
int(16711680)
string(15) "PNG Reader Test"
PNG image has already been read
int(3)
int(3)
int(3)
bool(true)
string(8) "hello gd"
int(2)
bool(true)
int(3)
int(2)
bool(true)
ValueError: Gd\Png\Reader::fromString(): Argument #1 ($bytes) must not be empty
ValueError: Gd\Png\Codec::fromString(): Argument #1 ($bytes) must not be empty
Gd\Codec\CodecException: Failed to read PNG info
ValueError: Gd\Png\WriteOptions::__construct(): Argument #4 ($comments) keys must be between 1 and 79 bytes
ValueError: Gd\Png\WriteOptions::__construct(): Argument #4 ($comments) keys must not contain null bytes
ValueError: Gd\Png\WriteOptions::__construct(): Argument #4 ($comments) values must not contain null bytes
bool(true)
bool(true)
string(7) "GdImage"
int(2)
