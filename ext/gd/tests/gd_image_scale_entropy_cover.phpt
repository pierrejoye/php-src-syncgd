--TEST--
GdImage::scale() entropy strategy selects high-detail cover region
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
function make_entropy_source(): GdImage {
    $image = imagecreatetruecolor(16, 8);
    imagealphablending($image, false);
    imagesavealpha($image, true);

    $blue = imagecolorallocatealpha($image, 0, 0, 255, 0);
    $black = imagecolorallocatealpha($image, 0, 0, 0, 0);
    $white = imagecolorallocatealpha($image, 255, 255, 255, 0);
    imagefill($image, 0, 0, $blue);

    for ($y = 0; $y < 8; $y++) {
        for ($x = 10; $x < 16; $x++) {
            imagesetpixel($image, $x, $y, (($x + $y) & 1) ? $black : $white);
        }
    }

    return $image;
}

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

$source = make_entropy_source();
$black = imagecolorallocatealpha($source, 0, 0, 0, 0);
$white = imagecolorallocatealpha($source, 255, 255, 255, 0);

$entropy = $source->scale(4, 4, new Gd\ScaleOptions(
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));
$center = $source->scale(4, 4, new Gd\ScaleOptions(
    gravity: Gd\ScaleGravity::Center,
    interpolation: Gd\ScaleInterpolation::NearestNeighbour,
));

printf("entropy black=%d white=%d\n", count_color($entropy, $black), count_color($entropy, $white));
printf("center black=%d white=%d\n", count_color($center, $black), count_color($center, $white));
var_dump(
    count_color($entropy, $black) + count_color($entropy, $white)
    > count_color($center, $black) + count_color($center, $white)
);
?>
--EXPECT--
entropy black=0 white=12
center black=0 white=4
bool(true)
