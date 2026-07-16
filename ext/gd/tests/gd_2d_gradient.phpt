--TEST--
Gd 2D linear and radial gradients
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\LinearGradient::class) || !class_exists(Gd\RadialGradient::class)) {
    die("skip Gd 2D gradient API not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
?>
--FILE--
<?php
function make_image(int $width, int $height, int $color = 0x000000): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagefilledrectangle($image, 0, 0, $width - 1, $height - 1, $color);
    return $image;
}

function rgb_at(GdImage $image, int $x, int $y): array {
    $pixel = imagecolorat($image, $x, $y) & 0xffffff;
    return [
        ($pixel >> 16) & 0xff,
        ($pixel >> 8) & 0xff,
        $pixel & 0xff,
    ];
}

function render_linear(?Gd\Matrix $matrix = null): GdImage {
    $image = make_image(12, 4);
    $gradient = (new Gd\LinearGradient(0, 0, 11, 0))
        ->addColorStopRgb(0, 1, 0, 0)
        ->addColorStopRgb(1, 0, 0, 1);
    if ($matrix !== null) {
        $gradient->setMatrix($matrix);
    }
    $image->getContext()->setSource($gradient)->paint()->flushImage();
    return $image;
}

$linear = new Gd\LinearGradient(0, 0, 11, 0);
var_dump($linear instanceof Gd\Gradient);
var_dump($linear instanceof Gd\Paint);
var_dump($linear->addColorStopRgb(0, 1, 0, 0) === $linear);
var_dump($linear->addColorStopRgba(1, 0, 0, 1, 0.75) === $linear);
var_dump($linear->setExtend(Gd\ExtendMode::Pad) === $linear);

$linearImage = make_image(12, 4);
$linearImage->getContext()->setSource($linear)->paint()->flushImage();
[$leftR, $leftG, $leftB] = rgb_at($linearImage, 0, 1);
[$midR, $midG, $midB] = rgb_at($linearImage, 5, 1);
[$rightR, $rightG, $rightB] = rgb_at($linearImage, 11, 1);
var_dump($leftR > 200 && $leftB < 60);
var_dump($midR > 40 && $midB > 40);
var_dump($rightB > 150 && $rightR < 80);

$repeat = make_image(8, 1);
$repeatGradient = (new Gd\LinearGradient(0, 0, 4, 0))
    ->setExtend(Gd\ExtendMode::Repeat)
    ->addColorStopRgb(0, 1, 0, 0)
    ->addColorStopRgb(1, 0, 0, 1);
$repeat->getContext()->setSource($repeatGradient)->paint()->flushImage();
var_dump(rgb_at($repeat, 0, 0) === rgb_at($repeat, 4, 0));
var_dump(rgb_at($repeat, 1, 0) === rgb_at($repeat, 5, 0));

$normal = render_linear();
$translated = render_linear(Gd\Matrix::translation(4, 0));
$diff = $normal->perceptualDiff($translated, 0.03);
var_dump($diff->pixelsChanged > 20);
var_dump($diff->maximumDelta > 0.1);

$radial = make_image(9, 9);
$radialGradient = (new Gd\RadialGradient(4, 4, 0, 4, 4, 4))
    ->addColorStopRgb(0, 1, 1, 1)
    ->addColorStopRgb(1, 0, 0, 0);
$radial->getContext()->setSource($radialGradient)->paint()->flushImage();
[$centerR, $centerG, $centerB] = rgb_at($radial, 4, 4);
[$cornerR, $cornerG, $cornerB] = rgb_at($radial, 0, 0);
var_dump($centerR > 180 && $centerG > 180 && $centerB > 180);
var_dump($cornerR < 20 && $cornerG < 20 && $cornerB < 20);

$alpha = make_image(8, 2, 0x00ff00);
$alphaGradient = (new Gd\LinearGradient(0, 0, 7, 0))
    ->addColorStopRgba(0, 1, 0, 0, 0.5)
    ->addColorStopRgba(1, 0, 0, 1, 0.5);
$alpha->getContext()->setSource($alphaGradient)->paint()->flushImage();
[$alphaR, $alphaG, $alphaB] = rgb_at($alpha, 0, 0);
var_dump($alphaR > 50 && $alphaG > 50 && $alphaB < 40);

foreach ([
    fn() => new Gd\LinearGradient(INF, 0, 1, 0),
    fn() => new Gd\RadialGradient(0, 0, -1, 1, 1, 2),
    fn() => (new Gd\LinearGradient(0, 0, 1, 0))->addColorStopRgb(-0.1, 1, 0, 0),
    fn() => (new Gd\LinearGradient(0, 0, 1, 0))->addColorStopRgba(0, 1, 0, 0, NAN),
    fn() => (new Gd\LinearGradient(0, 0, 1, 0))->setMatrix(Gd\Matrix::scaling(0, 1)),
] as $callback) {
    try {
        $callback();
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

try {
    new Gd\Gradient();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Gd\LinearGradient::__construct(): Argument #1 ($x0) must be finite
Gd\RadialGradient::__construct(): Argument #3 ($r0) must be greater than or equal to 0
Gd\Gradient::addColorStopRgb(): Argument #1 ($offset) must be between 0.0 and 1.0
Gd\Gradient::addColorStopRgba(): Argument #5 ($a) must be between 0.0 and 1.0
Gd\Gradient::setMatrix(): Argument #1 ($matrix) must be invertible
Cannot instantiate abstract class Gd\Gradient
