--TEST--
GH-19666 (Unexpected nan value in imageconvolution)
--EXTENSIONS--
gd
--SKIPIF--
<?php
    if (!GD_BUNDLED) die("skip requires bundled GD library\n");
?>
--FILE--
<?php
$image = imagecreatetruecolor(180, 30);

$gaussian = array(
    array(1.0, 2.0, 1.0),
    array(2.0, 4.0, 2.0),
    array(1.0, 2.0, -INF)
);

try {
    imageconvolution($image, $gaussian, 16, 0);
} catch (ValueError $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
imageconvolution(): Argument #2 ($matrix) matrix[2][2] must be finite
