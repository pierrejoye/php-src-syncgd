--TEST--
GdImage::scale() default interpolation resolves by scale direction
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_scale_supported.inc";
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
function source_image(int $width, int $height): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, imagecolorallocatealpha($image, 255, 255, 255, 0));

    for ($y = 0; $y < $height; $y++) {
        for ($x = 0; $x < $width; $x++) {
            $r = ($x * 17 + $y * 3) % 256;
            $g = ($x * 5 + $y * 19) % 256;
            $b = ($x * 11 + $y * 7) % 256;
            imagesetpixel($image, $x, $y, imagecolorallocatealpha($image, $r, $g, $b, 0));
        }
    }

    return $image;
}

function assert_same_image(string $label, GdImage $actual, GdImage $expected): void {
    $diff = $actual->perceptualDiff($expected, 0.0);
    printf("%s pixels changed: %d\n", $label, $diff->pixelsChanged);
}

$downSource = source_image(80, 50);
$downDefault = $downSource->scale(20, 10, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill));
$downLanczos3 = $downSource->scale(20, 10, new Gd\ScaleOptions(
	fit: Gd\ScaleFit::Fill,
	interpolation: Gd\ScaleInterpolation::Lanczos3,
));
$downCubicSpline = $downSource->scale(20, 10, new Gd\ScaleOptions(
	fit: Gd\ScaleFit::Fill,
	interpolation: Gd\ScaleInterpolation::CubicSpline,
));

$upSource = source_image(10, 8);
$upDefault = $upSource->scale(40, 32, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill));
$upCubicSpline = $upSource->scale(40, 32, new Gd\ScaleOptions(
	fit: Gd\ScaleFit::Fill,
	interpolation: Gd\ScaleInterpolation::CubicSpline,
));
$upCatmullRom = $upSource->scale(40, 32, new Gd\ScaleOptions(
    fit: Gd\ScaleFit::Fill,
    interpolation: Gd\ScaleInterpolation::CatmullRom,
));
$explicitDefault = $upSource->scale(40, 32, new Gd\ScaleOptions(
	fit: Gd\ScaleFit::Fill,
	interpolation: Gd\ScaleInterpolation::Default,
));

assert_same_image('down default vs lanczos3', $downDefault, $downLanczos3);
printf("down default differs from cubic spline: %s\n", $downDefault->perceptualDiff($downCubicSpline, 0.0)->pixelsChanged > 0 ? 'yes' : 'no');

assert_same_image('up default vs catmullrom', $upDefault, $upCatmullRom);
printf("up default differs from cubic spline: %s\n", $upDefault->perceptualDiff($upCubicSpline, 0.0)->pixelsChanged > 0 ? 'yes' : 'no');
printf("auto differs from explicit GD_DEFAULT: %s\n", $upDefault->perceptualDiff($explicitDefault, 0.0)->pixelsChanged > 0 ? 'yes' : 'no');
?>
--EXPECT--
down default vs lanczos3 pixels changed: 0
down default differs from cubic spline: yes
up default vs catmullrom pixels changed: 0
up default differs from cubic spline: yes
auto differs from explicit GD_DEFAULT: yes
