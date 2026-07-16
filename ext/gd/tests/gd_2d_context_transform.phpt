--TEST--
Gd\Context transforms affect rendered output
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\Matrix::class)) {
    die("skip Gd 2D transform API not available.");
}
?>
--FILE--
<?php
function red_at(GdImage $image, int $x, int $y): int {
    return (imagecolorat($image, $x, $y) >> 16) & 0xff;
}

$image = imagecreatetruecolor(5, 5);
imagefilledrectangle($image, 0, 0, 4, 4, 0x000000);

$image
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->translate(2, 1)
    ->beginPath()
    ->rect(0, 0, 1, 1)
    ->fill()
    ->flushImage();

var_dump(red_at($image, 0, 0) < 10);
var_dump(red_at($image, 2, 1) > 200);

$image = imagecreatetruecolor(5, 5);
imagefilledrectangle($image, 0, 0, 4, 4, 0x000000);

$image
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->transform(Gd\Matrix::translation(1, 2))
    ->beginPath()
    ->rect(0, 0, 1, 1)
    ->fill()
    ->flushImage();

var_dump(red_at($image, 0, 0) < 10);
var_dump(red_at($image, 1, 2) > 200);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
