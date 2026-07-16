--TEST--
GD text font loading
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\FontFace::class)) {
    die("skip GD text API not available");
}
?>
--FILE--
<?php
$fontFile = __DIR__ . '/Tuffy.ttf';

$fromFile = Gd\FontFace::fromFile($fontFile);
var_dump($fromFile instanceof Gd\FontFace);

$fromString = Gd\FontFace::fromString(file_get_contents($fontFile));
var_dump($fromString instanceof Gd\FontFace);

foreach ([
    fn() => Gd\FontFace::fromFile(''),
    fn() => Gd\FontFace::fromFile("abc\0def"),
    fn() => Gd\FontFace::fromFile($fontFile, -1),
    fn() => Gd\FontFace::fromString(''),
    fn() => Gd\FontFace::fromString('not a font'),
    fn() => Gd\FontFace::fromString(file_get_contents($fontFile), -1),
] as $callback) {
    try {
        $callback();
    } catch (Throwable $e) {
        echo get_class($e), "\n";
    }
}
?>
--EXPECT--
bool(true)
bool(true)
ValueError
ValueError
ValueError
ValueError
Gd\Text\FontException
ValueError
