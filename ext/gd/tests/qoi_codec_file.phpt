--TEST--
Gd\Qoi\Codec file operations
--SKIPIF--
<?php
if (!class_exists('Gd\Qoi\Codec')) die('SKIP: Gd\Qoi\Codec not available');
if (!extension_loaded('gd')) die('SKIP: GD extension not loaded');
?>
--FILE--
<?php
// Create a test image
$image = imagecreate(8, 8);
$red = imagecolorallocate($image, 255, 0, 0);
$blue = imagecolorallocate($image, 0, 0, 255);

for ($i = 0; $i < 8; $i++) {
    for ($j = 0; $j < 8; $j++) {
        $color = ($i < 4) ? $red : $blue;
        imagesetpixel($image, $i, $j, $color);
    }
}

// Test 1: Write to file with SRGB colorspace
echo "Test 1: toFile with SRGB... ";
$tmp_file = tempnam(sys_get_temp_dir(), 'qoi_');
Gd\Qoi\Codec::toFile($image, $tmp_file);
var_dump(file_exists($tmp_file) && filesize($tmp_file) > 0);

// Test 2: Read from file
echo "Test 2: fromFile... ";
$loaded = Gd\Qoi\Codec::fromFile($tmp_file);
var_dump($loaded instanceof GdImage);

// Test 2b: Read scalar info from file
echo "Test 2b: infoFile... ";
$file_info = Gd\Qoi\Reader::fromFile($tmp_file)->info();
var_dump($file_info->width === 8 && $file_info->height === 8 && $file_info->channels === 4);

// Test 3: Dimensions match
echo "Test 3: Dimensions preserved... ";
var_dump(imagesx($loaded) === 8 && imagesy($loaded) === 8);

// Test 4: Write and read with Linear colorspace
echo "Test 4: toFile/fromFile with Linear... ";
$tmp_file2 = tempnam(sys_get_temp_dir(), 'qoi_');
Gd\Qoi\Codec::toFile($image, $tmp_file2, new Gd\Qoi\WriteOptions(Gd\Qoi\Colorspace::Linear));
$loaded2 = Gd\Qoi\Codec::fromFile($tmp_file2);
var_dump($loaded2 instanceof GdImage && imagesx($loaded2) === 8);

// Test 5: File content is different for different colorspaces
echo "Test 5: Different colorspace produces different file content... ";
$content1 = file_get_contents($tmp_file);
$content2 = file_get_contents($tmp_file2);
var_dump($content1 !== $content2);

// Cleanup
unlink($tmp_file);
unlink($tmp_file2);

echo "All file operation tests passed!\n";
?>
--EXPECT--
Test 1: toFile with SRGB... bool(true)
Test 2: fromFile... bool(true)
Test 2b: infoFile... bool(true)
Test 3: Dimensions preserved... bool(true)
Test 4: toFile/fromFile with Linear... bool(true)
Test 5: Different colorspace produces different file content... bool(true)
All file operation tests passed!
