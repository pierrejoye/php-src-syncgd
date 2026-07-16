--TEST--
GdImage::scale() entropy strategy prefers the stronger competing detail region
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_scale_supported.inc";
if (!method_exists(GdImage::class, "scale")) {
    die("skip GdImage::scale() not available.");
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
function count_color(GdImage $image, int $color): int {
    $count = 0;
    for ($y = 0; $y < imagesy($image); $y++) {
        for ($x = 0; $x < imagesx($image); $x++) {
            if (imagecolorat($image, $x, $y) === $color) {
                $count++;
            }
        }
    }
    return $count;
}

$image = imagecreatetruecolor(48, 16);
$blue = imagecolorallocate($image, 20, 74, 122);
$red = imagecolorallocate($image, 230, 40, 40);
$black = imagecolorallocate($image, 0, 0, 0);
$white = imagecolorallocate($image, 255, 255, 255);

imagefill($image, 0, 0, $blue);

for ($y = 5; $y < 11; $y++) {
    for ($x = 2; $x < 8; $x++) {
        imagesetpixel($image, $x, $y, (($x + $y) & 1) ? $red : $white);
    }
}

for ($y = 0; $y < 16; $y++) {
    for ($x = 32; $x < 48; $x++) {
        imagesetpixel($image, $x, $y, (($x + $y) & 1) ? $black : $white);
    }
}

$entropy = $image->scale(16, 16, new Gd\ScaleOptions(
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
$center = $image->scale(16, 16, new Gd\ScaleOptions(
    gravity: Gd\ScaleGravity::Center,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));

$entropyDetail = count_color($entropy, $black) + count_color($entropy, $white);
$centerDetail = count_color($center, $black) + count_color($center, $white);

printf("entropy detail=%d\n", $entropyDetail);
printf("center detail=%d\n", $centerDetail);
var_dump($entropyDetail > $centerDetail);
?>
--EXPECT--
entropy detail=12
center detail=0
bool(true)
