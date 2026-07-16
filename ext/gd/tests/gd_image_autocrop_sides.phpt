--TEST--
GdImage::autoCrop() sides mode
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
$image = imagecreatetruecolor(18, 12);
$background = imagecolorallocate($image, 30, 40, 50);
$blue = imagecolorallocate($image, 10, 80, 210);
imagefill($image, 0, 0, $background);
imagefilledrectangle($image, 5, 2, 13, 8, $blue);

$cropped = $image->autoCrop(new Gd\AutoCropOptions(mode: Gd\AutoCropMode::Sides));

var_dump(imagesx($cropped));
var_dump(imagesy($cropped));
printf("%06X\n", imagecolorat($cropped, 0, 0));
printf("%06X\n", imagecolorat($cropped, 8, 6));
?>
--EXPECT--
int(9)
int(7)
0A50D2
0A50D2
