--TEST--
GdImage::transform()
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Matrix::class)) {
    die("skip Gd\\Matrix not available.");
}
if (!method_exists(GdImage::class, "transform")) {
    die("skip GdImage::transform() not available.");
}
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
function make_transform_source(): GdImage {
    $image = imagecreatetruecolor(5, 4);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    imagefill($image, 0, 0, 0x7f000000);
    imagefilledrectangle($image, 0, 0, 2, 1, 0x00112233);
    imagefilledrectangle($image, 2, 1, 4, 3, 0x40556677);
    imagesetpixel($image, 4, 0, 0x0000ff00);
    imagesetinterpolation($image, IMG_NEAREST_NEIGHBOUR);
    return $image;
}

function clone_image(GdImage $image): GdImage {
    $copy = imagecreatetruecolor(imagesx($image), imagesy($image));
    imagealphablending($copy, false);
    imagesavealpha($copy, true);
    imagecopy($copy, $image, 0, 0, 0, 0, imagesx($image), imagesy($image));
    imagesetinterpolation($copy, imagegetinterpolation($image));
    return $copy;
}

function assert_same_image(string $label, GdImage $actual, GdImage $expected): void {
    if (imagesx($actual) !== imagesx($expected) || imagesy($actual) !== imagesy($expected)) {
        printf("%s dimensions differ: %dx%d != %dx%d\n", $label, imagesx($actual), imagesy($actual), imagesx($expected), imagesy($expected));
        return;
    }

    for ($y = 0; $y < imagesy($actual); $y++) {
        for ($x = 0; $x < imagesx($actual); $x++) {
            if (imagecolorat($actual, $x, $y) !== imagecolorat($expected, $x, $y)) {
                printf("%s pixel differs at %d,%d: %08X != %08X\n", $label, $x, $y, imagecolorat($actual, $x, $y), imagecolorat($expected, $x, $y));
                return;
            }
        }
    }

    echo "$label ok\n";
}

function assert_similar_image(string $label, GdImage $actual, GdImage $expected): void {
    if (imagesx($actual) !== imagesx($expected) || imagesy($actual) !== imagesy($expected)) {
        printf("%s dimensions differ: %dx%d != %dx%d\n", $label, imagesx($actual), imagesy($actual), imagesx($expected), imagesy($expected));
        return;
    }

    $diff = $actual->perceptualDiff($expected, 0.03);
    if ($diff->pixelsChanged > 2 || $diff->maximumDelta > 0.08) {
        printf("%s differs: pixels=%d max=%.6f\n", $label, $diff->pixelsChanged, $diff->maximumDelta);
        return;
    }

    echo "$label ok\n";
}

$source = make_transform_source();
$matrix = Gd\Matrix::fromCoefficients(1, 0, 0.25, 1, 0, 0);
$affine = [1, 0, 0.25, 1, 0, 0];
$actual = $source->transform($matrix);
$expected = imageaffine($source, $affine);
var_dump($actual instanceof GdImage);
assert_similar_image('shear', $actual, $expected);
printf("%dx%d\n", imagesx($actual), imagesy($actual));

$identity = Gd\Matrix::fromCoefficients(1, 0, 0, 1, 0, 0);
$identityResult = $source->transform($identity);
$identityExpected = imageaffine($source, [1, 0, 0, 1, 0, 0]);
var_dump($identityResult !== $source);
assert_similar_image('identity', $identityResult, $identityExpected);

$mutationSource = make_transform_source();
$beforeTransform = clone_image($mutationSource);
$mutationSource->transform($matrix);
assert_same_image('source unchanged', $mutationSource, $beforeTransform);

$clip = $source->transform($identity, new Gd\TransformOptions(clip: new Gd\Rect(1.4, 0.2, 2.4, 2.2)));
$clipExpected = imageaffine($source, [1, 0, 0, 1, 0, 0], ['x' => 1, 'y' => 0, 'width' => 2, 'height' => 2]);
assert_similar_image('rounded clip', $clip, $clipExpected);
printf("%dx%d\n", imagesx($clip), imagesy($clip));

$interpolationSource = clone_image($source);
imagesetinterpolation($interpolationSource, IMG_CATMULLROM);
$interpolated = $source->transform($matrix, new Gd\TransformOptions(interpolation: Gd\InterpolationMethod::CatmullRom));
$interpolatedExpected = imageaffine($interpolationSource, $affine);
assert_similar_image('explicit interpolation', $interpolated, $interpolatedExpected);
printf("%d\n", imagegetinterpolation($source));

$background = $source->transform(Gd\Matrix::rotationAround(M_PI / 5, 2, 2), new Gd\TransformOptions(backgroundColor: 0x00ffffff));
$backgroundPixelCount = 0;
for ($y = 0; $y < imagesy($background); $y++) {
    for ($x = 0; $x < imagesx($background); $x++) {
        if (imagecolorat($background, $x, $y) === 0x00ffffff) {
            $backgroundPixelCount++;
        }
    }
}
var_dump($backgroundPixelCount > 0);

$palette = imagecreate(3, 3);
$transparent = imagecolorallocatealpha($palette, 0, 0, 0, 127);
$red = imagecolorallocate($palette, 255, 0, 0);
imagefill($palette, 0, 0, $transparent);
imagesetpixel($palette, 1, 1, $red);
$paletteResult = $palette->transform($identity);
var_dump(imageistruecolor($palette));
var_dump(imageistruecolor($paletteResult));

$method = new ReflectionMethod(GdImage::class, 'transform');
var_dump($method->getReturnType()->getName());
var_dump($method->getParameters()[0]->getType()->getName());
var_dump($method->getParameters()[1]->getType()->getName());
var_dump($method->getParameters()[1]->allowsNull());
var_dump($method->getParameters()[1]->isDefaultValueAvailable());
var_dump($method->getParameters()[1]->getDefaultValue());

$options = new Gd\TransformOptions();
var_dump($options->clip);
var_dump($options->backgroundColor);
var_dump($options->interpolation);

foreach ([
    Gd\Matrix::scaling(0, 1),
    Gd\Matrix::fromCoefficients(INF, 0, 0, 1, 0, 0),
] as $badMatrix) {
    try {
        $source->transform($badMatrix);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

foreach ([
    new Gd\Rect(0, 0, 0.4, 1),
    new Gd\Rect(0, 0, 1, NAN),
    new Gd\Rect(0, 0, PHP_INT_MAX, 1),
] as $badClip) {
    try {
        $source->transform($identity, new Gd\TransformOptions(clip: $badClip));
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

try {
    new Gd\TransformOptions(backgroundColor: 0x80000000);
} catch (ValueError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
bool(true)
shear ok
6x4
bool(true)
identity ok
source unchanged ok
rounded clip ok
2x2
explicit interpolation ok
%d
bool(true)
bool(false)
bool(true)
string(7) "GdImage"
string(9) "Gd\Matrix"
string(19) "Gd\TransformOptions"
bool(true)
bool(true)
NULL
NULL
int(2130706432)
NULL
GdImage::transform(): Argument #1 ($matrix) must be invertible
GdImage::transform(): Argument #1 ($matrix) must contain only finite values
Gd\Rect::$width must round to a value greater than 0
Gd\Rect::$height must be finite
Gd\Rect::$width must round to a value between %i and %d
Gd\TransformOptions::__construct(): Argument #2 ($backgroundColor) must be a valid GD truecolor alpha color
