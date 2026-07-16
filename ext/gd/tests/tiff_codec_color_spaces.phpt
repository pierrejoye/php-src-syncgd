--TEST--
Gd TIFF writer color spaces and grayscale polarity
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

$image = imagecreatetruecolor(1, 1);
imagealphablending($image, false);
imagesavealpha($image, true);
$red = imagecolorallocatealpha($image, 255, 0, 0, 64);
imagesetpixel($image, 0, 0, $red);

foreach ([
    [Gd\Tiff\ColorSpace::Rgb, false],
    [Gd\Tiff\ColorSpace::Rgba, false],
    [Gd\Tiff\ColorSpace::Gray, false],
    [Gd\Tiff\ColorSpace::Gray, true],
] as [$colorSpace, $minIsWhite]) {
    $options = new Gd\Tiff\WriteOptions(
        compression: Gd\Tiff\Compression::None,
        colorSpace: $colorSpace,
        minIsWhite: $minIsWhite,
    );
    $writer = Gd\Tiff\Writer::toMemory($options);
    $writer->addPage($image);
    $bytes = $writer->finish();
    $ifd = tiff_codec_inspect($bytes)[0];
    $offset = is_array($ifd[273]) ? $ifd[273][0] : $ifd[273];
    $samples = $ifd[277];
    $pixel = array_values(unpack('C' . $samples, substr($bytes, $offset, $samples)));
    printf(
        "%s white=%d photometric=%d samples=%d alpha=%s pixel=%s\n",
        $colorSpace->name,
        $minIsWhite,
        $ifd[262],
        $samples,
        isset($ifd[338]) ? (string) $ifd[338] : '-',
        implode(',', $pixel),
    );
}
?>
--EXPECT--
Rgb white=0 photometric=2 samples=3 alpha=- pixel=127,0,0
Rgba white=0 photometric=2 samples=4 alpha=2 pixel=255,0,0,126
Gray white=0 photometric=1 samples=1 alpha=- pixel=54
Gray white=1 photometric=0 samples=1 alpha=- pixel=201
