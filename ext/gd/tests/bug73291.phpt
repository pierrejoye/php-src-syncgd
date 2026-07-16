--TEST--
Bug #73291 (imagecropauto() $threshold differs from external libgd)
--EXTENSIONS--
gd
--FILE--
<?php

$src = imagecreatetruecolor(255, 255);
$white = imagecolorallocate($src, 255, 255, 255);
imagefilledrectangle($src, 0, 0, 254, 254, $white);

for ($i = 254; $i > 0; $i--) {
    $color = imagecolorallocate($src, $i, $i, $i);
    imagefilledellipse($src, 127, 127, $i, $i, $color);
}

$expectedSizes = [255, 253, 253, 225];
if (!GD_BUNDLED && version_compare(GD_VERSION, '2.4.0', '<')) {
    $expectedSizes = [247, 237, 229, 175];
}

foreach ([0.1, 0.5, 1.0, 10.0] as $i => $threshold) {
    $dst = imagecropauto($src, IMG_CROP_THRESHOLD, $threshold, $white);
    if ($dst !== false) {
        printf(
            "%s\n",
            imagesx($dst) === $expectedSizes[$i] && imagesy($dst) === $expectedSizes[$i]
                ? "size matches"
                : sprintf("size mismatch: expected %d*%d, got %d*%d", $expectedSizes[$i], $expectedSizes[$i], imagesx($dst), imagesy($dst))
        );
    } else {
        echo "cropped to zero size\n";
    }
}

?>
--EXPECT--
size matches
size matches
size matches
size matches
