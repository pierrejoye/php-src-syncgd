--TEST--
GdImage::scale() contain mode and background color
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
    $image = imagecreatetruecolor(4, 2);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, imagecolorallocatealpha($image, 0, 0, 0, 127));
    imagefilledrectangle($image, 1, 0, 3, 1, imagecolorallocatealpha($image, 20, 100, 220, 0));
    return $image;
}

function prepare_reference(GdImage $image, int $backgroundColor): GdImage {
    $copy = imagecreatetruecolor(imagesx($image), imagesy($image));
    imagealphablending($copy, false);
    imagesavealpha($copy, true);
    imagecopy($copy, $image, 0, 0, 0, 0, imagesx($image), imagesy($image));

    for ($y = 0; $y < imagesy($copy); $y++) {
        for ($x = 0; $x < imagesx($copy); $x++) {
            if ((imagecolorat($copy, $x, $y) >> 24) === 0x7f) {
                imagesetpixel($copy, $x, $y, $backgroundColor);
            }
        }
    }

    return $copy;
}

function contain_reference(GdImage $image, int $width, int $height, int $backgroundColor): GdImage {
    $prepared = prepare_reference($image, $backgroundColor);
    $scaled = imagescale($prepared, 8, 4, IMG_NEAREST_NEIGHBOUR);
    $canvas = imagecreatetruecolor($width, $height);
    imagealphablending($canvas, false);
    imagesavealpha($canvas, true);
    imagefill($canvas, 0, 0, $backgroundColor);
    imagecopy($canvas, $scaled, 0, 2, 0, 0, imagesx($scaled), imagesy($scaled));
    return $canvas;
}

function assert_similar(string $label, GdImage $actual, GdImage $expected): void {
    $diff = $actual->perceptualDiff($expected, 0.03);
    printf("%s pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
}

$source = make_source();

$default = $source->scale(8, 8, new Gd\ScaleOptions(fit: Gd\ScaleFit::Contain, interpolation: Gd\ScaleInterpolation::NearestNeighbour));
assert_similar('default', $default, contain_reference($source, 8, 8, 0x7f000000));
printf("default-corner %08X\n", imagecolorat($default, 0, 0));

$background = imagecolorallocatealpha($source, 0, 255, 0, 50);
$custom = $source->scale(8, 8, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Contain,
    backgroundColor: $background,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
assert_similar('custom', $custom, contain_reference($source, 8, 8, $background));
printf("custom-corner %08X\n", imagecolorat($custom, 0, 0));
printf("custom-prepared-transparent %08X\n", imagecolorat($custom, 0, 2));
?>
--EXPECT--
default pixels=0 max=0.000000
default-corner 7F000000
custom pixels=0 max=0.000000
custom-corner 3200FF00
custom-prepared-transparent 3200FF00
