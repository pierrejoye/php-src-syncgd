--TEST--
Gd\Image::loadFromString() reports unsupported codec read API with older external libgd
--EXTENSIONS--
gd
--SKIPIF--
<?php
try {
    Gd\Image::loadFromString('');
} catch (Gd\Codec\CodecException $e) {
    if (str_contains($e->getMessage(), 'Codec read API is not supported')) {
        exit;
    }
}
die('skip codec read API is supported');
?>
--FILE--
<?php
try {
    Gd\Image::loadFromString('');
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
Gd\Image::loadFrom*(): Codec read API is not supported by this GD library
