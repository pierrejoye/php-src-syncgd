--TEST--
Gd\Context fills clipped arc with image source
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !method_exists(Gd\Context::class, "setSourceImage")) {
    die("skip Gd\\Context::setSourceImage() not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
if (!function_exists("imagecreatefrompng")) {
    die("skip PNG support not available.");
}
if (!is_file(__DIR__ . "/gd_2d_path_arcfillimage.png")) {
    die("skip path arc image-fill reference image not available.");
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

function vector2d_create_checker(int $width, int $height): GdImage {
    $image = vector2d_create_image($width, $height, 0x00f0f0f0);
    $dark = 0x00375f96;
    $light = 0x00f5aa41;
    $size = max(intdiv($width, 8), 4);
    for ($y = 0; $y < $height; $y += $size) {
        for ($x = 0; $x < $width; $x += $size) {
            imagefilledrectangle(
                $image,
                $x,
                $y,
                $x + $size - 1,
                $y + $size - 1,
                ((intdiv($x, $size) + intdiv($y, $size)) & 1) ? $dark : $light
            );
        }
    }
    return $image;
}

$source = vector2d_create_checker(128, 128);
$image = vector2d_create_image(256, 256, 0x7fffffff);
$ctx = $image->getContext();
$ctx
    ->arc(128, 128, 76.8, 0, 2 * GD_2D_PI)
    ->clip()
    ->scale(2, 2)
    ->setSourceImage($source, 0, 0)
    ->paint()
    ->flushImage();

$reference = imagecreatefrompng(__DIR__ . "/gd_2d_path_arcfillimage.png");
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
