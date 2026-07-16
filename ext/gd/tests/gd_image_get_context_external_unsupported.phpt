--TEST--
GdImage::getContext() reports unsupported 2D API with older external libgd
--EXTENSIONS--
gd
--SKIPIF--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->getContext();
} catch (Error $e) {
    if (str_contains($e->getMessage(), '2D context is not supported')) {
        exit;
    }
}
die('skip 2D context API is supported');
?>
--FILE--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->getContext();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
GdImage::getContext(): 2D context is not supported by this GD library
