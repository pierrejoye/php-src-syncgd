--TEST--
Gd PNG codec validates write options and output streams
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Codec::class)) {
    die('skip PNG write options API is unavailable');
}
?>
--FILE--
<?php
foreach ([-2, 10] as $level) {
    try {
        new Gd\Png\WriteOptions(compressionLevel: $level);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

try {
    new Gd\Png\WriteOptions(filters: [Gd\Png\Filter::None, 'Sub']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

$stream = fopen('php://temp', 'w+b');
fclose($stream);
try {
    Gd\Png\Codec::toStream(imagecreatetruecolor(1, 1), $stream);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    @Gd\Png\Codec::toFile(imagecreatetruecolor(1, 1), __DIR__ . '/missing/png.png');
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
Gd\Png\WriteOptions::__construct(): Argument #1 ($compressionLevel) must be between -1 and 9
Gd\Png\WriteOptions::__construct(): Argument #1 ($compressionLevel) must be between -1 and 9
Gd\Png\WriteOptions::__construct(): Argument #2 ($filters) must contain only values of type Gd\Png\Filter
Gd\Png\Codec::toStream(): supplied resource is not a valid stream resource
Failed to open PNG output
