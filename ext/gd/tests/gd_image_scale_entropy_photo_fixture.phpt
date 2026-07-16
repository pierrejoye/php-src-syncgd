--TEST--
GdImage::scale() entropy strategy matches a libvips-style photo fixture
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_scale_supported.inc";
if (!method_exists(GdImage::class, "scale")) {
    die("skip GdImage::scale() not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
if (!function_exists("imagecreatefromjpeg")) {
    die("skip jpeg support unavailable");
}
$image = imagecreatetruecolor(1, 1);
try {
    $image->scale(1, 1, new Gd\ScaleOptions(strategy: Gd\ScaleStrategy::Entropy));
} catch (Error $e) {
    if ($e->getMessage() === "GdImage::scale(): Content-aware scale strategies are not supported by this GD library") {
        die("skip content-aware scale strategies are not supported by this GD library");
    }
    throw $e;
}
?>
--FILE--
<?php
$image = imagecreatefromjpeg(__DIR__ . "/gd_image_scale_entropy_portrait_right_side.jpg");
if (!$image) {
    throw new RuntimeException("Failed to load entropy photo fixture");
}

$entropy = $image->scale(120, 180, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Cover,
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\ScaleInterpolation::Linear,
));

$expected = imagecreatefromjpeg(__DIR__ . "/gd_image_scale_entropy_portrait_right_side_expected.jpg");
if (!$expected) {
    throw new RuntimeException("Failed to load expected entropy fixture");
}

$diff = $entropy->perceptualDiff($expected, 0.20);
var_dump(imagesx($image), imagesy($image));
var_dump(imagesx($entropy), imagesy($entropy));
var_dump($diff->pixelsChanged < 2200);
?>
--EXPECT--
int(240)
int(160)
int(120)
int(180)
bool(true)
