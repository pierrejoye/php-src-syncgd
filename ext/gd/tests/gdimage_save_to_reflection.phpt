--TEST--
GdImage generic saveTo* reflection and codec WriteOptions marker
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
foreach (['saveTo', 'saveToStream', 'saveToString'] as $name) {
    if (!$image->hasMethod($name)) {
        $failures[] = "missing GdImage::$name";
    }
}

if ($image->hasMethod('saveTo')) {
    $method = $image->getMethod('saveTo');
    $params = $method->getParameters();
    if ((string) $params[0]->getType() !== 'string') {
        $failures[] = 'saveTo path type';
    }
    if ((string) $params[1]->getType() !== '?' . Gd\Codec\Format::class) {
        $failures[] = 'saveTo format type';
    }
    if (!$params[1]->isDefaultValueAvailable() || $params[1]->getDefaultValue() !== null) {
        $failures[] = 'saveTo format default';
    }
    if ((string) $params[2]->getType() !== '?' . Gd\Codec\WriteOptions::class) {
        $failures[] = 'saveTo options type';
    }
    if (!$params[2]->isDefaultValueAvailable() || $params[2]->getDefaultValue() !== null) {
        $failures[] = 'saveTo options default';
    }
}

if ($image->hasMethod('saveToStream')) {
    $method = $image->getMethod('saveToStream');
    $params = $method->getParameters();
    if (!$params[0]->isDefaultValueAvailable() || $params[0]->getDefaultValue() !== null) {
        $failures[] = 'saveToStream stream default';
    }
    if ((string) $params[1]->getType() !== '?' . Gd\Codec\Format::class) {
        $failures[] = 'saveToStream format type';
    }
    if (!$params[1]->isDefaultValueAvailable() || $params[1]->getDefaultValue() !== null) {
        $failures[] = 'saveToStream format default';
    }
    if ((string) $params[2]->getType() !== '?' . Gd\Codec\WriteOptions::class) {
        $failures[] = 'saveToStream options type';
    }
    if (!$params[2]->isDefaultValueAvailable() || $params[2]->getDefaultValue() !== null) {
        $failures[] = 'saveToStream options default';
    }
}

if ($image->hasMethod('saveToString')) {
    $method = $image->getMethod('saveToString');
    $params = $method->getParameters();
    if ((string) $params[0]->getType() !== Gd\Codec\Format::class) {
        $failures[] = 'saveToString format type';
    }
    if ((string) $params[1]->getType() !== '?' . Gd\Codec\WriteOptions::class) {
        $failures[] = 'saveToString options type';
    }
    if ((string) $method->getReturnType() !== 'string') {
        $failures[] = 'saveToString return type';
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
