--TEST--
Gd\Context stroke style and fill rule options
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\LineCap::class)) {
    die("skip Gd 2D stroke style API not available.");
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

function changed_pixels(GdImage $first, GdImage $second): int {
    return $first->perceptualDiff($second, 0.03)->pixelsChanged;
}

function draw_line(?Gd\LineCap $cap = null, ?array $dash = null): GdImage {
    $image = make_image();
    $ctx = $image
        ->getContext()
        ->setSourceRgb(1, 0, 0)
        ->setLineWidth(14);

    if ($cap !== null) {
        $ctx->setLineCap($cap);
    }
    if ($dash !== null) {
        $ctx->setDash(0, $dash);
    }

    $ctx
        ->beginPath()
        ->moveTo(18, 48)
        ->lineTo(78, 48)
        ->stroke()
        ->flushImage();

    return $image;
}

$context = make_image()->getContext();
var_dump($context->setLineCap(Gd\LineCap::Round) === $context);
var_dump($context->setLineJoin(Gd\LineJoin::Bevel) === $context);
var_dump($context->setDash(0, [4, 3]) === $context);
var_dump($context->setFillRule(Gd\FillRule::EvenOdd) === $context);

$solid = draw_line(Gd\LineCap::Butt);
$dashed = draw_line(Gd\LineCap::Butt, [8, 8]);
var_dump(changed_pixels($solid, $dashed) > 200);

$butt = draw_line(Gd\LineCap::Butt);
$round = draw_line(Gd\LineCap::Round);
var_dump(changed_pixels($butt, $round) > 80);

$miter = make_image();
$miter
    ->getContext()
    ->setSourceRgb(0, 1, 0)
    ->setLineWidth(18)
    ->setLineJoin(Gd\LineJoin::Miter)
    ->beginPath()
    ->moveTo(22, 74)
    ->lineTo(48, 18)
    ->lineTo(74, 74)
    ->stroke()
    ->flushImage();

$bevel = make_image();
$bevel
    ->getContext()
    ->setSourceRgb(0, 1, 0)
    ->setLineWidth(18)
    ->setLineJoin(Gd\LineJoin::Bevel)
    ->beginPath()
    ->moveTo(22, 74)
    ->lineTo(48, 18)
    ->lineTo(74, 74)
    ->stroke()
    ->flushImage();

var_dump(changed_pixels($miter, $bevel) > 40);

$nonZero = make_image();
$nonZero
    ->getContext()
    ->setSourceRgb(0, 0, 1)
    ->setFillRule(Gd\FillRule::NonZero)
    ->beginPath()
    ->rect(14, 14, 68, 68)
    ->rect(30, 30, 36, 36)
    ->fill()
    ->flushImage();

$evenOdd = make_image();
$evenOdd
    ->getContext()
    ->setSourceRgb(0, 0, 1)
    ->setFillRule(Gd\FillRule::EvenOdd)
    ->beginPath()
    ->rect(14, 14, 68, 68)
    ->rect(30, 30, 36, 36)
    ->fill()
    ->flushImage();

var_dump(changed_pixels($nonZero, $evenOdd) > 1000);
var_dump(imagecolorat($nonZero, 48, 48) !== 0);
var_dump(imagecolorat($evenOdd, 48, 48) === 0);

foreach ([[], [4, 0], [4, -1]] as $pattern) {
    try {
        $context->setDash(0, $pattern);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\Context::setDash(): Argument #2 ($pattern) must not be empty
Gd\Context::setDash(): Argument #2 ($pattern) must contain only positive lengths
Gd\Context::setDash(): Argument #2 ($pattern) must contain only positive lengths
