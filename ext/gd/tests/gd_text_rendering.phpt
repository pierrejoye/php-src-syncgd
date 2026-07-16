--TEST--
GD text rendering, paths, and extents
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\FontFace::class)) {
    die("skip GD text API not available");
}
?>
--FILE--
<?php
function non_white_pixels(GdImage $image): int {
    $count = 0;
    for ($y = 0; $y < imagesy($image); $y++) {
        for ($x = 0; $x < imagesx($image); $x++) {
            if ((imagecolorat($image, $x, $y) & 0xffffff) !== 0xffffff) {
                $count++;
            }
        }
    }
    return $count;
}

$font = Gd\FontFace::fromFile(__DIR__ . '/Tuffy.ttf');

$show = imagecreatetruecolor(140, 70);
imagefill($show, 0, 0, 0xffffff);
$showCtx = $show->getContext();
$showCtx
    ->setSourceRgb(0, 0, 0)
    ->setFontFace($font)
    ->setFontSize(24);

$extents = $showCtx->textExtents('Hi');
var_dump($extents instanceof Gd\TextExtents);
var_dump($extents->width > 0, $extents->height > 0, $extents->xAdvance > 0);

$empty = $showCtx->textExtents('');
var_dump($empty->width, $empty->height, $empty->xAdvance);

$showCtx->showText('Hi', 10, 45)->flushImage();
var_dump(non_white_pixels($show) > 0);

$path = imagecreatetruecolor(140, 70);
imagefill($path, 0, 0, 0xffffff);
$path->getContext()
    ->setSourceRgb(0, 0, 0)
    ->setFontFace($font)
    ->setFontSize(24)
    ->textPath('Hi', 10, 45)
    ->fill()
    ->flushImage();
var_dump(non_white_pixels($path) > 0);

?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
float(0)
float(0)
float(0)
bool(true)
bool(true)
