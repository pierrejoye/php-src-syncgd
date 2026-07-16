--TEST--
GD text font lifetime
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
function count_ink(GdImage $image): int {
    $count = 0;
    for ($y = 0; $y < imagesy($image); $y++) {
        for ($x = 0; $x < imagesx($image); $x++) {
            if ((imagecolorat($image, $x, $y) & 0xffffff) !== 0xffffff) {
                $count++;
            }
        }
    }
    return $count;
}

$bytes = file_get_contents(__DIR__ . '/Tuffy.ttf');
$font = Gd\FontFace::fromString($bytes);
unset($bytes);
gc_collect_cycles();

$image = imagecreatetruecolor(140, 70);
imagefill($image, 0, 0, 0xffffff);
$ctx = $image->getContext()
    ->setSourceRgb(0, 0, 0)
    ->setFontFace($font)
    ->setFontSize(24);

unset($font);
gc_collect_cycles();

$ctx->showText('Hi', 10, 45)->flushImage();
var_dump(count_ink($image) > 0);

unset($ctx, $image);
gc_collect_cycles();
echo "done\n";
?>
--EXPECT--
bool(true)
done
