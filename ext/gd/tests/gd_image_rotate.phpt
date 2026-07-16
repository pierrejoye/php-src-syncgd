--TEST--
GdImage::rotate()
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!method_exists(GdImage::class, "rotate")) {
    die("skip GdImage::rotate() not available.");
}
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
function make_rotate_source(): GdImage {
    $image = imagecreatetruecolor(24, 16);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, 0x7f000000);
    imagefilledrectangle($image, 3, 2, 20, 13, 0x00144a7a);
    imagefilledrectangle($image, 7, 5, 16, 10, 0x402ad95f);
    imagealphablending($image, true);
    return $image;
}

function legacy_reference(GdImage $image, float $angle, int $backgroundColor): GdImage {
    $old = clone_for_reference($image);
    for ($y = 0; $y < imagesy($old); $y++) {
        for ($x = 0; $x < imagesx($old); $x++) {
            if ((imagecolorat($old, $x, $y) >> 24) === 0x7f) {
                imagesetpixel($old, $x, $y, $backgroundColor);
            }
        }
    }
    imagealphablending($old, false);
    imagesavealpha($old, true);
    $rotated = imagerotate($old, $angle, $backgroundColor);
    imagealphablending($rotated, false);
    imagesavealpha($rotated, true);
    return $rotated;
}

function clone_for_reference(GdImage $image): GdImage {
    $copy = imagecreatetruecolor(imagesx($image), imagesy($image));
    imagealphablending($copy, false);
    imagesavealpha($copy, true);
    imagecopy($copy, $image, 0, 0, 0, 0, imagesx($image), imagesy($image));
    return $copy;
}

function assert_similar(string $label, GdImage $actual, GdImage $expected): void {
    if (imagesx($actual) !== imagesx($expected) || imagesy($actual) !== imagesy($expected)) {
        echo "$label dimensions differ\n";
        return;
    }

    $diff = $actual->perceptualDiff($expected, 0.03);
    if ($diff->pixelsChanged > 2 || $diff->maximumDelta > 0.08) {
        echo "$label differs: pixels={$diff->pixelsChanged} max={$diff->maximumDelta}\n";
    }
}

$source = make_rotate_source();
$rotated = $source->rotate(24);
$reference = legacy_reference($source, 24, 0x7f000000);
var_dump($rotated instanceof GdImage);
var_dump(imageistruecolor($rotated));
assert_similar('default background', $rotated, $reference);
printf("%08X\n", imagecolorat($rotated, 0, 0));

$custom = $source->rotate(24, new Gd\RotateOptions(backgroundColor: 0x00ff0000));
$customReference = legacy_reference($source, 24, 0x00ff0000);
assert_similar('custom background', $custom, $customReference);
printf("%08X\n", imagecolorat($custom, 0, 0));
printf("%08X\n", imagecolorat($custom, 15, 3));

$allocatedBackground = imagecolorallocatealpha($source, 0, 255, 0, 50);
$allocated = $source->rotate(24, new Gd\RotateOptions(backgroundColor: $allocatedBackground));
$allocatedReference = legacy_reference($source, 24, $allocatedBackground);
assert_similar('allocated background', $allocated, $allocatedReference);
printf("%08X\n", imagecolorat($allocated, 15, 3));

$control = imagecreatetruecolor(1, 1);
imagefill($control, 0, 0, 0x00000000);
$blendingSource = imagecreatetruecolor(1, 1);
imagefill($blendingSource, 0, 0, 0x00000000);
imagealphablending($control, true);
imagealphablending($blendingSource, true);
$blendingSource->rotate(15);
imagesetpixel($control, 0, 0, 0x40ffffff);
imagesetpixel($blendingSource, 0, 0, 0x40ffffff);
var_dump(imagecolorat($blendingSource, 0, 0) === imagecolorat($control, 0, 0));

$options = new Gd\RotateOptions();
var_dump($options->backgroundColor);
$method = new ReflectionMethod(GdImage::class, 'rotate');
var_dump($method->getReturnType()->getName());
var_dump($method->getParameters()[1]->allowsNull());
var_dump($method->getParameters()[1]->isDefaultValueAvailable());
var_dump($method->getParameters()[1]->getDefaultValue());

foreach ([-21474837, 21474837] as $angle) {
    try {
        $source->rotate($angle);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

foreach ([-1, 2147483648] as $backgroundColor) {
    try {
        new Gd\RotateOptions($backgroundColor);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
bool(true)
bool(true)
7F000000
00FF0000
00FF0000
3200FF00
bool(true)
int(2130706432)
string(7) "GdImage"
bool(true)
bool(true)
NULL
GdImage::rotate(): Argument #1 ($angle) must be between -21474836 and 21474836
GdImage::rotate(): Argument #1 ($angle) must be between -21474836 and 21474836
Gd\RotateOptions::__construct(): Argument #1 ($backgroundColor) must be between 0 and 2147483647
Gd\RotateOptions::__construct(): Argument #1 ($backgroundColor) must be between 0 and 2147483647
