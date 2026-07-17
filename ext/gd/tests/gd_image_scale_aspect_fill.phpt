--TEST--
GdImage::scale() aspect-ratio dimensions and fill mode
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
?>
--FILE--
<?php
function make_source(): GdImage {
    $image = imagecreatetruecolor(8, 4);
    imagealphablending($image, false);
    imagesavealpha($image, true);

    for ($y = 0; $y < imagesy($image); $y++) {
        for ($x = 0; $x < imagesx($image); $x++) {
            imagesetpixel($image, $x, $y, imagecolorallocatealpha($image, $x * 25, $y * 45, 80 + $x + $y, 0));
        }
    }

    return $image;
}

function assert_similar(string $label, GdImage $actual, GdImage $expected): void {
    if (imagesx($actual) !== imagesx($expected) || imagesy($actual) !== imagesy($expected)) {
        echo "$label dimensions differ\n";
        return;
    }

    $diff = $actual->perceptualDiff($expected, 0.03);
    printf("%s pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
}

$source = make_source();

$widthOnly = $source->scale(4, null, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill, interpolation: Gd\InterpolationMethod::NearestNeighbour));
printf("width-only %dx%d\n", imagesx($widthOnly), imagesy($widthOnly));
assert_similar('width-only', $widthOnly, imagescale($source, 4, 2, IMG_NEAREST_NEIGHBOUR));

$heightOnly = $source->scale(null, 2, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill, interpolation: Gd\InterpolationMethod::NearestNeighbour));
printf("height-only %dx%d\n", imagesx($heightOnly), imagesy($heightOnly));
assert_similar('height-only', $heightOnly, imagescale($source, 4, 2, IMG_NEAREST_NEIGHBOUR));

$fill = $source->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill, interpolation: Gd\InterpolationMethod::Linear));
printf("fill %dx%d\n", imagesx($fill), imagesy($fill));
assert_similar('fill', $fill, imagescale($source, 4, 4, IMG_BILINEAR_FIXED));

$override = $source->scale(99, 99, new Gd\ScaleOptions(width: 4, height: 2, fit: Gd\ScaleFit::Fill));
printf("override %dx%d\n", imagesx($override), imagesy($override));
?>
--EXPECT--
width-only 4x2
width-only pixels=0 max=0.000000
height-only 4x2
height-only pixels=0 max=0.000000
fill 4x4
fill pixels=0 max=0.000000
override 4x2
