--TEST--
GdImage::autoCrop() validation errors and failure behavior
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_autocrop_supported.inc";
if (!method_exists(GdImage::class, "autoCrop")) {
    die("skip GdImage::autoCrop() not available.");
}
?>
--FILE--
<?php
$truecolor = imagecreatetruecolor(4, 4);
$palette = imagecreate(4, 4);

foreach ([
    fn() => $truecolor->autoCrop(new Gd\AutoCropOptions(mode: Gd\AutoCropMode::Threshold)),
    fn() => $palette->autoCrop(new Gd\AutoCropOptions(mode: Gd\AutoCropMode::Threshold, color: 100)),
] as $callback) {
    try {
        $callback();
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

try {
    $truecolor->autoCrop(new Gd\AutoCropOptions(mode: Gd\AutoCropMode::Black));
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}

var_dump(imagecropauto($truecolor, IMG_CROP_BLACK));
?>
--EXPECT--
GdImage::autoCrop(): Gd\AutoCropOptions::$color must be greater than or equal to 0 when using threshold mode
GdImage::autoCrop(): Gd\AutoCropOptions::$color must be greater than or equal to 0 when using threshold mode
Failed to auto-crop GdImage
bool(false)
