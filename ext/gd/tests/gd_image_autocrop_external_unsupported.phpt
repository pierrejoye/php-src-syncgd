--TEST--
GdImage::autoCrop() reports unsupported options API with older external libgd
--EXTENSIONS--
gd
--SKIPIF--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->autoCrop();
} catch (Error $e) {
    if (str_contains($e->getMessage(), 'Options-based autocrop is not supported')) {
        exit;
    }
}
die('skip autocrop options API is supported');
?>
--FILE--
<?php
$im = Gd\Image::create(4, 4);
try {
    $im->autoCrop();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
try {
    $im->autoCrop("bad");
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
GdImage::autoCrop(): Options-based autocrop is not supported by this GD library
GdImage::autoCrop(): Options-based autocrop is not supported by this GD library
