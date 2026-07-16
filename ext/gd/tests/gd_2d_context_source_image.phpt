--TEST--
Gd\Context image sources
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !method_exists(Gd\Context::class, "setSourceImage")) {
    die("skip Gd\\Context::setSourceImage() not available.");
}
?>
--FILE--
<?php
function truecolor_image(int $width, int $height, int $color = 0x000000): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagefilledrectangle($image, 0, 0, $width - 1, $height - 1, $color);
    return $image;
}

function color_at(GdImage $image, int $x, int $y): int {
    return imagecolorat($image, $x, $y) & 0xffffff;
}

$source = truecolor_image(2, 2);
imagesetpixel($source, 0, 0, 0xff0000);
imagesetpixel($source, 1, 0, 0x00ff00);
imagesetpixel($source, 0, 1, 0x0000ff);
imagesetpixel($source, 1, 1, 0xffffff);

$offset = truecolor_image(5, 5);
$ctx = $offset->getContext();
var_dump($ctx->setSourceImage($source, 2, 1) === $ctx);

imagefilledrectangle($source, 0, 0, 1, 1, 0x00ffff);
unset($source);

$ctx->paint()->flushImage();
var_dump(color_at($offset, 1, 1));
var_dump(color_at($offset, 2, 1));
var_dump(color_at($offset, 3, 1));
var_dump(color_at($offset, 2, 2));
var_dump(color_at($offset, 3, 2));
var_dump(color_at($offset, 4, 2));

$paletteSource = imagecreate(1, 1);
$paletteRed = imagecolorallocate($paletteSource, 255, 0, 0);
imagefill($paletteSource, 0, 0, $paletteRed);
$paletteTarget = truecolor_image(1, 1);
$paletteTarget->getContext()->setSourceImage($paletteSource)->paint()->flushImage();
var_dump(color_at($paletteTarget, 0, 0));

$alphaSource = truecolor_image(1, 1);
imagealphablending($alphaSource, false);
imagesavealpha($alphaSource, true);
imagesetpixel($alphaSource, 0, 0, 0x40ff0000);
$alphaTarget = truecolor_image(1, 1, 0x00ff00);
$alphaTarget->getContext()->setSourceImage($alphaSource)->paint()->flushImage();
[$r, $g, $b] = [
    (color_at($alphaTarget, 0, 0) >> 16) & 0xff,
    (color_at($alphaTarget, 0, 0) >> 8) & 0xff,
    color_at($alphaTarget, 0, 0) & 0xff,
];
var_dump($r > 80 && $r < 180);
var_dump($g > 80 && $g < 180);
var_dump($b < 10);

$clipSource = truecolor_image(3, 3, 0xff0000);
$clipTarget = truecolor_image(5, 5);
$clipTarget
    ->getContext()
    ->beginPath()
    ->rect(2, 2, 1, 1)
    ->clip()
    ->setSourceImage($clipSource, 1, 1)
    ->paint()
    ->flushImage();
var_dump(color_at($clipTarget, 1, 1));
var_dump(color_at($clipTarget, 2, 2));
var_dump(color_at($clipTarget, 3, 3));

$sourceForComposition = truecolor_image(1, 1, 0xff0000);
$composition = truecolor_image(1, 1, 0x00ff00);
$composition
    ->getContext()
    ->setOperator(Gd\CompositeOperator::Multiply)
    ->setSourceImage($sourceForComposition)
    ->paint()
    ->flushImage();
var_dump(color_at($composition, 0, 0));

foreach ([INF, NAN] as $bad) {
    try {
        truecolor_image(1, 1)->getContext()->setSourceImage(truecolor_image(1, 1), $bad, 0);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

$destroyed = truecolor_image(1, 1)->getContext();
$destroyed->destroy(false);
try {
    $destroyed->setSourceImage(truecolor_image(1, 1));
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
bool(true)
int(0)
int(16711680)
int(65280)
int(255)
int(16777215)
int(0)
int(16711680)
bool(true)
bool(true)
bool(true)
int(0)
int(16711680)
int(0)
int(0)
Gd\Context::setSourceImage(): Argument #2 ($x) must be finite
Gd\Context::setSourceImage(): Argument #2 ($x) must be finite
Gd\Context has already been destroyed
