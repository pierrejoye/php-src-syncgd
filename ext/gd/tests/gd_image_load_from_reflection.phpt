--TEST--
Gd\Image generic loadFrom* reflection
--EXTENSIONS--
gd
--FILE--
<?php
$failures = [];

$class = new ReflectionClass(Gd\Image::class);

foreach (['loadFromFile', 'loadFromStream', 'loadFromString'] as $name) {
    if (!$class->hasMethod($name)) {
        $failures[] = "missing Gd\\Image::$name";
        continue;
    }

    $method = $class->getMethod($name);
    if (!$method->isStatic()) {
        $failures[] = "$name is not static";
    }
    if ((string) $method->getReturnType() !== GdImage::class) {
        $failures[] = "$name return type";
    }
    if (count($method->getParameters()) !== 1) {
        $failures[] = "$name parameter count";
    }
}

if ($class->hasMethod('loadFromFile')) {
    $param = $class->getMethod('loadFromFile')->getParameters()[0];
    if ((string) $param->getType() !== 'string') {
        $failures[] = 'loadFromFile path type';
    }
}

if ($class->hasMethod('loadFromString')) {
    $param = $class->getMethod('loadFromString')->getParameters()[0];
    if ((string) $param->getType() !== 'string') {
        $failures[] = 'loadFromString bytes type';
    }
}

var_dump($failures);
?>
--EXPECT--
array(0) {
}
