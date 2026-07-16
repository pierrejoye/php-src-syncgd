--TEST--
Gd\Context stroke path operations
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
function rgb(int $pixel): array {
    return [($pixel >> 16) & 0xff, ($pixel >> 8) & 0xff, $pixel & 0xff];
}

$image = imagecreatetruecolor(8, 8);
imagefill($image, 0, 0, 0x000000);

$ctx = $image->getContext();
$result = $ctx
    ->setSourceRgb(0, 1, 0)
    ->setLineWidth(1)
    ->beginPath()
    ->moveTo(1, 1)
    ->lineTo(6, 1)
    ->stroke();

var_dump($result === $ctx);
var_dump(imagecolorat($image, 3, 1));
$ctx->flushImage();
[$r, $g, $b] = rgb(imagecolorat($image, 3, 1));
var_dump($r === 0);
var_dump($g > 0);
var_dump($b === 0);

$preserve = imagecreatetruecolor(8, 8);
imagefill($preserve, 0, 0, 0x000000);
$preserveCtx = $preserve->getContext();
$preserveCtx
    ->setSourceRgb(0, 1, 0)
    ->setLineWidth(1)
    ->beginPath()
    ->moveTo(1, 1)
    ->lineTo(6, 1)
    ->strokePreserve()
    ->setSourceRgb(1, 0, 0)
    ->stroke()
    ->flushImage();
[$r, $g, $b] = rgb(imagecolorat($preserve, 3, 1));
var_dump($r > 0);
var_dump($r > $g);
var_dump($b === 0);
?>
--EXPECT--
bool(true)
int(0)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
