--TEST--
GdImage::scale() validation errors
--EXTENSIONS--
gd
--SKIPIF--
<?php
require (getenv("TEST_PHP_SRCDIR") ?: getcwd()) . "/ext/gd/tests/skipif_gd_scale_supported.inc";
if (!method_exists(GdImage::class, "scale")) {
    die("skip GdImage::scale() not available.");
}
?>
--FILE--
<?php
$image = imagecreatetruecolor(4, 4);

foreach ([
    fn() => $image->scale(),
    fn() => $image->scale(0, 1),
    fn() => $image->scale(1, 0),
    fn() => new Gd\ScaleOptions(width: 0),
	fn() => new Gd\ScaleOptions(height: 0),
	fn() => new Gd\ScaleOptions(backgroundColor: -1),
	fn() => new Gd\ScaleOptions(backgroundColor: 0x80000000),
	fn() => $image->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Contain, strategy: Gd\ScaleStrategy::Entropy)),
	fn() => $image->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Fill, strategy: Gd\ScaleStrategy::Entropy)),
	fn() => $image->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Contain, strategy: Gd\ScaleStrategy::Attention)),
	fn() => $image->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Inside, strategy: Gd\ScaleStrategy::Entropy)),
	fn() => $image->scale(4, 4, new Gd\ScaleOptions(fit: Gd\ScaleFit::Outside, strategy: Gd\ScaleStrategy::Attention)),
] as $callback) {
    try {
        $callback();
    } catch (ValueError $e) {
        echo $e->getMessage(), "\n";
    }
}
?>
--EXPECT--
GdImage::scale(): Argument #1 ($width) and argument #2 ($height) cannot both be null
GdImage::scale(): Argument #1 ($width) must be between 1 and 2147483647
GdImage::scale(): Argument #2 ($height) must be between 1 and 2147483647
Gd\ScaleOptions::__construct(): Argument #1 ($width) must be between 1 and 2147483647
Gd\ScaleOptions::__construct(): Argument #2 ($height) must be between 1 and 2147483647
Gd\ScaleOptions::__construct(): Argument #6 ($backgroundColor) must be a valid GD truecolor alpha color
Gd\ScaleOptions::__construct(): Argument #6 ($backgroundColor) must be a valid GD truecolor alpha color
GdImage::scale(): Gd\ScaleStrategy can only be used with Gd\ScaleFit::Cover
GdImage::scale(): Gd\ScaleStrategy can only be used with Gd\ScaleFit::Cover
GdImage::scale(): Gd\ScaleStrategy can only be used with Gd\ScaleFit::Cover
GdImage::scale(): Gd\ScaleStrategy can only be used with Gd\ScaleFit::Cover
GdImage::scale(): Gd\ScaleStrategy can only be used with Gd\ScaleFit::Cover
