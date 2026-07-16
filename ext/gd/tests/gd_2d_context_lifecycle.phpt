--TEST--
Gd\Context image ownership and lifecycle
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Context::class)) {
    die("skip Gd\\Context not available.");
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(2, 2);
$ctx = $image->getContext();
$ctx2 = Gd\Context::createForImage($image);

var_dump($ctx instanceof Gd\Context);
var_dump($ctx2 instanceof Gd\Context);
var_dump($ctx === $ctx2);
var_dump($ctx->getImage() === $image);

unset($image);
gc_collect_cycles();

var_dump($ctx->getImage() instanceof GdImage);
$ctx->reloadImage();
$ctx->flushImage();
echo "sync ok\n";

$ctx->destroy(false);
try {
    $ctx->reloadImage();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}

$palette = imagecreate(1, 1);
try {
    Gd\Context::createForImage($palette);
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
sync ok
Gd\Context has already been destroyed
Gd\Context requires a truecolor GdImage
