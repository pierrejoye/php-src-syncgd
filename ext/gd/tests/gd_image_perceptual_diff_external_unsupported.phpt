--TEST--
GdImage::perceptualDiff() reports unsupported API with older external libgd
--EXTENSIONS--
gd
--SKIPIF--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->perceptualDiff($im, 0.1);
} catch (Error $e) {
    if (str_contains($e->getMessage(), 'Perceptual diff is not supported')) {
        exit;
    }
}
die('skip perceptual diff API is supported');
?>
--FILE--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->perceptualDiff($im, 0.1);
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
GdImage::perceptualDiff(): Perceptual diff is not supported by this GD library
