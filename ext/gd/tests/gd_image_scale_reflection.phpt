--TEST--
GdImage::scale() reflection and options defaults
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!method_exists(GdImage::class, "scale")) {
	die("skip GdImage::scale() not available.");
}
?>
--FILE--
<?php
$options = new Gd\ScaleOptions();
var_dump($options->width);
var_dump($options->height);
var_dump($options->fit);
var_dump($options->gravity);
var_dump($options->strategy);
var_dump($options->backgroundColor);
var_dump($options->interpolation);

$class = new ReflectionClass(Gd\ScaleOptions::class);
var_dump($class->isFinal());
var_dump($class->isReadOnly());

var_dump(array_map(fn($case) => $case->name, Gd\ScaleFit::cases()));
var_dump(array_map(fn($case) => $case->name, Gd\ScaleGravity::cases()));
var_dump(array_map(fn($case) => $case->name, Gd\ScaleStrategy::cases()));
var_dump(array_map(fn($case) => $case->name, Gd\ScaleInterpolation::cases()));

$method = new ReflectionMethod(GdImage::class, 'scale');
var_dump($method->getReturnType()->getName());
foreach ($method->getParameters() as $parameter) {
    printf(
        "%s nullable=%s default=%s\n",
        $parameter->getName(),
        $parameter->allowsNull() ? 'yes' : 'no',
        $parameter->isDefaultValueAvailable() ? var_export($parameter->getDefaultValue(), true) : 'none',
    );
}
?>
--EXPECT--
NULL
NULL
enum(Gd\ScaleFit::Cover)
enum(Gd\ScaleGravity::Center)
NULL
int(2130706432)
NULL
bool(true)
bool(true)
array(5) {
  [0]=>
  string(5) "Cover"
  [1]=>
  string(7) "Contain"
  [2]=>
  string(4) "Fill"
  [3]=>
  string(6) "Inside"
  [4]=>
  string(7) "Outside"
}
array(9) {
  [0]=>
  string(9) "Northwest"
  [1]=>
  string(5) "North"
  [2]=>
  string(9) "Northeast"
  [3]=>
  string(4) "West"
  [4]=>
  string(6) "Center"
  [5]=>
  string(4) "East"
  [6]=>
  string(9) "Southwest"
  [7]=>
  string(5) "South"
  [8]=>
  string(9) "Southeast"
}
array(2) {
  [0]=>
  string(7) "Entropy"
  [1]=>
  string(9) "Attention"
}
array(28) {
  [0]=>
  string(7) "Default"
  [1]=>
  string(4) "Bell"
  [2]=>
  string(6) "Bessel"
  [3]=>
  string(6) "Linear"
  [4]=>
  string(11) "CubicSpline"
  [5]=>
  string(8) "Blackman"
  [6]=>
  string(3) "Box"
  [7]=>
  string(7) "BSpline"
  [8]=>
  string(10) "CatmullRom"
  [9]=>
  string(8) "Gaussian"
  [10]=>
  string(16) "GeneralizedCubic"
  [11]=>
  string(7) "Hermite"
  [12]=>
  string(7) "Hamming"
  [13]=>
  string(7) "Hanning"
  [14]=>
  string(8) "Mitchell"
  [15]=>
  string(16) "NearestNeighbour"
  [16]=>
  string(5) "Power"
  [17]=>
  string(9) "Quadratic"
  [18]=>
  string(4) "Sinc"
  [19]=>
  string(8) "Triangle"
  [20]=>
  string(9) "Weighted4"
  [21]=>
  string(8) "Lanczos3"
  [22]=>
  string(8) "Lanczos8"
  [23]=>
  string(14) "BlackmanBessel"
  [24]=>
  string(12) "BlackmanSinc"
  [25]=>
  string(16) "QuadraticBSpline"
  [26]=>
  string(6) "Cosine"
  [27]=>
  string(5) "Welsh"
}
string(7) "GdImage"
width nullable=yes default=NULL
height nullable=yes default=NULL
options nullable=yes default=NULL
