--TEST--
Gd\Context explicit image synchronization
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
imagefill($image, 0, 0, 0x000000);

$ctx = $image->getContext();
$ctx->setSourceRgb(1, 0, 0)->paint();
var_dump(imagecolorat($image, 0, 0));
$ctx->flushImage();
var_dump(imagecolorat($image, 0, 0));

imagefilledrectangle($image, 0, 0, 1, 1, 0x00ff00);
$ctx->setSourceRgb(0, 0, 1)->paint()->flushImage();
var_dump(imagecolorat($image, 0, 0));

imagefilledrectangle($image, 0, 0, 1, 1, 0x00ff00);
$ctx->reloadImage();
$ctx->flushImage();
var_dump(imagecolorat($image, 0, 0));

$discard = imagecreatetruecolor(1, 1);
imagefill($discard, 0, 0, 0x000000);
$discardCtx = $discard->getContext();
$discardCtx->setSourceRgb(1, 0, 0)->paint();
$discardCtx->destroy(false);
var_dump(imagecolorat($discard, 0, 0));

$commit = imagecreatetruecolor(1, 1);
imagefill($commit, 0, 0, 0x000000);
$commitCtx = $commit->getContext();
$commitCtx->setSourceRgb(1, 0, 0)->paint();
$commitCtx->destroy(true);
var_dump(imagecolorat($commit, 0, 0));

$gc = imagecreatetruecolor(1, 1);
imagefill($gc, 0, 0, 0x000000);
(function (GdImage $image): void {
    $ctx = $image->getContext();
    $ctx->setSourceRgb(1, 0, 0)->paint();
})($gc);
gc_collect_cycles();
var_dump(imagecolorat($gc, 0, 0));
?>
--EXPECT--
int(0)
int(16711680)
int(255)
int(65280)
int(0)
int(16711680)
int(0)
