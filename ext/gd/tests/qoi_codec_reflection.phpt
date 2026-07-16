--TEST--
Gd\Qoi\Codec and Gd\Qoi\Colorspace reflection
--SKIPIF--
<?php
if (!class_exists('Gd\Qoi\Codec')) die('SKIP: Gd\Qoi\Codec not available');
if (!enum_exists('Gd\Qoi\Colorspace')) die('SKIP: Gd\Qoi\Colorspace not available');
?>
--FILE--
<?php
// Test Gd\Qoi\Colorspace enum
echo "Gd\\Qoi\\Colorspace enum tests:\n";

$reflect_enum = new ReflectionEnum('Gd\Qoi\Colorspace');
echo "  Cases: ";
$cases = $reflect_enum->getCases();
var_dump(count($cases) === 2);

echo "  Has SRGB case: ";
var_dump(in_array('SRGB', array_column($cases, 'name')));

echo "  Has Linear case: ";
var_dump(in_array('Linear', array_column($cases, 'name')));

echo "  Is Backed: ";
var_dump($reflect_enum->isBacked());

// Test Gd\Qoi\Codec class
echo "\nGd\\Qoi\\Codec class tests:\n";

$reflect_class = new ReflectionClass('Gd\Qoi\Codec');
echo "  Is final: ";
var_dump($reflect_class->isFinal());

echo "  Method count: ";
$methods = $reflect_class->getMethods();
var_dump(count($methods) >= 6);

$method_names = array_map(fn($m) => $m->name, $methods);
echo "  Has fromFile: ";
var_dump(in_array('fromFile', $method_names));

echo "  Has fromStream: ";
var_dump(in_array('fromStream', $method_names));

echo "  Has fromString: ";
var_dump(in_array('fromString', $method_names));

echo "  Has toFile: ";
var_dump(in_array('toFile', $method_names));

echo "  Has toStream: ";
var_dump(in_array('toStream', $method_names));

echo "  Has toString: ";
var_dump(in_array('toString', $method_names));

// Test that __construct is private
echo "  __construct is private: ";
$construct = $reflect_class->getConstructor();
var_dump($construct === null || $construct->isPrivate());

// Test method signatures
echo "\nMethod signature tests:\n";

echo "  fromFile is static: ";
$fromFile = $reflect_class->getMethod('fromFile');
var_dump($fromFile->isStatic());

echo "  toFile is static: ";
$toFile = $reflect_class->getMethod('toFile');
var_dump($toFile->isStatic());

echo "All reflection tests passed!\n";
?>
--EXPECT--
Gd\Qoi\Colorspace enum tests:
  Cases: bool(true)
  Has SRGB case: bool(true)
  Has Linear case: bool(true)
  Is Backed: bool(false)

Gd\Qoi\Codec class tests:
  Is final: bool(true)
  Method count: bool(true)
  Has fromFile: bool(true)
  Has fromStream: bool(true)
  Has fromString: bool(true)
  Has toFile: bool(true)
  Has toStream: bool(true)
  Has toString: bool(true)
  __construct is private: bool(true)

Method signature tests:
  fromFile is static: bool(true)
  toFile is static: bool(true)
All reflection tests passed!
