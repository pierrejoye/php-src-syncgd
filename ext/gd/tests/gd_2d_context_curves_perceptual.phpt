--TEST--
Gd\Context curves and arcs render consistently through transforms
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\Matrix::class)) {
    die("skip Gd 2D curve API not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
?>
--FILE--
<?php
const WIDTH = 96;
const HEIGHT = 96;

function make_image(): GdImage {
    $image = imagecreatetruecolor(WIDTH, HEIGHT);
    imagefilledrectangle($image, 0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
    return $image;
}

function point(Gd\Matrix $matrix, float $x, float $y): array {
    return $matrix->mapPoint($x, $y);
}

function assert_similar(GdImage $first, GdImage $second): void {
    $diff = $first->perceptualDiff($second, 0.03);
    var_dump($diff->pixelsChanged <= (int) (WIDTH * HEIGHT * 0.02));
    var_dump($diff->maximumDelta < 0.12);
}

$matrix = Gd\Matrix::rotationAround(M_PI / 5, 48, 48);

$transformed = make_image();
$transformed
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(2)
    ->transform($matrix)
    ->beginPath()
    ->moveTo(16, 68)
    ->curveTo(26, 12, 70, 12, 80, 68)
    ->stroke()
    ->flushImage();

[$x0, $y0] = point($matrix, 16, 68);
[$x1, $y1] = point($matrix, 26, 12);
[$x2, $y2] = point($matrix, 70, 12);
[$x3, $y3] = point($matrix, 80, 68);

$manual = make_image();
$manual
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(2)
    ->beginPath()
    ->moveTo($x0, $y0)
    ->curveTo($x1, $y1, $x2, $y2, $x3, $y3)
    ->stroke()
    ->flushImage();

assert_similar($transformed, $manual);

$quadAbsolute = make_image();
$quadAbsolute
    ->getContext()
    ->setSourceRgb(0, 1, 0)
    ->setLineWidth(2)
    ->beginPath()
    ->moveTo(20, 70)
    ->quadTo(48, 18, 76, 70)
    ->stroke()
    ->flushImage();

$quadRelative = make_image();
$quadRelative
    ->getContext()
    ->setSourceRgb(0, 1, 0)
    ->setLineWidth(2)
    ->beginPath()
    ->moveTo(20, 70)
    ->relQuadTo(28, -52, 56, 0)
    ->stroke()
    ->flushImage();

assert_similar($quadAbsolute, $quadRelative);

$arcRotated = make_image();
$arcRotated
    ->getContext()
    ->setSourceRgb(0, 0, 1)
    ->setLineWidth(2)
    ->transform(Gd\Matrix::rotationAround(M_PI / 4, 48, 48))
    ->beginPath()
    ->arc(48, 48, 24, 0, M_PI)
    ->stroke()
    ->flushImage();

$arcManual = make_image();
$arcManual
    ->getContext()
    ->setSourceRgb(0, 0, 1)
    ->setLineWidth(2)
    ->beginPath()
    ->arc(48, 48, 24, M_PI / 4, M_PI + M_PI / 4)
    ->stroke()
    ->flushImage();

assert_similar($arcRotated, $arcManual);

$relativeCubic = make_image();
$relativeCubic
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(2)
    ->beginPath()
    ->relMoveTo(16, 68)
    ->relCurveTo(10, -56, 54, -56, 64, 0)
    ->stroke()
    ->flushImage();

$absoluteCubic = make_image();
$absoluteCubic
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(2)
    ->beginPath()
    ->moveTo(16, 68)
    ->curveTo(26, 12, 70, 12, 80, 68)
    ->stroke()
    ->flushImage();

assert_similar($relativeCubic, $absoluteCubic);
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
