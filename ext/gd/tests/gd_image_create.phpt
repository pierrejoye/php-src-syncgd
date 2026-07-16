--TEST--
Gd\Image::create()
--EXTENSIONS--
gd
--FILE--
<?php
$truecolor = Gd\Image::create(2, 3);
var_dump($truecolor instanceof GdImage);
var_dump(imageistruecolor($truecolor));
var_dump(imagesx($truecolor), imagesy($truecolor));

$palette = Gd\Image::create(2, 3, Gd\ColorModel::Palette);
var_dump($palette instanceof GdImage);
var_dump(imageistruecolor($palette));
var_dump(imagesx($palette), imagesy($palette));

$method = new ReflectionMethod(Gd\Image::class, 'create');
var_dump($method->isStatic());
var_dump($method->getReturnType()->getName());
var_dump($method->getParameters()[2]->isDefaultValueAvailable());
var_dump($method->getParameters()[2]->getDefaultValue() === Gd\ColorModel::TrueColor);
var_dump(array_map(fn($case) => $case->name, Gd\ColorModel::cases()));

try {
    new Gd\Image();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}

foreach ([[-1, 1], [1, 0]] as [$width, $height]) {
    try {
        Gd\Image::create($width, $height);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
bool(true)
bool(true)
int(2)
int(3)
bool(true)
bool(false)
int(2)
int(3)
bool(true)
string(7) "GdImage"
bool(true)
bool(true)
array(2) {
  [0]=>
  string(9) "TrueColor"
  [1]=>
  string(7) "Palette"
}
Call to private Gd\Image::__construct() from global scope
Gd\Image::create(): Argument #1 ($width) must be greater than 0
Gd\Image::create(): Argument #2 ($height) must be greater than 0
