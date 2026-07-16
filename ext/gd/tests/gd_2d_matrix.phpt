--TEST--
Gd\Matrix operations
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Matrix::class)) {
    die("skip Gd\\Matrix not available.");
}
?>
--FILE--
<?php
function dump_point(array $point): void {
    printf("%.1f,%.1f\n", $point[0], $point[1]);
}

$identity = new Gd\Matrix();
dump_point($identity->mapPoint(4, 5));

$translation = Gd\Matrix::translation(2, 3);
dump_point($translation->mapPoint(4, 5));

$scaling = Gd\Matrix::scaling(2, 3);
dump_point($scaling->mapPoint(4, 5));
dump_point($scaling->inverted()->mapPoint(4, 6));

$singular = Gd\Matrix::scaling(0, 1);
var_dump($singular->inverted());

$translated = $identity->translate(2, 3);
dump_point($identity->mapPoint(4, 5));
dump_point($translated->mapPoint(4, 5));

$fromCoefficients = Gd\Matrix::fromCoefficients(1, 2, 3, 4, 5, 6);
dump_point($fromCoefficients->mapPoint(7, 8));

$combined = Gd\Matrix::translation(2, 0)->multiply(Gd\Matrix::scaling(3, 1));
dump_point($combined->mapPoint(4, 5));

$shearing = Gd\Matrix::shearing(0, 0);
dump_point($shearing->mapPoint(4, 5));
dump_point($identity->shear(0, 0)->mapPoint(4, 5));

$rotationAround = Gd\Matrix::rotationAround(M_PI / 2, 1, 1);
dump_point($rotationAround->mapPoint(2, 1));
dump_point($identity->rotateAround(M_PI / 2, 1, 1)->mapPoint(2, 1));

$rect = Gd\Matrix::translation(2, 3)->mapRect(new Gd\Rect(1, 2, 3, 4));
printf("%.1f,%.1f,%.1f,%.1f\n", $rect->x, $rect->y, $rect->width, $rect->height);

try {
    $rect->x = 42;
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
4.0,5.0
6.0,8.0
8.0,15.0
2.0,2.0
NULL
4.0,5.0
6.0,8.0
36.0,52.0
18.0,5.0
4.0,5.0
4.0,5.0
1.0,2.0
1.0,2.0
3.0,5.0,3.0,4.0
Cannot modify readonly property Gd\Rect::$x
