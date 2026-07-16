<?php
if (!method_exists(GdImage::class, "scale")) {
    die("skip GdImage::scale() not available.");
}

$image = Gd\Image::create(1, 1);
try {
    $image->scale(1, 1);
} catch (Error $e) {
    if (str_contains($e->getMessage(), "Options-based scale is not supported")) {
        die("skip GdImage::scale() not supported by this GD library.");
    }
    throw $e;
}
?>
