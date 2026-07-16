--TEST--
Gd\Pattern captures source image pixels
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\Pattern::class)) {
    die("skip Gd 2D pattern API not available.");
}
?>
--FILE--
<?php
$source = imagecreatetruecolor(2, 2);
imagefill($source, 0, 0, 0xff0000);

$image = imagecreatetruecolor(4, 4);
imagefill($image, 0, 0, 0x000000);
$ctx = $image->getContext();
$pattern = $ctx->createPattern($source);

imagefill($source, 0, 0, 0x0000ff);

var_dump($pattern instanceof Gd\Pattern);
var_dump($pattern instanceof Gd\Paint);
var_dump($ctx->setSource($pattern) === $ctx);
$ctx->paint()->flushImage();
var_dump(imagecolorat($image, 0, 0));
var_dump(imagecolorat($image, 3, 3));

$repeat = imagecreatetruecolor(4, 4);
imagefill($repeat, 0, 0, 0x000000);
$repeatCtx = $repeat->getContext();
$repeatPattern = $repeatCtx->createPattern($source)->setExtend(Gd\ExtendMode::Repeat);
$repeatCtx->setSource($repeatPattern)->paint()->flushImage();
var_dump(imagecolorat($repeat, 3, 3));

$matrixSource = imagecreatetruecolor(2, 1);
imagesetpixel($matrixSource, 0, 0, 0xff0000);
imagesetpixel($matrixSource, 1, 0, 0x0000ff);
$matrixImage = imagecreatetruecolor(2, 1);
imagefill($matrixImage, 0, 0, 0x000000);
$matrixCtx = $matrixImage->getContext();
$matrixPattern = $matrixCtx->createPattern($matrixSource)
    ->setExtend(Gd\ExtendMode::Repeat)
    ->setMatrix(Gd\Matrix::translation(1, 0));
$matrixCtx->setSource($matrixPattern)->paint()->flushImage();
var_dump(imagecolorat($matrixImage, 0, 0));

$opacity = imagecreatetruecolor(2, 2);
imagefill($opacity, 0, 0, 0x000000);
$opacityCtx = $opacity->getContext();
$opacityPattern = $opacityCtx->createPattern($source)->setOpacity(0.5);
var_dump($opacityPattern instanceof Gd\Pattern);
$opacityCtx->setSource($opacityPattern)->paint()->flushImage();
$pixel = imagecolorat($opacity, 0, 0);
var_dump((($pixel >> 16) & 0xff) > 0);
var_dump((($pixel >> 8) & 0xff) === 0);
var_dump(($pixel & 0xff) > 0);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
int(16711680)
int(0)
int(255)
int(255)
bool(true)
bool(false)
bool(true)
bool(true)
