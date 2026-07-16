--TEST--
Gd\Heif\Codec reflection
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Heif\Codec::class)) die('skip HEIF codec API not available');
?>
--FILE--
<?php
$read = new Gd\Heif\ReadOptions();
var_dump($read->ignoreTransformations);

$write = new Gd\Heif\WriteOptions();
var_dump($write->quality, $write->lossless, $write->codec, $write->chromaSubsampling);

$write = new Gd\Heif\WriteOptions(
    quality: 70,
    lossless: true,
    codec: Gd\Heif\CompressionFormat::Av1,
    chromaSubsampling: Gd\Heif\ChromaSubsampling::Yuv420,
);
var_dump($write->quality, $write->lossless, $write->codec, $write->chromaSubsampling);

foreach ([
    [Gd\Heif\Codec::class, 'fromFile'],
    [Gd\Heif\Codec::class, 'fromString'],
    [Gd\Heif\Codec::class, 'fromStream'],
    [Gd\Heif\Codec::class, 'toFile'],
    [Gd\Heif\Codec::class, 'toStream'],
    [Gd\Heif\Codec::class, 'toString'],
] as $method) {
    $reflection = new ReflectionMethod(...$method);
    echo $reflection->getName(), ':', $reflection->isStatic() ? 'static' : 'instance', "\n";
}

foreach ([
    fn() => new Gd\Heif\WriteOptions(quality: -2),
    fn() => new Gd\Heif\WriteOptions(quality: 101),
] as $fn) {
    try {
        $fn();
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
bool(true)
int(-1)
bool(false)
enum(Gd\Heif\CompressionFormat::Hevc)
enum(Gd\Heif\ChromaSubsampling::Yuv444)
int(70)
bool(true)
enum(Gd\Heif\CompressionFormat::Av1)
enum(Gd\Heif\ChromaSubsampling::Yuv420)
fromFile:static
fromString:static
fromStream:static
toFile:static
toStream:static
toString:static
Gd\Heif\WriteOptions::__construct(): Argument #1 ($quality) must be between -1 and 100
Gd\Heif\WriteOptions::__construct(): Argument #1 ($quality) must be between -1 and 100
