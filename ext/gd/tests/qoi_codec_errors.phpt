--TEST--
Gd\Qoi\Codec error handling
--SKIPIF--
<?php
if (!class_exists('Gd\Qoi\Codec')) die('SKIP: Gd\Qoi\Codec not available');
if (!extension_loaded('gd')) die('SKIP: GD extension not loaded');
?>
--FILE--
<?php
// Create a simple test image
$image = imagecreate(4, 4);
$white = imagecolorallocate($image, 255, 255, 255);

// Test 1: Invalid QOI data
echo "Test 1: fromString with invalid data... ";
try {
    Gd\Qoi\Codec::fromString("invalid qoi data");
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    echo "caught CodecException\n";
} catch (Exception $e) {
    echo "caught " . get_class($e) . "\n";
}

// Test 2: Non-existent file
echo "Test 2: fromFile with non-existent file... ";
try {
    @Gd\Qoi\Codec::fromFile("/nonexistent/path/file.qoi");
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    echo "caught CodecException\n";
} catch (Throwable $e) {
    echo "caught " . get_class($e) . "\n";
}

// Test 3: Invalid stream for writing to unwritable path
echo "Test 3: toFile to invalid path... ";
try {
    @Gd\Qoi\Codec::toFile($image, "/root/cannot/write/here.qoi");
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    echo "caught CodecException\n";
} catch (Throwable $e) {
    echo "caught " . get_class($e) . "\n";
}

// Test 4: Invalid stream type for fromStream
echo "Test 4: fromStream with closed stream... ";
try {
    $stream = fopen('php://memory', 'r+b');
    fclose($stream);
    Gd\Qoi\Codec::fromStream($stream);
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    echo "caught CodecException\n";
} catch (Throwable $e) {
    echo "caught " . get_class($e) . "\n";
}

// Test 5: Empty QOI data
echo "Test 5: fromString with empty data... ";
try {
    Gd\Qoi\Codec::fromString("");
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    echo "caught CodecException\n";
} catch (Exception $e) {
    echo "caught " . get_class($e) . "\n";
}

// Test 6: Header probe rejects a bad end marker
echo "Test 6: Reader info with bad end marker... ";
try {
    Gd\Qoi\Reader::fromString("qoif" . pack('N', 1) . pack('N', 1) . "\x04\x00" . str_repeat("\0", 8));
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    echo "caught CodecException\n";
}

// Test 7: A valid header with no pixel chunk fails and poisons the reader
echo "Test 7: Reader failed state after decode failure... ";
$truncated = "qoif" . pack('N', 1) . pack('N', 1) . "\x04\x00" . str_repeat("\0", 7) . "\x01";
$reader = Gd\Qoi\Reader::fromString($truncated);
try {
    $reader->read();
    echo "FAILED - no exception thrown\n";
} catch (Gd\Codec\CodecException $e) {
    try {
        $reader->read();
        echo "FAILED - failed reader was reusable\n";
    } catch (Gd\Codec\CodecException $e) {
        echo "caught CodecException\n";
    }
}

echo "All error handling tests completed!\n";
?>
--EXPECT--
Test 1: fromString with invalid data... caught CodecException
Test 2: fromFile with non-existent file... caught CodecException
Test 3: toFile to invalid path... caught CodecException
Test 4: fromStream with closed stream... caught TypeError
Test 5: fromString with empty data... caught CodecException
Test 6: Reader info with bad end marker... caught CodecException
Test 7: Reader failed state after decode failure... caught CodecException
All error handling tests completed!
