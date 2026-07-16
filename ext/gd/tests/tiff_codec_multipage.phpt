--TEST--
Gd TIFF writer supports mixed page dimensions, alpha, and inherited resolution
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Writer::class)) {
    die('skip TIFF writer API is unavailable');
}
?>
--FILE--
<?php
require __DIR__ . '/tiff_codec_helpers.inc';

$opaque = imagecreatetruecolor(3, 2);
imageresolution($opaque, 144, 120);
$red = imagecolorallocate($opaque, 255, 0, 0);
imagefilledrectangle($opaque, 0, 0, 2, 1, $red);

$alpha = imagecreatetruecolor(2, 4);
imageresolution($alpha, 300, 300);
imagealphablending($alpha, false);
imagesavealpha($alpha, true);
$blue = imagecolorallocatealpha($alpha, 0, 0, 255, 64);
imagefilledrectangle($alpha, 0, 0, 1, 3, $blue);

$options = new Gd\Tiff\WriteOptions(compression: Gd\Tiff\Compression::None);
$writer = Gd\Tiff\Writer::toMemory($options);
var_dump($writer->addPage($opaque) === $writer);
var_dump($writer->addPage($alpha) === $writer);
$bytes = $writer->finish();
$ifds = tiff_codec_inspect($bytes);

var_dump(count($ifds));
foreach ($ifds as $ifd) {
	$bitsPerSample = is_array($ifd[258]) ? $ifd[258][0] : $ifd[258];
    printf(
        "%dx%d bps=%d spp=%d compression=%d res=%.0fx%.0f unit=%d alpha=%d\n",
        $ifd[256], $ifd[257], $bitsPerSample, $ifd[277], $ifd[259],
        $ifd[282], $ifd[283], $ifd[296], $ifd[338]
    );
}

// With uncompressed contiguous RGBA, inspect the first pixel in each page.
foreach ($ifds as $ifd) {
    $offset = is_array($ifd[273]) ? $ifd[273][0] : $ifd[273];
    var_dump(array_values(unpack('C4', substr($bytes, $offset, 4))));
}
?>
--EXPECT--
bool(true)
bool(true)
int(2)
3x2 bps=8 spp=4 compression=1 res=144x120 unit=2 alpha=2
2x4 bps=8 spp=4 compression=1 res=144x120 unit=2 alpha=2
array(4) {
  [0]=>
  int(255)
  [1]=>
  int(0)
  [2]=>
  int(0)
  [3]=>
  int(255)
}
array(4) {
  [0]=>
  int(0)
  [1]=>
  int(0)
  [2]=>
  int(255)
  [3]=>
  int(126)
}
