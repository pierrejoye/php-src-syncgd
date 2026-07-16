--TEST--
Gd TIFF writer options, compression modes, and errors
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

$image = imagecreatetruecolor(3, 2);
foreach ([
    Gd\Tiff\Compression::None,
    Gd\Tiff\Compression::Lzw,
    Gd\Tiff\Compression::AdobeDeflate,
    Gd\Tiff\Compression::Deflate,
    Gd\Tiff\Compression::PackBits,
] as $compression) {
    $options = new Gd\Tiff\WriteOptions(
        compression: $compression,
        colorSpace: Gd\Tiff\ColorSpace::Gray,
        minIsWhite: true,
        resolutionUnit: Gd\Tiff\ResolutionUnit::Centimeter,
        xResolution: 40,
        yResolution: 50,
    );
    $writer = Gd\Tiff\Writer::toMemory($options);
    $writer->addPage($image);
    $ifd = tiff_codec_inspect($writer->finish())[0];
    printf("%s:%d:%d:%d:%.0fx%.0f\n", $compression->name, $ifd[259], $ifd[262], $ifd[296], $ifd[282], $ifd[283]);
}

foreach ([
    Gd\Tiff\Compression::CcittRle,
    Gd\Tiff\Compression::CcittFax3,
    Gd\Tiff\Compression::CcittFax4,
] as $compression) {
    try {
        @Gd\Tiff\Writer::toMemory(new Gd\Tiff\WriteOptions(compression: $compression))->addPage($image);
    } catch (Gd\Codec\CodecException $e) {
        echo $compression->name, ':', $e->getMessage(), "\n";
    }
}

foreach ([0, -1, INF, NAN] as $resolution) {
    try {
        new Gd\Tiff\WriteOptions(xResolution: $resolution);
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}

$palette = imagecreate(2, 2);
$writer = Gd\Tiff\Writer::toMemory();
try {
    $writer->addPage($palette);
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}

$stream = fopen('php://temp', 'w+b');
fclose($stream);
try {
    Gd\Tiff\Writer::toStream($stream);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    @Gd\Tiff\Writer::toFile(__DIR__ . '/missing-directory/output.tiff');
} catch (Gd\Codec\CodecException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
None:1:0:3:40x50
Lzw:5:0:3:40x50
AdobeDeflate:8:0:3:40x50
TIFFWriteDirectorySec: Warning, Creating TIFF with legacy Deflate codec identifier, COMPRESSION_ADOBE_DEFLATE is more widely supported.
Deflate:32946:0:3:40x50
PackBits:32773:0:3:40x50
CcittRle:Failed to initialize TIFF writer
CcittFax3:Failed to initialize TIFF writer
CcittFax4:Failed to initialize TIFF writer
Gd\Tiff\WriteOptions::__construct(): Argument #5 ($xResolution) must be finite and greater than 0
Gd\Tiff\WriteOptions::__construct(): Argument #5 ($xResolution) must be finite and greater than 0
Gd\Tiff\WriteOptions::__construct(): Argument #5 ($xResolution) must be finite and greater than 0
Gd\Tiff\WriteOptions::__construct(): Argument #5 ($xResolution) must be finite and greater than 0
TIFF writer only supports truecolor images
Gd\Tiff\Writer::toStream(): supplied resource is not a valid stream resource
Failed to open TIFF output
