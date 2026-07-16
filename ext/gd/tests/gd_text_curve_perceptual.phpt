--TEST--
GD text paths follow a curve with patterned fill and stroked outlines
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\FontFace::class) || !class_exists(Gd\Pattern::class)) {
    die("skip GD text/path pattern APIs not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
if (!function_exists("imagecreatefrompng")) {
    die("skip PNG support not available.");
}
if (!is_file(__DIR__ . "/textcurve.png")) {
    die("skip text curve reference image not available.");
}
if (!is_file(__DIR__ . "/DejaVuSans.ttf")) {
    die("skip DejaVuSans.ttf test font not available.");
}
?>
--FILE--
<?php
const GD_TEXT_PI = 3.14159265358979323846;

function text_curve_create_tile(): GdImage {
    $tile = imagecreatetruecolor(48, 48);
    imagefilledrectangle($tile, 0, 0, 47, 47, 0x2084ff);
    imagefilledrectangle($tile, 0, 0, 47, 15, 0xff6aa6);
    imagefilledrectangle($tile, 0, 32, 47, 47, 0x48e0e6);
    imagefilledrectangle($tile, 0, 0, 11, 47, 0xffce48);
    imagefilledrectangle($tile, 28, 0, 35, 47, 0xfff5b9);
    return $tile;
}

function text_curve_create_canvas(int $width, int $height): GdImage {
    $image = imagecreatetruecolor($width, $height);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefilledrectangle($image, 0, 0, $width - 1, $height - 1, 0x7f000000);
    imagealphablending($image, true);
    return $image;
}

function text_curve_draw_sine_text(
    Gd\Context $ctx,
    string $text,
    float $left,
    float $baseline,
    float $amplitude,
    float $cycles,
    Gd\Paint $fill,
): void {
    $total = $ctx->textExtents($text);
    $pen = 0.0;

    if ($total->xAdvance <= 0.0) {
        return;
    }

    $len = strlen($text);
    for ($i = 0; $i < $len; $i++) {
        $ch = $text[$i];
        $ext = $ctx->textExtents($ch);
        $center = $pen + $ext->xAdvance * 0.5;
        $phase = ($center / $total->xAdvance) * (2.0 * GD_TEXT_PI * $cycles);
        $x = $left + $center;
        $y = $baseline + $amplitude * sin($phase);
        $slope = $amplitude * (2.0 * GD_TEXT_PI * $cycles / $total->xAdvance) * cos($phase);
        $angle = atan($slope);

        if ($ch !== ' ') {
            $ctx
                ->save()
                ->translate($x, $y)
                ->rotate($angle)
                ->textPath($ch, -$ext->xAdvance * 0.5, 0.0)
                ->setSourceRgba(0.03, 0.05, 0.09, 0.98)
                ->setLineJoin(Gd\LineJoin::Round)
                ->setLineWidth(7.0)
                ->strokePreserve()
                ->setSource($fill)
                ->fillPreserve()
                ->setSourceRgba(0.03, 0.05, 0.09, 0.98)
                ->setLineJoin(Gd\LineJoin::Round)
                ->setLineWidth(2.2)
                ->stroke()
                ->restore();
        }

        $pen += $ext->xAdvance;
    }
}

$width = 1200;
$height = 320;
$text = "Text follows a curve with gd 2D paths";

$image = text_curve_create_canvas($width, $height);
$tile = text_curve_create_tile();
$ctx = $image->getContext();
$font = Gd\FontFace::fromFile(__DIR__ . "/DejaVuSans.ttf");

$ctx
    ->setFontFace($font)
    ->setFontSize(54);

$extents = $ctx->textExtents($text);
$left = ($width - $extents->xAdvance) * 0.5;
$pattern = $ctx
    ->createPattern($tile)
    ->setExtend(Gd\ExtendMode::Repeat);

text_curve_draw_sine_text($ctx, $text, $left, 178.0, 52.0, 1.35, $pattern);
$ctx->flushImage();

$reference = imagecreatefrompng(__DIR__ . "/textcurve.png");
var_dump(imagesx($image));
var_dump(imagesy($image));
var_dump(imagesx($reference));
var_dump(imagesy($reference));

$diff = $image->perceptualDiff($reference, 0.03);
var_dump($diff->pixelsChanged <= 16);
var_dump($diff->maximumDelta < 0.05);

?>
--EXPECT--
int(1200)
int(320)
int(1200)
int(320)
bool(true)
bool(true)
