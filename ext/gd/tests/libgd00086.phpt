--TEST--
libgd #86 (Possible infinite loop in imagecreatefrompng)
--EXTENSIONS--
gd
--SKIPIF--
<?php
    if (!GD_BUNDLED) die("skip requires bundled GD library\n");
?>
--FILE--
<?php

$warnings = 0;
set_error_handler(function (int $errno, string $errstr) use (&$warnings): bool {
    if ($errno === E_WARNING) {
        $warnings++;
        return true;
    }
    return false;
});

$im = imagecreatefrompng(__DIR__ . '/libgd00086.png');
restore_error_handler();

var_dump($im);
var_dump($warnings > 0);
?>
--EXPECT--
bool(false)
bool(true)
