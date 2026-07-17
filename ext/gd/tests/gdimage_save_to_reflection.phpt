--TEST--
GdImage generic to* reflection and codec WriteOptions marker
--EXTENSIONS--
gd
--FILE--
<?php
$failures = [];

if (!interface_exists(Gd\Codec\WriteOptions::class)) {
    $failures[] = 'missing marker interface';
}
if (!enum_exists(Gd\Codec\Format::class)) {
    $failures[] = 'missing format enum';
} else {
    $cases = array_map(fn($case) => $case->name, Gd\Codec\Format::cases());
    foreach (['Png', 'Jpeg', 'Webp', 'Bmp', 'Avif', 'Heif', 'Jxl', 'Tiff', 'Gif', 'Qoi'] as $case) {
        if (!in_array($case, $cases, true)) {
            $failures[] = "missing format $case";
        }
    }
}

$image = new ReflectionClass(GdImage::class);
foreach (['toFile', 'toStream', 'toString'] as $name) {
    if (!$image->hasMethod($name)) {
        $failures[] = "missing GdImage::$name";
    }
}

foreach (['saveTo', 'saveToStream', 'saveToString'] as $name) {
    if ($image->hasMethod($name)) {
        $failures[] = "unexpected GdImage::$name";
    }
}

if ($image->hasMethod('toFile')) {
    $method = $image->getMethod('toFile');
    $params = $method->getParameters();
    if ((string) $params[0]->getType() !== 'string') {
        $failures[] = 'toFile path type';
    }
    if ((string) $params[1]->getType() !== '?' . Gd\Codec\Format::class) {
        $failures[] = 'toFile format type';
    }
    if (!$params[1]->isDefaultValueAvailable() || $params[1]->getDefaultValue() !== null) {
        $failures[] = 'toFile format default';
    }
    if ((string) $params[2]->getType() !== '?' . Gd\Codec\WriteOptions::class) {
        $failures[] = 'toFile options type';
    }
    if (!$params[2]->isDefaultValueAvailable() || $params[2]->getDefaultValue() !== null) {
        $failures[] = 'toFile options default';
    }
}

if ($image->hasMethod('toStream')) {
    $method = $image->getMethod('toStream');
    $params = $method->getParameters();
    if ($params[0]->isDefaultValueAvailable()) {
        $failures[] = 'toStream stream required';
    }
    if ((string) $params[1]->getType() !== '?' . Gd\Codec\Format::class) {
        $failures[] = 'toStream format type';
    }
    if (!$params[1]->isDefaultValueAvailable() || $params[1]->getDefaultValue() !== null) {
        $failures[] = 'toStream format default';
    }
    if ((string) $params[2]->getType() !== '?' . Gd\Codec\WriteOptions::class) {
        $failures[] = 'toStream options type';
    }
    if (!$params[2]->isDefaultValueAvailable() || $params[2]->getDefaultValue() !== null) {
        $failures[] = 'toStream options default';
    }
}

if ($image->hasMethod('toString')) {
    $method = $image->getMethod('toString');
    $params = $method->getParameters();
    if ((string) $params[0]->getType() !== Gd\Codec\Format::class) {
        $failures[] = 'toString format type';
    }
    if ((string) $params[1]->getType() !== '?' . Gd\Codec\WriteOptions::class) {
        $failures[] = 'toString options type';
    }
    if ((string) $method->getReturnType() !== 'string') {
        $failures[] = 'toString return type';
    }
}

$singleImageOptions = [
    Gd\Png\WriteOptions::class,
    Gd\Jpeg\WriteOptions::class,
    Gd\Webp\WriteOptions::class,
    Gd\Bmp\WriteOptions::class,
    Gd\Avif\WriteOptions::class,
    Gd\Heif\WriteOptions::class,
    Gd\Jxl\WriteOptions::class,
    Gd\Tiff\WriteOptions::class,
    Gd\Gif\WriteOptions::class,
    Gd\Qoi\WriteOptions::class,
];

foreach ($singleImageOptions as $class) {
    if (class_exists($class) && !is_subclass_of($class, Gd\Codec\WriteOptions::class)) {
        $failures[] = "$class does not implement marker";
    }
}

$nonSingleImageOptions = [
    Gd\Webp\AnimWriteOptions::class,
    Gd\Jxl\AnimWriteOptions::class,
];

foreach ($nonSingleImageOptions as $class) {
    if (class_exists($class) && is_subclass_of($class, Gd\Codec\WriteOptions::class)) {
        $failures[] = "$class implements marker";
    }
}

var_dump($failures);
?>
--EXPECT--
array(0) {
}
