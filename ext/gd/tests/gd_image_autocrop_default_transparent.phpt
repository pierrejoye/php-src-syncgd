--TEST--
GdImage::autoCrop() default transparent mode
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
$image = imagecreatetruecolor(16, 14);
$transparent = imagecolorallocate($image, 10, 20, 30);
$red = imagecolorallocate($image, 200, 40, 60);
imagecolortransparent($image, $transparent);
imagefill($image, 0, 0, $transparent);
imagefilledrectangle($image, 4, 3, 10, 8, $red);

$cropped = $image->autoCrop();

var_dump(imagesx($cropped));
var_dump(imagesy($cropped));
printf("%06X\n", imagecolorat($cropped, 0, 0));
printf("%06X\n", imagecolorat($cropped, 6, 5));
?>
--EXPECT--
int(7)
int(6)
C8283C
C8283C
