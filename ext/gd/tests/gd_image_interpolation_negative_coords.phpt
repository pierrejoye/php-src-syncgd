--TEST--
Interpolation uses floor for negative fractional source coordinates
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!method_exists(GdImage::class, "transform") || !method_exists(GdImage::class, "rotate")) {
    die("skip fluent transform and rotate APIs are not available.");
}
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
function make_interpolation_fixture(int $padding = 0): GdImage {
    $width = 6 + 2 * $padding;
    $height = 5 + 2 * $padding;
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, 0x7f000000);

    $colors = [
        [0x20, 0x70, 0xd0], [0xe0, 0x30, 0x40], [0x30, 0xb0, 0x60],
        [0xf0, 0xb0, 0x20], [0x70, 0x40, 0xc0], [0x20, 0xc0, 0xc0],
    ];
    for ($y = 0; $y < 5; $y++) {
        for ($x = 0; $x < 6; $x++) {
            [$r, $g, $b] = $colors[($x + 2 * $y) % count($colors)];
            imagesetpixel($image, $padding + $x, $padding + $y, ($r << 16) | ($g << 8) | $b);
        }
    }

    imagesetinterpolation($image, IMG_CATMULLROM);
    return $image;
}

function crop_center(GdImage $image, int $width, int $height): GdImage {
    $cropped = imagecreatetruecolor($width, $height);
    imagealphablending($cropped, false);
    imagesavealpha($cropped, true);
    imagefill($cropped, 0, 0, 0x7f000000);
    imagecopy($cropped, $image, 0, 0, intdiv(imagesx($image) - $width, 2), intdiv(imagesy($image) - $height, 2), $width, $height);
    return $cropped;
}

function assert_similar(string $label, GdImage $actual, GdImage $expected): void {
    $diff = $actual->perceptualDiff($expected, 0.03);
    if ($diff->pixelsChanged > 0 || $diff->maximumDelta > 0.03) {
        printf("%s differs: pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
        return;
    }
    echo "$label ok\n";
}

$angle = M_PI / 7;
$source = make_interpolation_fixture();
$paddedSource = make_interpolation_fixture(4);

$actual = $source->transform(
    Gd\Matrix::rotationAround($angle, 3, 2.5),
    new Gd\TransformOptions(interpolation: Gd\InterpolationMethod::CatmullRom),
);
$padded = $paddedSource->transform(
    Gd\Matrix::rotationAround($angle, 7, 6.5),
    new Gd\TransformOptions(interpolation: Gd\InterpolationMethod::CatmullRom),
);
assert_similar('transform negative coordinates', $actual, crop_center($padded, imagesx($actual), imagesy($actual)));

$weighted = $source->transform(
    Gd\Matrix::rotationAround($angle, 3, 2.5),
    new Gd\TransformOptions(interpolation: Gd\InterpolationMethod::Weighted4),
);
$paddedWeighted = $paddedSource->transform(
    Gd\Matrix::rotationAround($angle, 7, 6.5),
    new Gd\TransformOptions(interpolation: Gd\InterpolationMethod::Weighted4),
);
assert_similar('weighted-4 negative coordinates', $weighted, crop_center($paddedWeighted, imagesx($weighted), imagesy($weighted)));

$rotated = $source->rotate($angle * 180 / M_PI);
$paddedRotated = $paddedSource->rotate($angle * 180 / M_PI);
assert_similar('rotate negative coordinates', $rotated, crop_center($paddedRotated, imagesx($rotated), imagesy($rotated)));
?>
--EXPECT--
transform negative coordinates ok
weighted-4 negative coordinates ok
rotate negative coordinates ok
