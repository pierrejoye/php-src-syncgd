--TEST--
Gd\Context clipping path with transparent output
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class)) {
    die("skip Gd\\Context not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
if (!function_exists("imagecreatefrompng")) {
    die("skip PNG support not available.");
}
if (!is_file(__DIR__ . "/gd_2d_pathclip.png")) {
    die("skip pathclip reference image not available.");
}
?>
--FILE--
<?php
const GD_2D_PI = 3.14159265358979323846;

function vector2d_create_image(int $width, int $height, int $color): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagefilledrectangle($image, 0, 0, $width - 1, $height - 1, $color);
    imagealphablending($image, true);
    imagesavealpha($image, true);
    return $image;
}

$width = 256;
$height = 256;
$image = vector2d_create_image($width, $height, 0x7fffffff);
$ctx = $image->getContext();

$xc = 128.0;
$yc = 128.0;
$radius = 100.0;

$ctx
    ->setLineWidth(10.0)
    ->arc($xc, $yc, $radius, 0, GD_2D_PI * 2)
    ->clip()
    ->setSourceRgba(1, 0.2, 0.2, 0.6)
    ->setLineWidth(40.0)
    ->arc($xc - 100, $yc - 100, 10.0, 0, 2 * GD_2D_PI)
    ->fill()
    ->moveTo(0, 0)
    ->lineTo($width, $height)
    ->moveTo($width, 0)
    ->lineTo(0, $height)
    ->stroke()
    ->flushImage();

$reference = imagecreatefrompng(__DIR__ . "/gd_2d_pathclip.png");
var_dump(imagesx($image));
var_dump(imagesy($image));
var_dump(imagesx($reference));
var_dump(imagesy($reference));

$diff = $image->perceptualDiff($reference, 0.03);
var_dump($diff->pixelsChanged <= 16);
var_dump($diff->maximumDelta < 0.05);

?>
--EXPECT--
int(256)
int(256)
int(256)
int(256)
bool(true)
bool(true)
