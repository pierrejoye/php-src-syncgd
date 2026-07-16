--TEST--
Gd\Pattern repeated source image through rotated context
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\Pattern::class)) {
    die("skip Gd 2D pattern API not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
if (!function_exists("imagecreatefrompng")) {
    die("skip PNG support not available.");
}
if (!is_file(__DIR__ . "/gd_2d_path_pattern.png")) {
    die("skip path pattern reference image not available.");
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

$width = 512;
$height = 512;
$source = vector2d_create_checker(128, 128);
$image = vector2d_create_image($width, $height, 0x00141928);
$ctx = $image->getContext();
$pattern = $ctx
    ->createPattern($source)
    ->setExtend(Gd\ExtendMode::Repeat)
    ->setMatrix(Gd\Matrix::scaling(0.4, 0.4));

$ctx
    ->setSource($pattern)
    ->translate($width / 2.0, $height / 2.0)
    ->rotate(GD_2D_PI / 4)
    ->scale(1 / sqrt(2.0), 1 / sqrt(2.0))
    ->translate(-$width / 2.0, -$height / 2.0)
    ->rect(0, 0, $width, $height)
    ->fill()
    ->flushImage();

$reference = imagecreatefrompng(__DIR__ . "/gd_2d_path_pattern.png");
var_dump(imagesx($image));
var_dump(imagesy($image));
var_dump(imagesx($reference));
var_dump(imagesy($reference));
$diff = $image->perceptualDiff($reference, 0.03);
$pixels_ok = $diff->pixelsChanged <= 16;
$delta_ok = $diff->maximumDelta < 0.05;

if (!$pixels_ok || !$delta_ok) {
    imagepng($image, __DIR__ . "/gd_2d_path_pattern.actual.png");
    imagepng($reference, __DIR__ . "/gd_2d_path_pattern.reference.png");

    $overlay = $image->perceptualDiff(
        $reference,
        0.03,
        new Gd\PerceptualDiffOptions(Gd\PerceptualDiffMode::Overlay, 0xff00ff),
    );
    if ($overlay->diffImage instanceof GdImage) {
        imagepng($overlay->diffImage, __DIR__ . "/gd_2d_path_pattern.diff-overlay.png");
    }

    $mask = $image->perceptualDiff(
        $reference,
        0.03,
        new Gd\PerceptualDiffOptions(Gd\PerceptualDiffMode::Mask),
    );
    if ($mask->diffImage instanceof GdImage) {
        imagepng($mask->diffImage, __DIR__ . "/gd_2d_path_pattern.diff-mask.png");
    }
}

var_dump($pixels_ok);
var_dump($delta_ok);

?>
--EXPECT--
int(512)
int(512)
int(512)
int(512)
bool(true)
bool(true)
