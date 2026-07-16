--TEST--
Gd\Qoi\Codec stream operations
--SKIPIF--
<?php
if (!class_exists('Gd\Qoi\Codec')) die('SKIP: Gd\Qoi\Codec not available');
if (!extension_loaded('gd')) die('SKIP: GD extension not loaded');
?>
--FILE--
<?php
// Create a test image
$image = imagecreate(6, 6);
$green = imagecolorallocate($image, 0, 255, 0);
$yellow = imagecolorallocate($image, 255, 255, 0);

for ($i = 0; $i < 6; $i++) {
    for ($j = 0; $j < 6; $j++) {
        $color = (($i + $j) % 2) ? $green : $yellow;
        imagesetpixel($image, $i, $j, $color);
    }
}

// Test 1: Write to stream (memory) with SRGB
echo "Test 1: toStream with SRGB... ";
$stream = fopen('php://memory', 'r+b');
Gd\Qoi\Codec::toStream($image, $stream);
rewind($stream);
$stream_content = stream_get_contents($stream);
var_dump(is_string($stream_content) && strlen($stream_content) > 0);

// Test 2: Read from stream
echo "Test 2: fromStream... ";
rewind($stream);
$loaded = Gd\Qoi\Codec::fromStream($stream);
var_dump($loaded instanceof GdImage);

// Test 3: Dimensions match
echo "Test 3: Dimensions preserved... ";
var_dump(imagesx($loaded) === 6 && imagesy($loaded) === 6);

// Test 4: Write to stream with Linear colorspace
echo "Test 4: toStream with Linear... ";
$stream2 = fopen('php://memory', 'r+b');
Gd\Qoi\Codec::toStream($image, $stream2, new Gd\Qoi\WriteOptions(Gd\Qoi\Colorspace::Linear));
rewind($stream2);
$stream_content2 = stream_get_contents($stream2);
var_dump(is_string($stream_content2) && strlen($stream_content2) > 0);

// Test 5: Stream content differs for different colorspaces
echo "Test 5: Different colorspaces produce different stream content... ";
var_dump($stream_content !== $stream_content2);

// Test 6: Stream can be read after toStream (stream ownership)
echo "Test 6: Stream position after toStream... ";
$stream3 = fopen('php://memory', 'r+b');
$initial_pos = ftell($stream3);
Gd\Qoi\Codec::toStream($image, $stream3);
$final_pos = ftell($stream3);
var_dump($final_pos > $initial_pos);

// Cleanup
fclose($stream);
fclose($stream2);
fclose($stream3);

echo "All stream operation tests passed!\n";
?>
--EXPECT--
Test 1: toStream with SRGB... bool(true)
Test 2: fromStream... bool(true)
Test 3: Dimensions preserved... bool(true)
Test 4: toStream with Linear... bool(true)
Test 5: Different colorspaces produce different stream content... bool(true)
Test 6: Stream position after toStream... bool(true)
All stream operation tests passed!
