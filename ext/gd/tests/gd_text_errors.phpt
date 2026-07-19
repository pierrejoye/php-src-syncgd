--TEST--
GD text errors
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
$image = imagecreatetruecolor(20, 20);
$ctx = $image->getContext();

try {
    $ctx->textExtents('Hi');
} catch (Throwable $e) {
    echo get_class($e), "\n";
    var_dump($e instanceof Gd\GdException);
}

$font = Gd\FontFace::fromFile(__DIR__ . '/Tuffy.ttf');
$ctx->setFontFace($font)->setFontSize(12);

foreach ([
    fn() => $ctx->setFontSize(0),
    fn() => $ctx->setFontSize(INF),
    fn() => new Gd\TextOptions(Gd\TextShaping::None, 0),
    fn() => new Gd\TextOptions(Gd\TextShaping::None, NAN),
    fn() => $ctx->showText('Hi', INF, 0),
    fn() => $ctx->textPath('Hi', 0, NAN),
] as $callback) {
    try {
        $callback();
    } catch (Throwable $e) {
        echo get_class($e), "\n";
    }
}

try {
    $ctx->textExtents("\xff");
} catch (Throwable $e) {
    echo get_class($e), "\n";
}
?>
--EXPECT--
Gd\Text\InvalidTextException
bool(true)
ValueError
ValueError
ValueError
ValueError
ValueError
ValueError
Gd\Text\InvalidTextException
