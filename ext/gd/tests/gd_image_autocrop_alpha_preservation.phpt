--TEST--
GdImage::autoCrop() preserves alpha pixels and leaves source unchanged
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
$image = imagecreatetruecolor(12, 10);
imagealphablending($image, false);
imagesavealpha($image, true);
$transparent = imagecolorallocatealpha($image, 0, 0, 0, 127);
$semiRed = imagecolorallocatealpha($image, 220, 20, 40, 63);
imagefill($image, 0, 0, $transparent);
imagefilledrectangle($image, 3, 2, 8, 6, $semiRed);

$before = imagecolorat($image, 3, 2);
$cropped = $image->autoCrop(new Gd\AutoCropOptions(mode: Gd\AutoCropMode::Sides));
$after = imagecolorat($image, 3, 2);

var_dump(imagesx($cropped));
var_dump(imagesy($cropped));
printf("%08X\n", imagecolorat($cropped, 0, 0));
var_dump($before === $after);
var_dump(imagesx($image));
var_dump(imagesy($image));
?>
--EXPECT--
int(6)
int(5)
3FDC1428
bool(true)
int(12)
int(10)
