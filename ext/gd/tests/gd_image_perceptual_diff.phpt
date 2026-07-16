--TEST--
GdImage::perceptualDiff()
--EXTENSIONS--
gd
--SKIPIF--
<?php
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
function make_image(int $w = 2, int $h = 2): GdImage {
    $image = Gd\Image::create($w, $h);
    imagefill($image, 0, 0, 0x000000);
    return $image;
}

$first = make_image();
$same = make_image();
$changed = make_image();
imagesetpixel($changed, 0, 0, 0xffffff);

$identical = $first->perceptualDiff($same, 0.1);
var_dump($identical instanceof Gd\PerceptualDiffResult);
var_dump($identical->pixelsChanged);
var_dump($identical->maximumDelta);
var_dump($identical->diffImage);

$stats = $first->perceptualDiff($changed, 0.1);
var_dump($stats->pixelsChanged);
var_dump($stats->maximumDelta > 0.9);
var_dump($stats->diffImage);

$overlay = $first->perceptualDiff(
    $changed,
    0.1,
    new Gd\PerceptualDiffOptions(Gd\PerceptualDiffMode::Overlay, 0x00ff00),
);
var_dump($overlay->pixelsChanged);
var_dump($overlay->diffImage instanceof GdImage);
var_dump(imageistruecolor($overlay->diffImage));

$mask = $first->perceptualDiff(
    $changed,
    0.1,
    new Gd\PerceptualDiffOptions(Gd\PerceptualDiffMode::Mask),
);
var_dump($mask->pixelsChanged);
var_dump($mask->diffImage instanceof GdImage);
var_dump(imageistruecolor($mask->diffImage));

$options = new Gd\PerceptualDiffOptions();
var_dump($options->mode === Gd\PerceptualDiffMode::None);
var_dump($options->highlightColor);
var_dump(array_map(fn($case) => $case->name, Gd\PerceptualDiffMode::cases()));

$method = new ReflectionMethod(GdImage::class, 'perceptualDiff');
var_dump($method->isStatic());
var_dump($method->getReturnType()->getName());
var_dump($method->getParameters()[2]->isDefaultValueAvailable());
var_dump($method->getParameters()[2]->getDefaultValue() instanceof Gd\PerceptualDiffOptions);

foreach ([-0.1, 1.1] as $threshold) {
    try {
        $first->perceptualDiff($same, $threshold);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

try {
    $first->perceptualDiff(make_image(1, 1), 0.1);
} catch (ValueError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
bool(true)
int(0)
float(0)
NULL
int(1)
bool(true)
NULL
int(1)
bool(true)
bool(true)
int(1)
bool(true)
bool(true)
bool(true)
int(16711680)
array(3) {
  [0]=>
  string(4) "None"
  [1]=>
  string(7) "Overlay"
  [2]=>
  string(4) "Mask"
}
bool(false)
string(23) "Gd\PerceptualDiffResult"
bool(true)
bool(true)
GdImage::perceptualDiff(): Argument #2 ($threshold) must be between 0.0 and 1.0
GdImage::perceptualDiff(): Argument #2 ($threshold) must be between 0.0 and 1.0
GdImage::perceptualDiff(): Argument #1 ($image) must have the same dimensions as the receiver image
