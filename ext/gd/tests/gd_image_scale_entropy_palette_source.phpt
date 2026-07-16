--TEST--
GdImage::scale() entropy strategy reads palette and truecolor sources equivalently
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
function make_entropy_source(): GdImage {
    $image = imagecreatetruecolor(32, 16);

    $blue = imagecolorallocate($image, 20, 74, 122);
    $red = imagecolorallocate($image, 230, 40, 40);
    $black = imagecolorallocate($image, 0, 0, 0);
    $white = imagecolorallocate($image, 255, 255, 255);

    imagefill($image, 0, 0, $blue);
    imagefilledrectangle($image, 3, 3, 12, 12, $red);

    for ($y = 0; $y < 16; $y++) {
        for ($x = 22; $x < 32; $x++) {
            imagesetpixel($image, $x, $y, (($x + $y) & 1) ? $black : $white);
        }
    }

    return $image;
}

$truecolor = make_entropy_source();
$palette = make_entropy_source();
var_dump(imagetruecolortopalette($palette, false, 4));
var_dump(imageistruecolor($truecolor), imageistruecolor($palette));

$options = new Gd\ScaleOptions(
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
);

$fromTruecolor = $truecolor->scale(8, 8, $options);
$fromPalette = $palette->scale(8, 8, $options);

$diff = $fromTruecolor->perceptualDiff($fromPalette, 0.02);
var_dump(imageistruecolor($fromTruecolor), imageistruecolor($fromPalette));
var_dump($diff->pixelsChanged === 0);
var_dump($diff->maximumDelta < 0.02);
?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
bool(true)
