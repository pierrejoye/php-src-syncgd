--TEST--
GdImage::scale() cover mode gravity
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
function row_source(): GdImage {
    $image = imagecreatetruecolor(4, 4);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    $rows = [
        imagecolorallocatealpha($image, 255, 0, 0, 0),
        imagecolorallocatealpha($image, 0, 255, 0, 0),
        imagecolorallocatealpha($image, 0, 0, 255, 0),
        imagecolorallocatealpha($image, 255, 255, 255, 0),
    ];
    foreach ($rows as $y => $color) {
        imagefilledrectangle($image, 0, $y, 3, $y, $color);
    }
    return $image;
}

function column_source(): GdImage {
    $image = imagecreatetruecolor(4, 4);
    imagealphablending($image, false);
    imagesavealpha($image, true);
    $columns = [
        imagecolorallocatealpha($image, 255, 0, 0, 0),
        imagecolorallocatealpha($image, 0, 255, 0, 0),
        imagecolorallocatealpha($image, 0, 0, 255, 0),
        imagecolorallocatealpha($image, 255, 255, 255, 0),
    ];
    foreach ($columns as $x => $color) {
        imagefilledrectangle($image, $x, 0, $x, 3, $color);
    }
    return $image;
}

function show_vertical(Gd\ScaleGravity $gravity): void {
    $scaled = row_source()->scale(4, 2, new Gd\ScaleOptions(gravity: $gravity, interpolation: Gd\InterpolationMethod::NearestNeighbour));
    printf(
        "%s rows %08X %08X\n",
        $gravity->name,
        imagecolorat($scaled, 0, 0),
        imagecolorat($scaled, 0, 1),
    );
}

function show_horizontal(Gd\ScaleGravity $gravity): void {
    $scaled = column_source()->scale(2, 4, new Gd\ScaleOptions(gravity: $gravity, interpolation: Gd\InterpolationMethod::NearestNeighbour));
    printf(
        "%s columns %08X %08X\n",
        $gravity->name,
        imagecolorat($scaled, 0, 0),
        imagecolorat($scaled, 1, 0),
    );
}

foreach ([Gd\ScaleGravity::Northwest, Gd\ScaleGravity::North, Gd\ScaleGravity::Northeast] as $gravity) {
    show_vertical($gravity);
}
foreach ([Gd\ScaleGravity::West, Gd\ScaleGravity::Center, Gd\ScaleGravity::East] as $gravity) {
    show_vertical($gravity);
}
foreach ([Gd\ScaleGravity::Southwest, Gd\ScaleGravity::South, Gd\ScaleGravity::Southeast] as $gravity) {
    show_vertical($gravity);
}

foreach ([Gd\ScaleGravity::Northwest, Gd\ScaleGravity::West, Gd\ScaleGravity::Southwest] as $gravity) {
    show_horizontal($gravity);
}
foreach ([Gd\ScaleGravity::North, Gd\ScaleGravity::Center, Gd\ScaleGravity::South] as $gravity) {
    show_horizontal($gravity);
}
foreach ([Gd\ScaleGravity::Northeast, Gd\ScaleGravity::East, Gd\ScaleGravity::Southeast] as $gravity) {
    show_horizontal($gravity);
}
?>
--EXPECT--
Northwest rows 00FF0000 0000FF00
North rows 00FF0000 0000FF00
Northeast rows 00FF0000 0000FF00
West rows 0000FF00 000000FF
Center rows 0000FF00 000000FF
East rows 0000FF00 000000FF
Southwest rows 000000FF 00FFFFFF
South rows 000000FF 00FFFFFF
Southeast rows 000000FF 00FFFFFF
Northwest columns 00FF0000 0000FF00
West columns 00FF0000 0000FF00
Southwest columns 00FF0000 0000FF00
North columns 0000FF00 000000FF
Center columns 0000FF00 000000FF
South columns 0000FF00 000000FF
Northeast columns 000000FF 00FFFFFF
East columns 000000FF 00FFFFFF
Southeast columns 000000FF 00FFFFFF
