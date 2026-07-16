--TEST--
Bug #73272 (imagescale() is not affected by, but affects imagesetinterpolation())
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!(imagetypes() & IMG_PNG)) {
    die("skip No PNG support");
}
?>
--FILE--
<?php
require_once __DIR__ . DIRECTORY_SEPARATOR . 'func.inc';

$src = imagecreatetruecolor(100, 100);
imagefilledrectangle($src, 0,0, 99,99, 0xFFFFFF);
imageellipse($src, 49,49, 40,40, 0x000000);

imagesetinterpolation($src, IMG_NEAREST_NEIGHBOUR);
imagescale($src, 200, 200, IMG_BILINEAR_FIXED);
$dst = imagerotate($src, 60, 0xFFFFFF);

$expected = __DIR__ . DIRECTORY_SEPARATOR . 'bug73272.png';
if (!GD_BUNDLED && version_compare(GD_VERSION, '2.4.0', '<')) {
    $expected = __DIR__ . DIRECTORY_SEPARATOR . 'bug73272_external_pre_24.png';
}

test_image_equals_file($expected, $dst);
?>
--EXPECT--
The images are equal.
