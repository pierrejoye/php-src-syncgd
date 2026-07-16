--TEST--
GdImage::autoCrop() threshold mode
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_autocrop_supported.inc";
if (!method_exists(GdImage::class, "autoCrop")) {
    die("skip GdImage::autoCrop() not available.");
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(20, 16);
$background = imagecolorallocate($image, 250, 250, 250);
$nearBackground = imagecolorallocate($image, 248, 249, 250);
$green = imagecolorallocate($image, 40, 180, 90);
imagefill($image, 0, 0, $background);
imagefilledrectangle($image, 1, 1, 18, 14, $nearBackground);
imagefilledrectangle($image, 6, 5, 12, 10, $green);

$cropped = $image->autoCrop(new Gd\AutoCropOptions(
    mode: Gd\AutoCropMode::Threshold,
    threshold: 0.5,
    color: $background,
));

var_dump(imagesx($cropped));
var_dump(imagesy($cropped));
printf("%06X\n", imagecolorat($cropped, 0, 0));
printf("%06X\n", imagecolorat($cropped, 6, 5));
?>
--EXPECT--
int(7)
int(6)
28B45A
28B45A
