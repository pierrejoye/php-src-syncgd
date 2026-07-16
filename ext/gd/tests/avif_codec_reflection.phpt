--TEST--
Gd\Avif\Codec reflection
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Avif\Codec::class)) die('skip AVIF codec API not available');
?>
--FILE--
<?php
$write = new Gd\Avif\WriteOptions();
var_dump($write->quality, $write->speed, $write->lossless, $write->chromaSubsampling);
var_dump(new Gd\Avif\ReadOptions() instanceof Gd\Avif\ReadOptions);

$write = new Gd\Avif\WriteOptions(
    quality: 70,
    speed: 5,
    lossless: true,
    chromaSubsampling: Gd\Avif\ChromaSubsampling::Yuv444,
);
var_dump($write->quality, $write->speed, $write->lossless, $write->chromaSubsampling);

foreach ([
    [Gd\Avif\Codec::class, 'fromFile'],
    [Gd\Avif\Codec::class, 'fromString'],
    [Gd\Avif\Codec::class, 'fromStream'],
    [Gd\Avif\Codec::class, 'toFile'],
    [Gd\Avif\Codec::class, 'toStream'],
    [Gd\Avif\Codec::class, 'toString'],
] as $method) {
    $reflection = new ReflectionMethod(...$method);
    echo $reflection->getName(), ':', $reflection->isStatic() ? 'static' : 'instance', "\n";
}

foreach ([
    fn() => new Gd\Avif\WriteOptions(quality: -2),
    fn() => new Gd\Avif\WriteOptions(quality: 101),
    fn() => new Gd\Avif\WriteOptions(speed: -2),
    fn() => new Gd\Avif\WriteOptions(speed: 11),
] as $fn) {
    try {
        $fn();
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
int(-1)
int(-1)
bool(false)
NULL
bool(true)
int(70)
int(5)
bool(true)
enum(Gd\Avif\ChromaSubsampling::Yuv444)
fromFile:static
fromString:static
fromStream:static
toFile:static
toStream:static
toString:static
Gd\Avif\WriteOptions::__construct(): Argument #1 ($quality) must be between -1 and 100
Gd\Avif\WriteOptions::__construct(): Argument #1 ($quality) must be between -1 and 100
Gd\Avif\WriteOptions::__construct(): Argument #2 ($speed) must be between -1 and 10
Gd\Avif\WriteOptions::__construct(): Argument #2 ($speed) must be between -1 and 10
