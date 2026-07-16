--TEST--
GdImage::scale() entropy strategy falls back to gravity for flat images
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
$image = imagecreatetruecolor(1, 1);
try {
    $image->scale(1, 1, new Gd\ScaleOptions(strategy: Gd\ScaleStrategy::Entropy));
} catch (Error $e) {
    if ($e->getMessage() === "GdImage::scale(): Entropy scale strategy is not supported by this GD library") {
        die("skip entropy scale strategy is not supported by this GD library");
    }
    throw $e;
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(12, 8);
$color = imagecolorallocatealpha($image, 10, 80, 140, 0);
imagefill($image, 0, 0, $color);

$entropy = $image->scale(4, 4, new Gd\ScaleOptions(
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
$gravity = $image->scale(4, 4, new Gd\ScaleOptions(
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));

$diff = $entropy->perceptualDiff($gravity, 0.0);
printf("fallback pixels=%d max=%.6f\n", $diff->pixelsChanged, $diff->maximumDelta);
?>
--EXPECT--
fallback pixels=0 max=0.000000
