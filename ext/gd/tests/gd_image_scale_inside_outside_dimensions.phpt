--TEST--
GdImage::scale() inside and outside fit dimensions
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_scale_supported.inc";
if (!method_exists(GdImage::class, "scale")) {
    die("skip GdImage::scale() not available.");
}
?>
--FILE--
<?php
function source(int $width, int $height): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagefill($image, 0, 0, imagecolorallocate($image, 20, 80, 160));
    return $image;
}

function show(string $label, GdImage $image): void {
    printf("%s %dx%d\n", $label, imagesx($image), imagesy($image));
}

$wide = source(400, 200);
show('wide inside', $wide->scale(100, 100, new Gd\ScaleOptions(fit: Gd\ScaleFit::Inside)));
show('wide outside', $wide->scale(100, 100, new Gd\ScaleOptions(fit: Gd\ScaleFit::Outside)));

$tall = source(200, 400);
show('tall inside', $tall->scale(100, 100, new Gd\ScaleOptions(fit: Gd\ScaleFit::Inside)));
show('tall outside', $tall->scale(100, 100, new Gd\ScaleOptions(fit: Gd\ScaleFit::Outside)));

$square = source(100, 100);
show('square inside', $square->scale(50, 50, new Gd\ScaleOptions(fit: Gd\ScaleFit::Inside)));
show('square outside', $square->scale(50, 50, new Gd\ScaleOptions(fit: Gd\ScaleFit::Outside)));
?>
--EXPECT--
wide inside 100x50
wide outside 200x100
tall inside 50x100
tall outside 100x200
square inside 50x50
square outside 50x50
