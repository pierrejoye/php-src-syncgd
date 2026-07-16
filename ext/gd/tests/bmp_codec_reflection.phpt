--TEST--
Gd\Bmp\Codec reflection
--EXTENSIONS--
gd
--FILE--
<?php
$write = new Gd\Bmp\WriteOptions();
var_dump(
    $write->bitsPerPixel,
    $write->compression,
    $write->quantize,
    $write->forceV4Header,
    $write->rgb555,
);
var_dump(new Gd\Bmp\ReadOptions() instanceof Gd\Bmp\ReadOptions);

$write = new Gd\Bmp\WriteOptions(
    bitsPerPixel: 8,
    compression: Gd\Bmp\Compression::Rle8,
    quantize: true,
    forceV4Header: true,
    rgb555: true,
);
var_dump(
    $write->bitsPerPixel,
    $write->compression,
    $write->quantize,
    $write->forceV4Header,
    $write->rgb555,
);

foreach ([
    [Gd\Bmp\Codec::class, 'fromFile'],
    [Gd\Bmp\Codec::class, 'fromString'],
    [Gd\Bmp\Codec::class, 'fromStream'],
    [Gd\Bmp\Codec::class, 'toFile'],
    [Gd\Bmp\Codec::class, 'toStream'],
    [Gd\Bmp\Codec::class, 'toString'],
] as $method) {
    $reflection = new ReflectionMethod(...$method);
    echo $reflection->getName(), ':', $reflection->isStatic() ? 'static' : 'instance', "\n";
}

foreach ([-1, 2, 3, 5, 15, 17, 33] as $bitsPerPixel) {
    try {
        new Gd\Bmp\WriteOptions(bitsPerPixel: $bitsPerPixel);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
int(0)
NULL
bool(false)
bool(false)
bool(false)
bool(true)
int(8)
enum(Gd\Bmp\Compression::Rle8)
bool(true)
bool(true)
bool(true)
fromFile:static
fromString:static
fromStream:static
toFile:static
toStream:static
toString:static
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
Gd\Bmp\WriteOptions::__construct(): Argument #1 ($bitsPerPixel) must be one of 0, 1, 4, 8, 16, 24, or 32
