--TEST--
Gd\Image generic loadFrom* behavior
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Codec::class)) die('skip Gd\Png\Codec not available');
?>
--XLEAK--
--FILE--
<?php
$failures = [];

function load_test_image(): GdImage {
    $im = imagecreatetruecolor(64, 64);
    imagefill($im, 0, 0, imagecolorallocate($im, 0x22, 0x66, 0xaa));
    return $im;
}

function record_failure(array &$failures, string $name, callable $callback): void {
    try {
        if (!$callback()) {
            $failures[] = $name;
        }
    } catch (Throwable $e) {
        $failures[] = $name . ': ' . $e::class . ': ' . $e->getMessage();
    }
}

function same_size(GdImage $im): bool {
    return imagesx($im) === 64 && imagesy($im) === 64;
}

$im = load_test_image();
$png = Gd\Png\Codec::toString($im);

record_failure($failures, 'png from string', function () use ($png) {
    return same_size(Gd\Image::loadFromString($png));
});

record_failure($failures, 'png from stream', function () use ($png) {
    $stream = fopen('php://temp', 'w+b');
    fwrite($stream, $png);
    rewind($stream);
    $loaded = Gd\Image::loadFromStream($stream);
    fclose($stream);
    return same_size($loaded);
});

record_failure($failures, 'png from mismatched extension', function () use ($png) {
    $file = tempnam(sys_get_temp_dir(), 'gd-load-from-') . '.jpg';
    file_put_contents($file, $png);
    $loaded = Gd\Image::loadFromFile($file);
    unlink($file);
    return same_size($loaded);
});

record_failure($failures, 'invalid bytes throw', function () {
    try {
        Gd\Image::loadFromString('not an image');
        return false;
    } catch (Gd\Codec\CodecException $e) {
        return $e->getMessage() === 'Unrecognized image format';
    }
});

record_failure($failures, 'legacy gd signature rejected', function () {
    try {
        Gd\Image::loadFromString("\xff\xfe");
        return false;
    } catch (Gd\Codec\CodecException $e) {
        return $e->getMessage() === 'Image format is not supported by Gd\\Image::loadFrom*()';
    }
});

$formats = [
    [Gd\Codec\Format::Png, Gd\Png\Codec::class],
    [Gd\Codec\Format::Jpeg, Gd\Jpeg\Codec::class],
    [Gd\Codec\Format::Webp, Gd\Webp\Codec::class],
    [Gd\Codec\Format::Bmp, Gd\Bmp\Codec::class],
    [Gd\Codec\Format::Avif, Gd\Avif\Codec::class],
    [Gd\Codec\Format::Heif, Gd\Heif\Codec::class],
    [Gd\Codec\Format::Jxl, Gd\Jxl\Codec::class],
    [Gd\Codec\Format::Tiff, Gd\Tiff\Codec::class],
    [Gd\Codec\Format::Gif, Gd\Gif\Codec::class],
    [Gd\Codec\Format::Qoi, Gd\Qoi\Codec::class],
];

foreach ($formats as [$format, $codec]) {
    if (!class_exists($codec)) {
        continue;
    }
    record_failure($failures, $format->name . ' smoke', function () use ($im, $format) {
        $options = $format === Gd\Codec\Format::Tiff
            ? new Gd\Tiff\WriteOptions(Gd\Tiff\Compression::None)
            : null;
        $bytes = $im->toString($format, $options);
        return same_size(Gd\Image::loadFromString($bytes));
    });
}

var_dump($failures);
?>
--EXPECT--
array(0) {
}
