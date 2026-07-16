--TEST--
Gd\Context compositing operators match upstream gallery
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
if (!function_exists("imagecreatefrompng")) {
    die("skip PNG support not available.");
}
if (!is_file(__DIR__ . "/gd_2d_composite_operators.png")) {
    die("skip compositing reference image not available.");
}
?>
--FILE--
<?php
const COLUMNS = 5;
const TILE_WIDTH = 160;
const TILE_HEIGHT = 120;
const LABEL_HEIGHT = 18;
const GAP = 8;

$operatorNames = [
    "CLEAR", "SOURCE", "OVER", "IN", "OUT", "ATOP",
    "DEST", "DEST_OVER", "DEST_IN", "DEST_OUT", "DEST_ATOP", "XOR",
    "ADD", "SATURATE", "MULTIPLY", "SCREEN", "OVERLAY", "DARKEN",
    "LIGHTEN", "COLOR_DODGE", "COLOR_BURN", "HARD_LIGHT", "SOFT_LIGHT", "DIFFERENCE",
    "EXCLUSION", "HSL_HUE", "HSL_SATURATION", "HSL_COLOR", "HSL_LUMINOSITY",
];

$operators = [
    Gd\CompositeOperator::Clear,
    Gd\CompositeOperator::Source,
    Gd\CompositeOperator::Over,
    Gd\CompositeOperator::In,
    Gd\CompositeOperator::Out,
    Gd\CompositeOperator::Atop,
    Gd\CompositeOperator::Dest,
    Gd\CompositeOperator::DestOver,
    Gd\CompositeOperator::DestIn,
    Gd\CompositeOperator::DestOut,
    Gd\CompositeOperator::DestAtop,
    Gd\CompositeOperator::Xor,
    Gd\CompositeOperator::Add,
    Gd\CompositeOperator::Saturate,
    Gd\CompositeOperator::Multiply,
    Gd\CompositeOperator::Screen,
    Gd\CompositeOperator::Overlay,
    Gd\CompositeOperator::Darken,
    Gd\CompositeOperator::Lighten,
    Gd\CompositeOperator::ColorDodge,
    Gd\CompositeOperator::ColorBurn,
    Gd\CompositeOperator::HardLight,
    Gd\CompositeOperator::SoftLight,
    Gd\CompositeOperator::Difference,
    Gd\CompositeOperator::Exclusion,
    Gd\CompositeOperator::HslHue,
    Gd\CompositeOperator::HslSaturation,
    Gd\CompositeOperator::HslColor,
    Gd\CompositeOperator::HslLuminosity,
];

/* Tiny 5x7 capitals keep this test independent of font libraries. */
$font = [
    [14, 17, 17, 31, 17, 17, 17], [30, 17, 17, 30, 17, 17, 30], [14, 17, 16, 16, 16, 17, 14],
    [30, 17, 17, 17, 17, 17, 30], [31, 16, 16, 30, 16, 16, 31], [31, 16, 16, 30, 16, 16, 16],
    [14, 17, 16, 23, 17, 17, 15], [17, 17, 17, 31, 17, 17, 17], [14, 4, 4, 4, 4, 4, 14],
    [7, 2, 2, 2, 18, 18, 12],     [17, 18, 20, 24, 20, 18, 17], [16, 16, 16, 16, 16, 16, 31],
    [17, 27, 21, 21, 17, 17, 17], [17, 25, 21, 19, 17, 17, 17], [14, 17, 17, 17, 17, 17, 14],
    [30, 17, 17, 30, 16, 16, 16], [14, 17, 17, 17, 21, 18, 13], [30, 17, 17, 30, 20, 18, 17],
    [15, 16, 16, 14, 1, 1, 30],   [31, 4, 4, 4, 4, 4, 4],       [17, 17, 17, 17, 17, 17, 14],
    [17, 17, 17, 17, 17, 10, 4],  [17, 17, 17, 21, 21, 21, 10], [17, 17, 10, 4, 10, 17, 17],
    [17, 17, 10, 4, 4, 4, 4],     [31, 1, 2, 4, 8, 16, 31],
];

function label(Gd\Context $ctx, array $font, string $text, float $x, float $y, float $scale): void {
    $length = strlen($text);
    for ($i = 0; $i < $length; $i++, $x += 6 * $scale) {
        $ord = ord($text[$i]);
        if ($ord < 65 || $ord > 90) {
            continue;
        }
        $glyph = $font[$ord - 65];
        for ($row = 0; $row < 7; $row++) {
            for ($col = 0; $col < 5; $col++) {
                if ($glyph[$row] & (16 >> $col)) {
                    $ctx->rect($x + $col * $scale, $y + $row * $scale, $scale, $scale);
                    $ctx->fill();
                }
            }
        }
    }
}

function render_example(Gd\Context $ctx, Gd\CompositeOperator $operator, float $x, float $y): void {
    /* Composite on genuine transparency: destination (red), then source (blue). */
    $ctx
        ->setOperator(Gd\CompositeOperator::Over)
        ->setSourceRgba(0.7, 0, 0, 0.8)
        ->rect($x, $y, 120, 90)
        ->fill();

    $ctx
        ->setOperator($operator)
        ->setSourceRgba(0, 0, 0.9, 0.4)
        ->rect($x + 40, $y + 30, 120, 90)
        ->fill();
}

$rows = intdiv(count($operators) + COLUMNS - 1, COLUMNS);
$width = COLUMNS * TILE_WIDTH + (COLUMNS + 1) * GAP;
$height = $rows * (LABEL_HEIGHT + TILE_HEIGHT) + ($rows + 1) * GAP;

$gallery = imagecreatetruecolor($width, $height);
imagealphablending($gallery, false);
imagesavealpha($gallery, true);
imagefill($gallery, 0, 0, 0x7fffffff);
imagealphablending($gallery, true);

foreach ($operators as $op => $operator) {
    $x = GAP + ($op % COLUMNS) * (TILE_WIDTH + GAP);
    $y = GAP + intdiv($op, COLUMNS) * (LABEL_HEIGHT + TILE_HEIGHT + GAP);
    $tileContext = $gallery->getContext();
    $tileContext
        ->rect($x, $y + LABEL_HEIGHT, TILE_WIDTH, TILE_HEIGHT)
        ->clip();
    render_example($tileContext, $operator, $x, $y + LABEL_HEIGHT);
    $tileContext->flushImage();
    $tileContext->destroy(false);
}

$labels = $gallery->getContext();
$labels->setSourceRgb(0.05, 0.05, 0.08);
foreach ($operatorNames as $op => $name) {
    $x = GAP + ($op % COLUMNS) * (TILE_WIDTH + GAP);
    $y = GAP + intdiv($op, COLUMNS) * (LABEL_HEIGHT + TILE_HEIGHT + GAP);
    label($labels, $font, $name, $x, $y, 1.5);
}
$labels->flushImage();
$labels->destroy(false);

$reference = imagecreatefrompng(__DIR__ . "/gd_2d_composite_operators.png");
var_dump(imagesx($gallery));
var_dump(imagesy($gallery));
var_dump(imagesx($reference));
var_dump(imagesy($reference));

$diff = $gallery->perceptualDiff($reference, 0.03);
var_dump($diff->pixelsChanged <= 16);
var_dump($diff->maximumDelta < 0.05);

foreach ($operators as $op => $operator) {
    printf("%-14s %s\n", $operatorNames[$op], $operator->name);
}
?>
--EXPECT--
int(848)
int(884)
int(848)
int(884)
bool(true)
bool(true)
CLEAR          Clear
SOURCE         Source
OVER           Over
IN             In
OUT            Out
ATOP           Atop
DEST           Dest
DEST_OVER      DestOver
DEST_IN        DestIn
DEST_OUT       DestOut
DEST_ATOP      DestAtop
XOR            Xor
ADD            Add
SATURATE       Saturate
MULTIPLY       Multiply
SCREEN         Screen
OVERLAY        Overlay
DARKEN         Darken
LIGHTEN        Lighten
COLOR_DODGE    ColorDodge
COLOR_BURN     ColorBurn
HARD_LIGHT     HardLight
SOFT_LIGHT     SoftLight
DIFFERENCE     Difference
EXCLUSION      Exclusion
HSL_HUE        HslHue
HSL_SATURATION HslSaturation
HSL_COLOR      HslColor
HSL_LUMINOSITY HslLuminosity
