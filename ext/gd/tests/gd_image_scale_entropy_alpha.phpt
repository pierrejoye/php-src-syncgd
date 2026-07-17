--TEST--
GdImage::scale() entropy strategy preserves alpha in selected cover region
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
$image = imagecreatetruecolor(16, 8);
imagealphablending($image, false);
imagesavealpha($image, true);

$blue = imagecolorallocatealpha($image, 0, 0, 255, 0);
$redAlpha63 = imagecolorallocatealpha($image, 255, 0, 0, 63);
$greenAlpha31 = imagecolorallocatealpha($image, 0, 255, 0, 31);
imagefill($image, 0, 0, $blue);

for ($y = 0; $y < 8; $y++) {
    for ($x = 10; $x < 16; $x++) {
        imagesetpixel($image, $x, $y, (($x + $y) & 1) ? $redAlpha63 : $greenAlpha31);
    }
}

$scaled = $image->scale(4, 4, new Gd\ScaleOptions(
    strategy: Gd\ScaleStrategy::Entropy,
    interpolation: Gd\InterpolationMethod::NearestNeighbour,
));

var_dump(imagecolorat($scaled, 2, 0) === $redAlpha63 || imagecolorat($scaled, 2, 0) === $greenAlpha31);
printf("%08X\n", imagecolorat($scaled, 2, 0));
?>
--EXPECT--
bool(true)
1F00FF00
