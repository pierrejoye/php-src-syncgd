--TEST--
Gd gradients with compositing operators match upstream gallery
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\LinearGradient::class) || !class_exists(Gd\RadialGradient::class) || !class_exists(Gd\CompositeOperator::class)) {
    die("skip Gd 2D gradient/compositing API not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
if (!function_exists("imagecreatefrompng")) {
    die("skip PNG support not available.");
}
if (!is_file(__DIR__ . "/gd_2d_gradient_operators.png")) {
    die("skip gradient operators reference image not available.");
}
?>
--FILE--
<?php
const COLS = 5;
const TILE = 150;
const GAP = 8;
const GD_2D_PI = 3.14159265358979323846;

$names = [
    "CLEAR", "SOURCE", "OVER", "IN", "OUT", "ATOP",
    "DEST", "DEST_OVER", "DEST_IN", "DEST_OUT", "DEST_ATOP", "XOR",
    "ADD", "SATURATE", "MULTIPLY", "SCREEN", "OVERLAY", "DARKEN",
    "LIGHTEN", "COLOR_DODGE", "COLOR_BURN", "HARD_LIGHT", "SOFT_LIGHT", "DIFFERENCE",
    "EXCLUSION", "HSL_HUE", "HSL_SATURATION", "HSL_COLOR", "HSL_LUMINOSITY",
];

$shortNames = [
    "CLEAR", "SOURCE", "OVER", "IN", "OUT", "ATOP",
    "DEST", "DOVER", "DIN", "DOUT", "DATOP", "XOR",
    "ADD", "SATURATE", "MULTIPLY", "SCREEN", "OVERLAY", "DARKEN",
    "LIGHTEN", "DODGE", "BURN", "HARDLIGHT", "SOFTLIGHT", "DIFFERENCE",
    "EXCLUSION", "HUE", "SATURATION", "COLOR", "LUMINOSITY",
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

function create_image(int $width, int $height): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, 0x7fffffff);
    imagealphablending($image, true);
    return $image;
}

function label(Gd\Context $ctx, array $font, string $text, float $x, float $y, float $scale): void {
    for ($i = 0, $length = strlen($text); $i < $length; $i++, $x += 6 * $scale) {
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

function render_tile(Gd\CompositeOperator $operator, string $shortName, array $font): GdImage {
    $image = create_image(TILE, TILE);
    $ctx = $image->getContext();

    $linear = (new Gd\LinearGradient(0, 0, TILE, TILE))
        ->addColorStopRgb(0, 0.05, 0.72, 0.86)
        ->addColorStopRgb(0.5, 0.17, 0.16, 0.48)
        ->addColorStopRgb(1, 0.96, 0.32, 0.18);
    $ctx
        ->setSource($linear)
        ->rect(12, 30, TILE * 0.68, TILE * 0.68)
        ->fill();

    $radial = (new Gd\RadialGradient(TILE * 0.66, TILE * 0.40, 2, TILE * 0.57, TILE * 0.52, TILE * 0.48))
        ->addColorStopRgba(0, 1, 0.94, 0.20, 0.96)
        ->addColorStopRgba(0.55, 0.96, 0.12, 0.63, 0.82)
        ->addColorStopRgba(1, 0.20, 0.02, 0.55, 0.12);
    $ctx
        ->setOperator($operator)
        ->setSource($radial)
        ->arc(TILE * 0.58, TILE * 0.52, TILE * 0.43, 0, 2 * GD_2D_PI)
        ->fill()
        ->setOperator(Gd\CompositeOperator::Over)
        ->setSourceRgba(0.05, 0.05, 0.08, 1);
    label($ctx, $font, $shortName, 6, 5, 1.8);

    $ctx->flushImage();
    $ctx->destroy(false);
    return $image;
}

$rows = intdiv(count($operators) + COLS - 1, COLS);
$width = COLS * TILE + (COLS + 1) * GAP;
$height = $rows * TILE + ($rows + 1) * GAP;
$gallery = create_image($width, $height);
imagealphablending($gallery, false);

foreach ($operators as $op => $operator) {
    $tile = render_tile($operator, $shortNames[$op], $font);
    $x = GAP + ($op % COLS) * (TILE + GAP);
    $y = GAP + intdiv($op, COLS) * (TILE + GAP);
    imagecopy($gallery, $tile, $x, $y, 0, 0, TILE, TILE);
    printf("%02d  %s\n", $op, $names[$op]);
}
imagealphablending($gallery, true);

$reference = imagecreatefrompng(__DIR__ . "/gd_2d_gradient_operators.png");
var_dump(imagesx($gallery));
var_dump(imagesy($gallery));
var_dump(imagesx($reference));
var_dump(imagesy($reference));

$diff = $gallery->perceptualDiff($reference, 0.03);
var_dump($diff->pixelsChanged <= 16);
var_dump($diff->maximumDelta < 0.05);
?>
--EXPECT--
00  CLEAR
01  SOURCE
02  OVER
03  IN
04  OUT
05  ATOP
06  DEST
07  DEST_OVER
08  DEST_IN
09  DEST_OUT
10  DEST_ATOP
11  XOR
12  ADD
13  SATURATE
14  MULTIPLY
15  SCREEN
16  OVERLAY
17  DARKEN
18  LIGHTEN
19  COLOR_DODGE
20  COLOR_BURN
21  HARD_LIGHT
22  SOFT_LIGHT
23  DIFFERENCE
24  EXCLUSION
25  HSL_HUE
26  HSL_SATURATION
27  HSL_COLOR
28  HSL_LUMINOSITY
int(798)
int(956)
int(798)
int(956)
bool(true)
bool(true)
