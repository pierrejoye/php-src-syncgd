--TEST--
Gd\Tiff\Reader file and caller-owned stream sources
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Reader::class)) die('skip TIFF reader API unavailable');
?>
--FILE--
<?php
$image = imagecreatetruecolor(2, 1);
$writer = Gd\Tiff\Writer::toMemory(new Gd\Tiff\WriteOptions(compression: Gd\Tiff\Compression::None));
$bytes = $writer->addPage($image)->finish();

$path = __DIR__ . '/tiff_reader_source.tmp';
file_put_contents($path, $bytes);
$fileReader = Gd\Tiff\Reader::fromFile($path);
var_dump($fileReader->next()->width);
unlink($path);

$stream = fopen('php://memory', 'w+b');
fwrite($stream, 'prefix' . $bytes);
fseek($stream, 6);
$streamReader = Gd\Tiff\Reader::fromStream($stream);
var_dump(ftell($stream) === strlen('prefix' . $bytes));
var_dump(is_resource($stream));
var_dump($streamReader->next()->height, $streamReader->next());
fclose($stream);

foreach (['', 'not a tiff'] as $invalid) {
    try {
        Gd\Tiff\Reader::fromString($invalid);
    } catch (Gd\Codec\CodecException $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
int(2)
bool(true)
bool(true)
int(1)
NULL
Failed to open TIFF input
: Not a TIFF or MDI file, bad magic number 28526 (0x6f6e).
Failed to open TIFF input
