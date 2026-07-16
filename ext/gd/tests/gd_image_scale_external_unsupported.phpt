--TEST--
GdImage::scale() reports unsupported options API with older external libgd
--EXTENSIONS--
gd
--SKIPIF--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->scale(2, 2);
} catch (Error $e) {
    if (str_contains($e->getMessage(), 'Options-based scale is not supported')) {
        exit;
    }
}
die('skip scale options API is supported');
?>
--FILE--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->scale(2, 2);
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
try {
    $im->scale("bad");
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
GdImage::scale(): Options-based scale is not supported by this GD library
GdImage::scale(): Options-based scale is not supported by this GD library
