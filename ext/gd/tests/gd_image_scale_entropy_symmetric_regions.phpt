--TEST--
GdImage::scale() entropy strategy handles symmetric competing detail regions deterministically
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
$black = imagecolorallocate($image, 0, 0, 0);
$white = imagecolorallocate($image, 255, 255, 255);

imagefill($image, 0, 0, $blue);

foreach ([[0, 16], [32, 48]] as [$x1, $x2]) {
    for ($y = 0; $y < 16; $y++) {
        for ($x = $x1; $x < $x2; $x++) {
            imagesetpixel($image, $x, $y, (($x + $y) & 1) ? $black : $white);
        }
    }
}

$entropy = $image->scale(16, 16, new Gd\ScaleOptions(
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
$west = $image->scale(16, 16, new Gd\ScaleOptions(
    gravity: Gd\ScaleGravity::West,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
$east = $image->scale(16, 16, new Gd\ScaleOptions(
    gravity: Gd\ScaleGravity::East,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));

$westDetail = count_color($west, $black) + count_color($west, $white);
$eastDetail = count_color($east, $black) + count_color($east, $white);
$entropyDetail = count_color($entropy, $black) + count_color($entropy, $white);

printf("entropy detail=%d west detail=%d east detail=%d\n", $entropyDetail, $westDetail, $eastDetail);
var_dump($entropyDetail === $westDetail || $entropyDetail === $eastDetail);
?>
--EXPECT--
entropy detail=256 west detail=256 east detail=256
bool(true)
