--TEST--
GdImage::scale() inside and outside preserve alpha without canvas padding or cropping
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
function alpha_source(): GdImage {
    $image = imagecreatetruecolor(2, 2);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagesetpixel($image, 0, 0, imagecolorallocatealpha($image, 255, 0, 0, 63));
    imagesetpixel($image, 1, 0, imagecolorallocatealpha($image, 0, 255, 0, 31));
    imagesetpixel($image, 0, 1, imagecolorallocatealpha($image, 0, 0, 255, 95));
    imagesetpixel($image, 1, 1, imagecolorallocatealpha($image, 255, 255, 255, 10));
    return $image;
}

function assert_similar(string $label, GdImage $actual, GdImage $expected): void {
    if (imagesx($actual) !== imagesx($expected) || imagesy($actual) !== imagesy($expected)) {
        printf("%s dimensions differ: %dx%d vs %dx%d\n", $label, imagesx($actual), imagesy($actual), imagesx($expected), imagesy($expected));
        return;
    }

    $diff = $actual->perceptualDiff($expected, 0.03);
    printf("%s pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
}

function assert_color(string $label, GdImage $image, int $x, int $y, int $expected): void {
    $actual = imagecolorat($image, $x, $y);
    if ($actual !== $expected) {
        printf("%s expected=%08X actual=%08X\n", $label, $expected, $actual);
    }
}

$source = alpha_source();
$red = imagecolorat($source, 0, 0);
$green = imagecolorat($source, 1, 0);
$background = imagecolorallocatealpha($source, 255, 255, 0, 0);

$inside = $source->scale(4, 6, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Inside,
    backgroundColor: $background,
    interpolation: Gd\InterpolationMethod::NearestNeighbour,
));
printf("inside %dx%d\n", imagesx($inside), imagesy($inside));
assert_similar('inside', $inside, imagescale($source, 4, 4, IMG_NEAREST_NEIGHBOUR));
assert_color('inside red alpha', $inside, 0, 0, $red);
assert_color('inside green alpha', $inside, 2, 0, $green);

$outside = $source->scale(4, 6, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Outside,
    backgroundColor: $background,
    interpolation: Gd\InterpolationMethod::NearestNeighbour,
));
printf("outside %dx%d\n", imagesx($outside), imagesy($outside));
assert_similar('outside', $outside, imagescale($source, 6, 6, IMG_NEAREST_NEIGHBOUR));
assert_color('outside red alpha', $outside, 0, 0, $red);
assert_color('outside green alpha', $outside, 3, 0, $green);
?>
--EXPECT--
inside 4x4
inside pixels=0 max=0.000000
outside 6x6
outside pixels=0 max=0.000000
