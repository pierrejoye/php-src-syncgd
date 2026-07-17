--TEST--
GdImage interpolation keeps transformed transparent edges clean
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!method_exists(GdImage::class, "transform") || !method_exists(GdImage::class, "rotate")) {
    die("skip fluent transform and rotate APIs are not available.");
}
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
if (!is_file(__DIR__ . "/gd_image_interpolation_edges_affine.png") || !is_file(__DIR__ . "/gd_image_interpolation_edges_rotate.png")) {
    die("skip interpolation edge reference images are not available.");
}
?>
--FILE--
<?php
function make_edge_fixture(): GdImage {
    $width = 240;
    $height = 180;
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, 0x7fff00ff);

    imagefilledrectangle($image, 0, 0, $width - 1, 34, 0x003b82d0);
    imagefilledrectangle($image, 0, 145, $width - 1, $height - 1, 0x00c43b78);
    imagefilledrectangle($image, 0, 35, 34, 144, 0x0047ad68);
    imagefilledrectangle($image, 205, 35, $width - 1, 144, 0x00e18b32);
    imagefilledrectangle($image, 35, 35, 70, 144, 0x5035b7d4);
    imagefilledrectangle($image, 169, 35, 204, 144, 0x703f65d8);
    imagefilledrectangle($image, 71, 35, 168, 70, 0x3825c46a);
    imagefilledrectangle($image, 71, 109, 168, 144, 0x6030d0b0);
    imagefilledellipse($image, 120, 89, 72, 58, 0x7fff00ff);
    imagefilledellipse($image, 120, 89, 48, 36, 0x00f0d040);

    return $image;
}

function assert_reference(string $label, GdImage $actual, string $referencePath): void {
    $reference = imagecreatefrompng($referencePath);
    $diff = $actual->perceptualDiff($reference, 0.03);
    var_dump($label, imagesx($actual) === imagesx($reference), imagesy($actual) === imagesy($reference), $diff->pixelsChanged < 100, $diff->maximumDelta < 0.08);
}

$source = make_edge_fixture();
$matrix = (new Gd\Matrix())
    ->rotateAround(M_PI / 11, 120, 90)
    ->shear(0.10, -0.06)
    ->translate(10, -7);

$affine = $source->transform(
    $matrix,
    new Gd\TransformOptions(interpolation: Gd\InterpolationMethod::CatmullRom),
);

imagesetinterpolation($source, IMG_CATMULLROM);
$rotated = $source->rotate(27, new Gd\RotateOptions(backgroundColor: 0x7f000000));

assert_reference('affine', $affine, __DIR__ . '/gd_image_interpolation_edges_affine.png');
assert_reference('rotate', $rotated, __DIR__ . '/gd_image_interpolation_edges_rotate.png');
?>
--EXPECT--
string(6) "affine"
bool(true)
bool(true)
bool(true)
bool(true)
string(6) "rotate"
bool(true)
bool(true)
bool(true)
bool(true)
