--TEST--
GdImage::composeFrom()
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\CompositeOperator::class)) {
    die("skip Gd\\CompositeOperator not available.");
}
if (!method_exists(GdImage::class, "composeFrom")) {
    die("skip GdImage::composeFrom() not available.");
}
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
function image_of(int $width, int $height, int $color): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, $color);
    return $image;
}

function clone_image(GdImage $image): GdImage {
    $copy = imagecreatetruecolor(imagesx($image), imagesy($image));
    imagealphablending($copy, false);
    imagesavealpha($copy, true);
    imagecopy($copy, $image, 0, 0, 0, 0, imagesx($image), imagesy($image));
    return $copy;
}

function assert_pixel(string $label, GdImage $image, int $x, int $y, int $expected): void {
    $actual = imagecolorat($image, $x, $y);
    if ($actual !== $expected) {
        printf("%s pixel %d,%d differs: %08X != %08X\n", $label, $x, $y, $actual, $expected);
        return;
    }
    echo "$label ok\n";
}

function assert_same_image(string $label, GdImage $actual, GdImage $expected): void {
    if (imagesx($actual) !== imagesx($expected) || imagesy($actual) !== imagesy($expected)) {
        printf("%s dimensions differ\n", $label);
        return;
    }

    $diff = $actual->perceptualDiff($expected, 0.03);
    if ($diff->pixelsChanged > 0 || $diff->maximumDelta > 0.03) {
        printf("%s differs: pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
        return;
    }

    echo "$label ok\n";
}

function context_expected(GdImage $dest, GdImage $source, int $x, int $y, Gd\CompositeOperator $operator, float $opacity = 1.0): GdImage {
    $expected = clone_image($dest);
    $expected
        ->getContext()
        ->setSourceImage($source, $x, $y)
        ->setOperator($operator)
        ->setOpacity($opacity)
        ->paint()
        ->flushImage();
    return $expected;
}

$dest = image_of(3, 3, 0x000000ff);
$source = image_of(2, 2, 0x7f000000);
imagesetpixel($source, 0, 0, 0x40ff0000);
imagesetpixel($source, 1, 0, 0x0000ff00);
$sourceBefore = clone_image($source);
$expected = context_expected($dest, $source, 1, 1, Gd\CompositeOperator::Over);
$return = $dest->composeFrom($source, 1, 1);
var_dump($return === $dest);
assert_same_image('default over', $dest, $expected);
assert_same_image('source unchanged', $source, $sourceBefore);

$sourceOp = image_of(1, 1, 0x0000ff00);
$sourceDest = image_of(1, 1, 0x00ff0000);
$sourceDest->composeFrom($sourceOp, operator: Gd\CompositeOperator::Source);
assert_pixel('source operator', $sourceDest, 0, 0, 0x0000ff00);

$multiplyDest = image_of(1, 1, 0x00808080);
$multiplySource = image_of(1, 1, 0x00800000);
$multiplyExpected = context_expected($multiplyDest, $multiplySource, 0, 0, Gd\CompositeOperator::Multiply);
$multiplyDest->composeFrom($multiplySource, operator: Gd\CompositeOperator::Multiply);
assert_same_image('multiply operator', $multiplyDest, $multiplyExpected);

$destOp = image_of(1, 1, 0x00112233);
$destOp->composeFrom(image_of(1, 1, 0x00ffffff), operator: Gd\CompositeOperator::Dest);
assert_pixel('dest operator', $destOp, 0, 0, 0x00112233);

$destOut = image_of(1, 1, 0x00ff0000);
$destOut->composeFrom(image_of(1, 1, 0x00000000), operator: Gd\CompositeOperator::DestOut);
assert_pixel('dest out operator', $destOut, 0, 0, 0x7f000000);

$opacityDest = image_of(1, 1, 0x000000ff);
$opacitySource = image_of(1, 1, 0x00ff0000);
$opacityDest->composeFrom($opacitySource, operator: Gd\CompositeOperator::Over, opacity: 0.5);
assert_pixel('opacity', $opacityDest, 0, 0, 0x00800080);

$alphaDest = image_of(1, 1, 0x3f0000ff);
$alphaSource = image_of(1, 1, 0x40ff0000);
$alphaExpected = context_expected($alphaDest, $alphaSource, 0, 0, Gd\CompositeOperator::Over);
$alphaDest->composeFrom($alphaSource);
assert_same_image('over weights source and destination alpha', $alphaDest, $alphaExpected);

$opacityAlphaDest = image_of(1, 1, 0x3f0000ff);
$opacityAlphaExpected = context_expected($opacityAlphaDest, $alphaSource, 0, 0, Gd\CompositeOperator::Over, 0.5);
$opacityAlphaDest->composeFrom($alphaSource, operator: Gd\CompositeOperator::Over, opacity: 0.5);
assert_same_image('opacity weights source alpha', $opacityAlphaDest, $opacityAlphaExpected);

$multiplyAlphaDest = image_of(1, 1, 0x3f4080ff);
$multiplyAlphaSource = image_of(1, 1, 0x4080ff40);
$multiplyAlphaExpected = context_expected($multiplyAlphaDest, $multiplyAlphaSource, 0, 0, Gd\CompositeOperator::Multiply);
$multiplyAlphaDest->composeFrom($multiplyAlphaSource, operator: Gd\CompositeOperator::Multiply);
assert_same_image('multiply weights source and destination alpha', $multiplyAlphaDest, $multiplyAlphaExpected);

$sheet = image_of(2, 1, 0x0000ff00);
imagesetpixel($sheet, 1, 0, 0x00ff0000);
$cropped = image_of(1, 1, 0x00000000);
$cropped->composeFrom($sheet, sourceRegion: new Gd\Rect(1, 0, 1, 1));
assert_pixel('source region', $cropped, 0, 0, 0x00ff0000);

$clipped = image_of(3, 1, 0x000000ff);
$clipped->composeFrom(image_of(3, 1, 0x00ff0000), clip: new Gd\Rect(1, 0, 1, 1));
assert_pixel('clip left unchanged', $clipped, 0, 0, 0x000000ff);
assert_pixel('clip middle changed', $clipped, 1, 0, 0x00ff0000);
assert_pixel('clip right unchanged', $clipped, 2, 0, 0x000000ff);

$unbounded = image_of(3, 1, 0x00ff0000);
$mask = image_of(1, 1, 0x00000000);
$unbounded->composeFrom($mask, 1, 0, Gd\CompositeOperator::DestIn, clip: new Gd\Rect(0, 0, 3, 1));
assert_pixel('unbounded left cleared', $unbounded, 0, 0, 0x7f000000);
assert_pixel('unbounded middle kept', $unbounded, 1, 0, 0x00ff0000);
assert_pixel('unbounded right cleared', $unbounded, 2, 0, 0x7f000000);

$offCanvas = image_of(1, 1, 0x000000ff);
$wideSource = image_of(2, 1, 0x0000ff00);
imagesetpixel($wideSource, 1, 0, 0x00ff0000);
$offCanvas->composeFrom($wideSource, -1, 0, Gd\CompositeOperator::Source);
assert_pixel('off canvas', $offCanvas, 0, 0, 0x00ff0000);

$paletteSource = imagecreate(1, 1);
$paletteRed = imagecolorallocate($paletteSource, 255, 0, 0);
imagesetpixel($paletteSource, 0, 0, $paletteRed);
$paletteDest = image_of(1, 1, 0x00000000);
$paletteDest->composeFrom($paletteSource, operator: Gd\CompositeOperator::Source);
assert_pixel('palette source', $paletteDest, 0, 0, 0x00ff0000);

foreach ([
    fn() => imagecreate(1, 1)->composeFrom(image_of(1, 1, 0x00ffffff)),
    fn() => image_of(1, 1, 0x00000000)->composeFrom(image_of(1, 1, 0x00ffffff), opacity: -0.1),
    fn() => image_of(1, 1, 0x00000000)->composeFrom(image_of(1, 1, 0x00ffffff), opacity: 1.1),
    fn() => image_of(1, 1, 0x00000000)->composeFrom(image_of(1, 1, 0x00ffffff), opacity: NAN),
    fn() => image_of(1, 1, 0x00000000)->composeFrom(image_of(1, 1, 0x00ffffff), sourceRegion: new Gd\Rect(0, 0, 0.4, 1)),
    fn() => image_of(1, 1, 0x00000000)->composeFrom(image_of(1, 1, 0x00ffffff), clip: new Gd\Rect(0, 0, 1, NAN)),
] as $callback) {
    try {
        $callback();
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

$method = new ReflectionMethod(GdImage::class, 'composeFrom');
var_dump($method->getReturnType()->getName());
foreach ($method->getParameters() as $parameter) {
    printf("%s:%s:%s\n", $parameter->getName(), $parameter->getType(), $parameter->isDefaultValueAvailable() ? var_export($parameter->getDefaultValue(), true) : 'none');
}
?>
--EXPECTF--
bool(true)
default over ok
source unchanged ok
source operator ok
multiply operator ok
dest operator ok
dest out operator ok
opacity ok
over weights source and destination alpha ok
opacity weights source alpha ok
multiply weights source and destination alpha ok
source region ok
clip left unchanged ok
clip middle changed ok
clip right unchanged ok
unbounded left cleared ok
unbounded middle kept ok
unbounded right cleared ok
off canvas ok
palette source ok
GdImage::composeFrom(): receiver image must be truecolor
GdImage::composeFrom(): Argument #5 ($opacity) must be between 0.0 and 1.0
GdImage::composeFrom(): Argument #5 ($opacity) must be between 0.0 and 1.0
GdImage::composeFrom(): Argument #5 ($opacity) must be finite
Gd\Rect::$width must round to a value greater than 0
Gd\Rect::$height must be finite
string(7) "GdImage"
source:GdImage:none
x:int:0
y:int:0
operator:Gd\CompositeOperator:%S
opacity:float:1.0
sourceRegion:?Gd\Rect:NULL
clip:?Gd\Rect:NULL
