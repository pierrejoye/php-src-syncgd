--TEST--
Gd\Context fill path operations
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class)) {
    die("skip Gd\\Context not available.");
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(8, 8);
imagefill($image, 0, 0, 0x000000);

$ctx = $image->getContext();
$result = $ctx
    ->setSourceRgb(1, 0, 0)
    ->beginPath()
    ->rect(2, 2, 4, 4)
    ->fill();

var_dump($result === $ctx);
var_dump(imagecolorat($image, 3, 3));
$ctx->flushImage();
var_dump(imagecolorat($image, 0, 0));
var_dump(imagecolorat($image, 3, 3));
var_dump(imagecolorat($image, 2, 2));
var_dump(imagecolorat($image, 5, 5));
var_dump(imagecolorat($image, 6, 6));

$preserve = imagecreatetruecolor(8, 8);
imagefill($preserve, 0, 0, 0x000000);
$preserveCtx = $preserve->getContext();
$preserveCtx
    ->setSourceRgb(1, 0, 0)
    ->beginPath()
    ->rect(1, 1, 4, 4)
    ->fillPreserve()
    ->setSourceRgb(0, 0, 1)
    ->fill()
    ->flushImage();
var_dump(imagecolorat($preserve, 2, 2));

$triangle = imagecreatetruecolor(8, 8);
imagefill($triangle, 0, 0, 0x000000);
$triangleCtx = $triangle->getContext();
$triangleCtx
    ->setSourceRgb(0, 1, 0)
    ->beginPath()
    ->moveTo(1, 1)
    ->lineTo(6, 1)
    ->lineTo(1, 6)
    ->closePath()
    ->fill()
    ->flushImage();
var_dump(imagecolorat($triangle, 2, 2) !== 0);
?>
--EXPECT--
bool(true)
int(0)
int(0)
int(16711680)
int(16711680)
int(16711680)
int(0)
int(255)
bool(true)
