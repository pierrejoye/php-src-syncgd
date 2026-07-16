<?php
if (!method_exists(GdImage::class, "perceptualDiff")) {
	die("skip GdImage::perceptualDiff() not available.");
}

$image = imagecreatetruecolor(1, 1);
try {
	$image->perceptualDiff($image, 0.0);
} catch (Error $e) {
	if (str_contains($e->getMessage(), "not supported by this GD library")) {
		die("skip GdImage::perceptualDiff() not supported by this GD library.");
	}
	throw $e;
}
