--TEST--
Gd\Context compositing operators and opacity
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\CompositeOperator::class)) {
    die("skip Gd 2D compositing API not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
?>
--FILE--
<?php
function make_image(int $color, int $size = 8): GdImage {
    $image = imagecreatetruecolor($size, $size);
    imagefill($image, 0, 0, $color);
    return $image;
}

function color_at(GdImage $image, int $x = 3, int $y = 3): int {
    return imagecolorat($image, $x, $y) & 0xffffff;
}

function rgb(int $color): array {
    return [
        ($color >> 16) & 0xff,
        ($color >> 8) & 0xff,
        $color & 0xff,
    ];
}

$cases = array_map(fn($case) => $case->name, Gd\CompositeOperator::cases());
var_dump($cases);

$source = make_image(0x00ff00);
$sourceCtx = $source->getContext();
var_dump($sourceCtx->setOperator(Gd\CompositeOperator::Source) === $sourceCtx);
var_dump($sourceCtx->setOpacity(1) === $sourceCtx);
$sourceCtx
    ->setSourceRgb(1, 0, 0)
    ->beginPath()
    ->rect(2, 2, 3, 3)
    ->fill()
    ->flushImage();
var_dump(color_at($source));
var_dump(color_at($source, 0, 0));

$over = make_image(0x00ff00);
$over
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setOpacity(0.5)
    ->beginPath()
    ->rect(2, 2, 3, 3)
    ->fill()
    ->flushImage();
[$r, $g, $b] = rgb(color_at($over));
var_dump($r > 80 && $r < 180);
var_dump($g > 80 && $g < 180);
var_dump($b < 10);

$clampedLow = make_image(0x000000);
$clampedLow
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setOpacity(-1)
    ->paint()
    ->flushImage();
var_dump(color_at($clampedLow));

$clampedHigh = make_image(0x000000);
$clampedHigh
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setOpacity(2)
    ->paint()
    ->flushImage();
var_dump(color_at($clampedHigh));

$clear = make_image(0x0000ff);
$clear
    ->getContext()
    ->setOperator(Gd\CompositeOperator::Clear)
    ->beginPath()
    ->rect(2, 2, 3, 3)
    ->fill()
    ->flushImage();
var_dump(color_at($clear));
var_dump(color_at($clear, 0, 0));

$multiply = make_image(0x00ff00);
$multiply
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setOperator(Gd\CompositeOperator::Multiply)
    ->paint()
    ->flushImage();
var_dump(color_at($multiply));
$diff = $multiply->perceptualDiff($over, 0.03);
var_dump($diff->pixelsChanged > 20);
var_dump($diff->maximumDelta > 0.1);

$restored = make_image(0x00ff00);
$restored
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->setOperator(Gd\CompositeOperator::Source)
    ->setOpacity(1)
    ->save()
    ->setOperator(Gd\CompositeOperator::Clear)
    ->setOpacity(0)
    ->restore()
    ->paint()
    ->flushImage();
var_dump(color_at($restored));

foreach ([INF, -INF, NAN] as $opacity) {
    try {
        make_image(0x000000)->getContext()->setOpacity($opacity);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

$destroyed = make_image(0x000000)->getContext();
$destroyed->destroy(false);
foreach (["setOperator" => [Gd\CompositeOperator::Over], "setOpacity" => [1]] as $method => $args) {
    try {
        $destroyed->$method(...$args);
    } catch (Error $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
array(29) {
  [0]=>
  string(5) "Clear"
  [1]=>
  string(6) "Source"
  [2]=>
  string(4) "Over"
  [3]=>
  string(2) "In"
  [4]=>
  string(3) "Out"
  [5]=>
  string(4) "Atop"
  [6]=>
  string(4) "Dest"
  [7]=>
  string(8) "DestOver"
  [8]=>
  string(6) "DestIn"
  [9]=>
  string(7) "DestOut"
  [10]=>
  string(8) "DestAtop"
  [11]=>
  string(3) "Xor"
  [12]=>
  string(3) "Add"
  [13]=>
  string(8) "Saturate"
  [14]=>
  string(8) "Multiply"
  [15]=>
  string(6) "Screen"
  [16]=>
  string(7) "Overlay"
  [17]=>
  string(6) "Darken"
  [18]=>
  string(7) "Lighten"
  [19]=>
  string(10) "ColorDodge"
  [20]=>
  string(9) "ColorBurn"
  [21]=>
  string(9) "HardLight"
  [22]=>
  string(9) "SoftLight"
  [23]=>
  string(10) "Difference"
  [24]=>
  string(9) "Exclusion"
  [25]=>
  string(6) "HslHue"
  [26]=>
  string(13) "HslSaturation"
  [27]=>
  string(8) "HslColor"
  [28]=>
  string(13) "HslLuminosity"
}
bool(true)
bool(true)
int(16711680)
int(65280)
bool(true)
bool(true)
bool(true)
int(0)
int(16711680)
int(0)
int(255)
int(0)
bool(true)
bool(true)
int(16711680)
Gd\Context::setOpacity(): Argument #1 ($opacity) must be finite
Gd\Context::setOpacity(): Argument #1 ($opacity) must be finite
Gd\Context::setOpacity(): Argument #1 ($opacity) must be finite
Gd\Context has already been destroyed
Gd\Context has already been destroyed
