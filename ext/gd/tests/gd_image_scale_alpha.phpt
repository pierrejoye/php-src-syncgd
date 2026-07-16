--TEST--
GdImage::scale() preserves alpha through fill, contain, cover, and source state
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
function make_alpha_source(): GdImage {
    $image = imagecreatetruecolor(2, 2);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagesetpixel($image, 0, 0, imagecolorallocatealpha($image, 255, 0, 0, 63));
    imagesetpixel($image, 1, 0, imagecolorallocatealpha($image, 0, 255, 0, 31));
    imagesetpixel($image, 0, 1, imagecolorallocatealpha($image, 0, 0, 255, 95));
    imagesetpixel($image, 1, 1, imagecolorallocatealpha($image, 255, 255, 255, 0));
    return $image;
}

function assert_color(string $label, GdImage $image, int $x, int $y, int $expected): void {
    $actual = imagecolorat($image, $x, $y);
    if ($actual !== $expected) {
        printf("%s expected=%08X actual=%08X\n", $label, $expected, $actual);
    }
}

function assert_similar(string $label, GdImage $actual, GdImage $expected): void {
    $diff = $actual->perceptualDiff($expected, 0.03);
    printf("%s pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
}

function expected_contain(GdImage $source, int $background): GdImage {
    $scaled = imagescale($source, 4, 4, IMG_NEAREST_NEIGHBOUR);
    $canvas = imagecreatetruecolor(4, 6);
    imagealphablending($canvas, false);
    imagesavealpha($canvas, true);
    imagefill($canvas, 0, 0, $background);
    imagecopy($canvas, $scaled, 0, 1, 0, 0, 4, 4);
    return $canvas;
}

function expected_cover(GdImage $source, int $background): GdImage {
    $scaled = imagescale($source, 6, 6, IMG_NEAREST_NEIGHBOUR);
    $canvas = imagecreatetruecolor(6, 4);
    imagealphablending($canvas, false);
    imagesavealpha($canvas, true);
    imagefill($canvas, 0, 0, $background);
    imagecopy($canvas, $scaled, 0, 0, 0, 0, 6, 4);
    return $canvas;
}

$source = make_alpha_source();
$redAlpha63 = imagecolorat($source, 0, 0);
$greenAlpha31 = imagecolorat($source, 1, 0);
$blueAlpha95 = imagecolorat($source, 0, 1);
$background = imagecolorallocatealpha($source, 0, 255, 0, 50);

$fill = $source->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill, interpolation: Gd\ScaleInterpolation::NearestNeighbour));
assert_similar('fill', $fill, imagescale($source, 4, 4, IMG_NEAREST_NEIGHBOUR));
assert_color('fill keeps semi-transparent red', $fill, 0, 0, $redAlpha63);
assert_color('fill keeps semi-transparent blue', $fill, 0, 2, $blueAlpha95);

$contain = $source->scale(4, 6, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Contain,
    backgroundColor: $background,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
assert_similar('contain', $contain, expected_contain($source, $background));
assert_color('contain keeps background alpha', $contain, 0, 0, $background);
assert_color('contain copy keeps semi-transparent red', $contain, 0, 1, $redAlpha63);
assert_color('contain copy keeps semi-transparent blue', $contain, 0, 3, $blueAlpha95);

$cover = $source->scale(6, 4, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Cover,
    gravity: Gd\ScaleGravity::North,
    backgroundColor: $background,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
assert_similar('cover', $cover, expected_cover($source, $background));
assert_color('cover copy keeps semi-transparent red', $cover, 0, 0, $redAlpha63);
assert_color('cover copy keeps semi-transparent green', $cover, 3, 0, $greenAlpha31);

$control = imagecreatetruecolor(1, 1);
imagefill($control, 0, 0, 0x00000000);
$blendingSource = imagecreatetruecolor(1, 1);
imagefill($blendingSource, 0, 0, 0x00000000);
imagealphablending($control, true);
imagealphablending($blendingSource, true);
$blendingSource->scale(2, 2);
imagesetpixel($control, 0, 0, 0x40ffffff);
imagesetpixel($blendingSource, 0, 0, 0x40ffffff);
var_dump(imagecolorat($blendingSource, 0, 0) === imagecolorat($control, 0, 0));
?>
--EXPECT--
fill pixels=0 max=0.000000
contain pixels=0 max=0.000000
cover pixels=0 max=0.000000
bool(true)
