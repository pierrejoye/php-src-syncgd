--TEST--
Gd\Path reusable path objects
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Path::class) || !class_exists(Gd\Context::class)) {
    die("skip Gd 2D path API not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
?>
--FILE--
<?php
const SIZE = 96;

function make_image(): GdImage {
    $image = imagecreatetruecolor(SIZE, SIZE);
    imagefilledrectangle($image, 0, 0, SIZE - 1, SIZE - 1, 0x000000);
    return $image;
}

function assert_similar(GdImage $first, GdImage $second): void {
    $diff = $first->perceptualDiff($second, 0.03);
    var_dump($diff->pixelsChanged <= (int) (SIZE * SIZE * 0.01));
    var_dump($diff->maximumDelta < 0.08);
}

function color_at(GdImage $image, int $x, int $y): int {
    return imagecolorat($image, $x, $y) & 0xffffff;
}

$path = new Gd\Path();
var_dump($path instanceof Gd\Path);
var_dump($path
    ->moveTo(16, 68)
    ->curveTo(26, 12, 70, 12, 80, 68)
    ->relMoveTo(-64, 0)
    ->relLineTo(32, -40)
    ->relLineTo(32, 40)
    ->close()
    === $path);

$fromPath = make_image();
$fromPath
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(2)
    ->appendPath($path)
    ->stroke()
    ->flushImage();

$direct = make_image();
$direct
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(2)
    ->beginPath()
    ->moveTo(16, 68)
    ->curveTo(26, 12, 70, 12, 80, 68)
    ->relMoveTo(-64, 0)
    ->relLineTo(32, -40)
    ->relLineTo(32, 40)
    ->closePath()
    ->stroke()
    ->flushImage();

assert_similar($fromPath, $direct);

$rect = new Gd\Path();
$rect->rect(10, 10, 20, 20);
$ctxAppended = make_image();
$ctx = $ctxAppended
    ->getContext()
    ->setSourceRgb(0, 1, 0)
    ->appendPath($rect);
$rect->transform(Gd\Matrix::translation(40, 40));
$ctx->fill()->flushImage();
var_dump(color_at($ctxAppended, 15, 15));
var_dump(color_at($ctxAppended, 55, 55));

$transformedPath = new Gd\Path();
$transformedPath
    ->rect(20, 20, 24, 24)
    ->transform(Gd\Matrix::translation(16, 8));

$renderTransformedPath = make_image();
$renderTransformedPath
    ->getContext()
    ->setSourceRgb(0, 0, 1)
    ->appendPath($transformedPath)
    ->fill()
    ->flushImage();

$renderContextTransform = make_image();
$renderContextTransform
    ->getContext()
    ->setSourceRgb(0, 0, 1)
    ->transform(Gd\Matrix::translation(16, 8))
    ->beginPath()
    ->rect(20, 20, 24, 24)
    ->fill()
    ->flushImage();

assert_similar($renderTransformedPath, $renderContextTransform);

$complex = new Gd\Path();
$complex
    ->moveTo(12, 72)
    ->quadTo(48, 12, 84, 72)
    ->moveTo(16, 48)
    ->arc(48, 48, 20, 0, M_PI)
    ->negativeArc(48, 48, 12, M_PI, 0);

$image = make_image();
$image->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(3)
    ->appendPath($complex)
    ->stroke()
    ->flushImage();
var_dump(make_image()->perceptualDiff($image, 0.03)->pixelsChanged > 100);

$holder = imagecreatetruecolor(1, 1);
$holderCtx = $holder->getContext();
$holderCtx->destroy(false);
unset($holder, $holderCtx);
gc_collect_cycles();

$afterContextGone = make_image();
$afterContextGone
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->appendPath($complex)
    ->stroke()
    ->flushImage();

var_dump($afterContextGone instanceof GdImage);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
int(65280)
int(0)
bool(true)
bool(true)
bool(true)
bool(true)
