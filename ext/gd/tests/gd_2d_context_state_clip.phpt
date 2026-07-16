--TEST--
Gd\Context state stack and clipping
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class)) {
    die("skip Gd\\Context not available.");
}
if (!method_exists(GdImage::class, "perceptualDiff")) {
    die("skip GdImage::perceptualDiff() not available.");
}
?>
--FILE--
<?php
function make_image(int $size = 16): GdImage {
    $image = imagecreatetruecolor($size, $size);
    imagefilledrectangle($image, 0, 0, $size - 1, $size - 1, 0x000000);
    return $image;
}

function similar(GdImage $first, GdImage $second): void {
    $diff = $first->perceptualDiff($second, 0.03);
    var_dump($diff->pixelsChanged <= 4);
    var_dump($diff->maximumDelta < 0.05);
}

function color_at(GdImage $image, int $x, int $y): int {
    return imagecolorat($image, $x, $y) & 0xffffff;
}

$image = make_image();
$ctx = $image->getContext();
var_dump($ctx->save() === $ctx);
$ctx->restore();

$ctx
    ->setSourceRgb(1, 0, 0)
    ->setLineWidth(7)
    ->save()
    ->setSourceRgb(0, 0, 1)
    ->setLineWidth(1)
    ->restore()
    ->beginPath()
    ->rect(2, 2, 4, 4)
    ->fill()
    ->flushImage();
var_dump(color_at($image, 3, 3));

$path = make_image();
$pathCtx = $path->getContext();
$pathCtx
    ->beginPath()
    ->rect(2, 2, 4, 4)
    ->save()
    ->restore()
    ->setSourceRgb(1, 0, 0)
    ->fill()
    ->flushImage();
var_dump(color_at($path, 3, 3));

$clipPreserve = make_image();
$clipPreserve
    ->getContext()
    ->beginPath()
    ->rect(3, 3, 6, 6)
    ->clipPreserve()
    ->setSourceRgb(1, 0, 0)
    ->fill()
    ->flushImage();
var_dump(color_at($clipPreserve, 4, 4));
var_dump(color_at($clipPreserve, 2, 2));

$clipConsumesPath = make_image();
$clipConsumesPath
    ->getContext()
    ->beginPath()
    ->rect(3, 3, 6, 6)
    ->clip()
    ->setSourceRgb(1, 0, 0)
    ->fill()
    ->flushImage();
var_dump(color_at($clipConsumesPath, 4, 4));

$clippedPaint = make_image();
$clippedPaint
    ->getContext()
    ->beginPath()
    ->rect(3, 3, 6, 6)
    ->clip()
    ->setSourceRgb(1, 0, 0)
    ->paint()
    ->flushImage();

$expectedClip = make_image();
$expectedClip
    ->getContext()
    ->setSourceRgb(1, 0, 0)
    ->beginPath()
    ->rect(3, 3, 6, 6)
    ->fill()
    ->flushImage();
similar($clippedPaint, $expectedClip);

$restoredClip = make_image();
$restoredCtx = $restoredClip->getContext();
$restoredCtx
    ->beginPath()
    ->rect(2, 2, 10, 10)
    ->clip()
    ->save()
    ->beginPath()
    ->rect(5, 5, 2, 2)
    ->clip()
    ->setSourceRgb(0, 0, 1)
    ->paint()
    ->restore()
    ->setSourceRgb(1, 0, 0)
    ->paint()
    ->flushImage();
var_dump(color_at($restoredClip, 3, 3));
var_dump(color_at($restoredClip, 6, 6));
var_dump(color_at($restoredClip, 1, 1));

try {
    make_image()->getContext()->restore();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}

$destroyed = make_image()->getContext();
$destroyed->destroy(false);
foreach (["save", "restore", "clip", "clipPreserve"] as $method) {
    try {
        $destroyed->$method();
    } catch (Error $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECTF--
bool(true)
int(16711680)
int(16711680)
int(16711680)
int(0)
int(0)
bool(true)
bool(true)
int(16711680)
int(16711680)
int(0)

Warning: Gd\Context::restore(): gdContextRestore: no saved graphics state in %s on line %d
Failed to restore Gd\Context state
Gd\Context has already been destroyed
Gd\Context has already been destroyed
Gd\Context has already been destroyed
Gd\Context has already been destroyed
