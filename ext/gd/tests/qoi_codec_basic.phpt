--TEST--
Gd\Qoi\Codec basic functionality
--SKIPIF--
<?php
if (!class_exists('Gd\Qoi\Codec')) die('SKIP: Gd\Qoi\Codec not available');
if (!extension_loaded('gd')) die('SKIP: GD extension not loaded');
?>
--FILE--
<?php
// Create a simple test image
$image = imagecreate(10, 10);
$white = imagecolorallocate($image, 255, 255, 255);
$black = imagecolorallocate($image, 0, 0, 0);

// Fill with pattern
for ($i = 0; $i < 10; $i++) {
    for ($j = 0; $j < 10; $j++) {
        $color = (($i + $j) % 2) ? $white : $black;
        imagesetpixel($image, $i, $j, $color);
    }
}

// Test 1: Convert to string and back
echo "Test 1: String round-trip... ";
$qoi_data = Gd\Qoi\Codec::toString($image);
var_dump(is_string($qoi_data) && strlen($qoi_data) > 0);
$restored = Gd\Qoi\Codec::fromString($qoi_data);
var_dump($restored instanceof GdImage);

// Test 2: Verify image dimensions are preserved
echo "Test 2: Image dimensions... ";
$w = imagesx($restored);
$h = imagesy($restored);
var_dump($w === 10 && $h === 10);

// Test 3: Test Linear colorspace
echo "Test 3: Linear colorspace... ";
$qoi_linear = Gd\Qoi\Codec::toString($image, new Gd\Qoi\WriteOptions(Gd\Qoi\Colorspace::Linear));
var_dump(is_string($qoi_linear) && strlen($qoi_linear) > 0);

// Test 4: Verify both colorspaces produce different results
echo "Test 4: Different colorspaces produce different data... ";
var_dump($qoi_data !== $qoi_linear);

echo "All basic tests passed!\n";
?>
--EXPECT--
Test 1: String round-trip... bool(true)
bool(true)
Test 2: Image dimensions... bool(true)
Test 3: Linear colorspace... bool(true)
Test 4: Different colorspaces produce different data... bool(true)
All basic tests passed!
