--TEST--
Gd 2D API reflection surface
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class) || !class_exists(Gd\LinearGradient::class)) {
    die("skip Gd 2D API not available.");
}
?>
--FILE--
<?php
function assert_true(bool $condition, string $message): void {
    if (!$condition) {
        throw new Exception($message);
    }
}

function assert_same(mixed $expected, mixed $actual, string $message): void {
    if ($expected !== $actual) {
        throw new Exception($message . "\nExpected: " . var_export($expected, true) . "\nActual: " . var_export($actual, true));
    }
}

function type_name(?ReflectionType $type): ?string {
    if ($type === null) {
        return null;
    }
    return ($type->allowsNull() ? '?' : '') . $type->getName();
}

function method_signature(string $class, string $method): array {
    $reflection = new ReflectionMethod($class, $method);
    $parameters = [];
    foreach ($reflection->getParameters() as $parameter) {
        $parameters[] = [
            'name' => $parameter->getName(),
            'type' => type_name($parameter->getType()),
            'optional' => $parameter->isOptional(),
            'default' => $parameter->isDefaultValueAvailable() ? $parameter->getDefaultValue() : null,
        ];
    }
    return [
        'static' => $reflection->isStatic(),
        'public' => $reflection->isPublic(),
        'private' => $reflection->isPrivate(),
        'return' => type_name($reflection->getReturnType()),
        'parameters' => $parameters,
    ];
}

assert_same(
    ['None', 'Repeat', 'Reflect', 'Pad'],
    array_map(fn($case) => $case->name, Gd\ExtendMode::cases()),
    'ExtendMode cases changed'
);
assert_same(
    ['Butt', 'Round', 'Square'],
    array_map(fn($case) => $case->name, Gd\LineCap::cases()),
    'LineCap cases changed'
);
assert_same(
    ['Miter', 'Round', 'Bevel'],
    array_map(fn($case) => $case->name, Gd\LineJoin::cases()),
    'LineJoin cases changed'
);
assert_same(
    ['NonZero', 'EvenOdd'],
    array_map(fn($case) => $case->name, Gd\FillRule::cases()),
    'FillRule cases changed'
);
assert_same(
    [
        'Clear', 'Source', 'Over', 'In', 'Out', 'Atop',
        'Dest', 'DestOver', 'DestIn', 'DestOut', 'DestAtop', 'Xor',
        'Add', 'Saturate', 'Multiply', 'Screen', 'Overlay', 'Darken',
        'Lighten', 'ColorDodge', 'ColorBurn', 'HardLight', 'SoftLight', 'Difference',
        'Exclusion', 'HslHue', 'HslSaturation', 'HslColor', 'HslLuminosity',
    ],
    array_map(fn($case) => $case->name, Gd\CompositeOperator::cases()),
    'CompositeOperator cases changed'
);

$paint = new ReflectionClass(Gd\Paint::class);
$gradient = new ReflectionClass(Gd\Gradient::class);
$linear = new ReflectionClass(Gd\LinearGradient::class);
$radial = new ReflectionClass(Gd\RadialGradient::class);
$pattern = new ReflectionClass(Gd\Pattern::class);
$context = new ReflectionClass(Gd\Context::class);
$matrix = new ReflectionClass(Gd\Matrix::class);
$rect = new ReflectionClass(Gd\Rect::class);
$path = new ReflectionClass(Gd\Path::class);

assert_true($paint->isAbstract() && !$paint->isInstantiable(), 'Paint must be abstract');
assert_true($gradient->isAbstract() && $gradient->isSubclassOf(Gd\Paint::class), 'Gradient hierarchy changed');
assert_true($linear->isFinal() && $linear->isSubclassOf(Gd\Gradient::class) && $linear->isInstantiable(), 'LinearGradient hierarchy changed');
assert_true($radial->isFinal() && $radial->isSubclassOf(Gd\Gradient::class) && $radial->isInstantiable(), 'RadialGradient hierarchy changed');
assert_true($pattern->isFinal() && $pattern->isSubclassOf(Gd\Paint::class) && !$pattern->isInstantiable(), 'Pattern hierarchy changed');
assert_true($context->isFinal() && !$context->isInstantiable(), 'Context construction changed');
assert_true($matrix->isFinal() && $matrix->isInstantiable(), 'Matrix construction changed');
assert_true($rect->isFinal() && $rect->isInstantiable(), 'Rect construction changed');
assert_true($path->isFinal() && $path->isInstantiable(), 'Path construction changed');

foreach (['x', 'y', 'width', 'height'] as $property) {
    $prop = $rect->getProperty($property);
    assert_true($prop->isPublic() && $prop->isReadOnly(), "Rect::\$$property must be public readonly");
    assert_same('float', type_name($prop->getType()), "Rect::\$$property type changed");
}

assert_true($matrix->hasMethod('shearing'), 'Matrix::shearing() missing');
assert_true(!$matrix->hasMethod('shear') || !(new ReflectionMethod(Gd\Matrix::class, 'shear'))->isStatic(), 'Matrix::shear() must not be a static factory');
assert_true($path->hasMethod('relMoveTo') && $path->hasMethod('relCurveTo') && $path->hasMethod('relQuadTo'), 'Relative path methods missing');
assert_true($context->hasMethod('relMoveTo') && $context->hasMethod('relCurveTo') && $context->hasMethod('relQuadTo'), 'Relative context path methods missing');

assert_same([
    'static' => true,
    'public' => true,
    'private' => false,
    'return' => 'Gd\Context',
    'parameters' => [
        ['name' => 'image', 'type' => 'GdImage', 'optional' => false, 'default' => null],
    ],
], method_signature(Gd\Context::class, 'createForImage'), 'Context::createForImage signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'void',
    'parameters' => [],
], method_signature(Gd\Context::class, 'flushImage'), 'Context::flushImage signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'void',
    'parameters' => [],
], method_signature(Gd\Context::class, 'reloadImage'), 'Context::reloadImage signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'void',
    'parameters' => [
        ['name' => 'flush', 'type' => 'bool', 'optional' => true, 'default' => true],
    ],
], method_signature(Gd\Context::class, 'destroy'), 'Context::destroy signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'Gd\Context',
    'parameters' => [
        ['name' => 'image', 'type' => 'GdImage', 'optional' => false, 'default' => null],
        ['name' => 'x', 'type' => 'float', 'optional' => true, 'default' => 0],
        ['name' => 'y', 'type' => 'float', 'optional' => true, 'default' => 0],
    ],
], method_signature(Gd\Context::class, 'setSourceImage'), 'Context::setSourceImage signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'Gd\Pattern',
    'parameters' => [
        ['name' => 'image', 'type' => 'GdImage', 'optional' => false, 'default' => null],
    ],
], method_signature(Gd\Context::class, 'createPattern'), 'Context::createPattern signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'Gd\Context',
    'parameters' => [
        ['name' => 'offset', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'pattern', 'type' => 'array', 'optional' => false, 'default' => null],
    ],
], method_signature(Gd\Context::class, 'setDash'), 'Context::setDash signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'Gd\Gradient',
    'parameters' => [
        ['name' => 'offset', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'r', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'g', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'b', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'a', 'type' => 'float', 'optional' => false, 'default' => null],
    ],
], method_signature(Gd\Gradient::class, 'addColorStopRgba'), 'Gradient::addColorStopRgba signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => null,
    'parameters' => [
        ['name' => 'x0', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'y0', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'x1', 'type' => 'float', 'optional' => false, 'default' => null],
        ['name' => 'y1', 'type' => 'float', 'optional' => false, 'default' => null],
    ],
], method_signature(Gd\LinearGradient::class, '__construct'), 'LinearGradient::__construct signature changed');

assert_same([
    'static' => false,
    'public' => true,
    'private' => false,
    'return' => 'Gd\Pattern',
    'parameters' => [
        ['name' => 'opacity', 'type' => 'float', 'optional' => false, 'default' => null],
    ],
], method_signature(Gd\Pattern::class, 'setOpacity'), 'Pattern::setOpacity signature changed');

$getContext = method_signature(GdImage::class, 'getContext');
assert_same('Gd\Context', $getContext['return'], 'GdImage::getContext return type changed');

echo "done\n";
?>
--EXPECT--
done
