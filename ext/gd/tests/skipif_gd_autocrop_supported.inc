<?php
if (!method_exists(GdImage::class, "autoCrop")) {
    die("skip GdImage::autoCrop() not available.");
}

$image = Gd\Image::create(1, 1);
try {
    $image->autoCrop();
} catch (Error $e) {
    if (str_contains($e->getMessage(), "Options-based autocrop is not supported")) {
        die("skip GdImage::autoCrop() not supported by this GD library.");
    }
    throw $e;
}
?>
