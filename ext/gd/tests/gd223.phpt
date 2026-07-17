--TEST--
libgd bug 223 (gdImageRotateGeneric() does not properly interpolate)
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!GD_BUNDLED) die("skip only for bundled libgd");
require __DIR__ . "/skipif_gd_perceptual_diff_supported.inc";
?>
--FILE--
<?php
$im = imagecreatetruecolor(64, 64);
for ($j = 0; $j < 64; $j++) {
    for ($i = 0; $i < 64; $i++) {
        imagesetpixel($im, $i, $j, ($i % 2 || $j % 2) ? 0x000000 : 0xffffff);
    }
}

imagesetinterpolation($im, IMG_BICUBIC);
$im = imagerotate($im, 45, 0xff0000);

$expected = imagecreatefrompng(__DIR__ . "/gd223.png");
$diff = $im->perceptualDiff($expected, 0.03);
var_dump($diff->pixelsChanged < 5 && $diff->maximumDelta < 0.08);
?>
--EXPECT--
bool(true)
