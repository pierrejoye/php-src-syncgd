--TEST--
GdImage generic to* behavior
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Png\Codec::class)) die('skip Gd\Png\Codec not available');
?>
--FILE--
<?php
$failures = [];

function save_to_image(): GdImage {
    $im = imagecreatetruecolor(4, 3);
    imagefill($im, 0, 0, imagecolorallocate($im, 0x33, 0x66, 0x99));
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

$im = save_to_image();

record_failure($failures, 'png extension inference', function () use ($im) {
    $file = tempnam(sys_get_temp_dir(), 'gd-save-to-') . '.png';
    $im->toFile($file);
    $bytes = file_get_contents($file);
    unlink($file);
    return str_starts_with($bytes, "\x89PNG");
});

record_failure($failures, 'png explicit string', function () use ($im) {
    return str_starts_with($im->toString(Gd\Codec\Format::Png), "\x89PNG");
});

record_failure($failures, 'png stream', function () use ($im) {
    $stream = fopen('php://temp', 'w+b');
    $im->toStream($stream, Gd\Codec\Format::Png);
    rewind($stream);
    $bytes = stream_get_contents($stream);
    fclose($stream);
    return str_starts_with($bytes, "\x89PNG");
});

record_failure($failures, 'png output stream omitted', function () use ($im) {
    try {
        $im->toStream();
        return false;
    } catch (ArgumentCountError) {
        return true;
    }
});

record_failure($failures, 'png output stream null', function () use ($im) {
    try {
        $im->toStream(null, Gd\Codec\Format::Png);
        return false;
    } catch (TypeError) {
        return true;
    }
});

try {
    $im->toFile(sys_get_temp_dir() . '/gd-save-to.unknown-extension');
    $failures[] = 'unknown extension did not throw';
} catch (Gd\Codec\CodecException) {
}

if (class_exists(Gd\Jpeg\Codec::class)) {
    record_failure($failures, 'explicit jpeg ignores png extension', function () use ($im) {
        $file = tempnam(sys_get_temp_dir(), 'gd-save-to-') . '.png';
        $im->toFile($file, Gd\Codec\Format::Jpeg, new Gd\Jpeg\WriteOptions());
        $bytes = file_get_contents($file);
        unlink($file);
        return str_starts_with($bytes, "\xff\xd8");
    });

    record_failure($failures, 'format options mismatch throws', function () use ($im) {
        try {
            $im->toString(Gd\Codec\Format::Png, new Gd\Jpeg\WriteOptions());
            return false;
        } catch (Gd\Codec\CodecException) {
            return true;
        }
    });
}

if (class_exists(Gd\Gif\Codec::class)) {
    record_failure($failures, 'gif empty options', function () use ($im) {
        return str_starts_with($im->toString(Gd\Codec\Format::Gif, new Gd\Gif\WriteOptions()), 'GIF');
    });
}

if (class_exists(Gd\Qoi\Codec::class)) {
    record_failure($failures, 'qoi write options default', function () {
        return (new Gd\Qoi\WriteOptions())->colorspace === Gd\Qoi\Colorspace::SRGB;
    });
    record_failure($failures, 'qoi linear options', function () use ($im) {
        $bytes = $im->toString(Gd\Codec\Format::Qoi, new Gd\Qoi\WriteOptions(Gd\Qoi\Colorspace::Linear));
        return substr($bytes, 0, 4) === 'qoif' && ord($bytes[13]) === 1;
    });
}

var_dump($failures);
?>
--EXPECT--
array(0) {
}
