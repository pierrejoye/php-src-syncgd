--TEST--
Gd\Tiff\Reader reads independent multipage images and metadata
--EXTENSIONS--
gd
--SKIPIF--
<?php
if (!class_exists(Gd\Tiff\Reader::class)) die('skip TIFF reader API unavailable');
?>
--FILE--
<?php
function tiff_patch_short_tag(string $data, int $tag, int $value): string
{
    $little = substr($data, 0, 2) === "II";
    $u16 = static fn (int $offset): int => unpack($little ? 'v' : 'n', substr($data, $offset, 2))[1];
    $u32 = static fn (int $offset): int => unpack($little ? 'V' : 'N', substr($data, $offset, 4))[1];

    $ifdOffset = $u32(4);
    $count = $u16($ifdOffset);
    for ($i = 0; $i < $count; $i++) {
        $entry = $ifdOffset + 2 + $i * 12;
        if ($u16($entry) === $tag) {
            $data = substr_replace($data, pack($little ? 'v' : 'n', $value), $entry + 8, 2);
            return $data;
        }
    }
    throw new Exception("tag not found");
}

$first = imagecreatetruecolor(2, 2);
$second = imagecreatetruecolor(3, 1);
imagesetpixel($first, 0, 0, 0xff0000);
imagesetpixel($second, 0, 0, 0x00ff00);

$writer = Gd\Tiff\Writer::toMemory(new Gd\Tiff\WriteOptions(
    compression: Gd\Tiff\Compression::None,
    colorSpace: Gd\Tiff\ColorSpace::Rgb,
));
$bytes = $writer->addPage($first)->addPage($second)->finish();

$reader = Gd\Tiff\Reader::fromString($bytes);
$info = $reader->info();
var_dump($info->width, $info->height, $info->pageCount);
var_dump($info->compressionTag, $info->compression, $info->photometricTag, $info->photometric, $info->resolutionUnit);
var_dump(property_exists($info, 'colorSpace'));

$page0 = $reader->next();
$page1 = $reader->next();
var_dump($page0->pageIndex, $page0->width, $page0->height);
var_dump($page1->pageIndex, $page1->width, $page1->height);
var_dump($page0->compressionTag, $page0->compression, $page0->photometricTag, $page0->photometric);
var_dump($page0->planarConfigurationTag, $page0->planarConfiguration);
var_dump(property_exists($page0, 'colorSpace'));
var_dump(imagecolorat($page0->image, 0, 0), imagecolorat($page1->image, 0, 0));
var_dump($reader->next(), $reader->next());

unset($reader);
var_dump(imagesx($page0->image), imagesx($page1->image));

$unknownCompression = Gd\Tiff\Reader::fromString(tiff_patch_short_tag($bytes, 259, 65000))->info();
var_dump($unknownCompression->compressionTag, $unknownCompression->compression);

$unknownPhotometric = Gd\Tiff\Reader::fromString(tiff_patch_short_tag($bytes, 262, 65001))->info();
var_dump($unknownPhotometric->photometricTag, $unknownPhotometric->photometric);
?>
--EXPECT--
int(2)
int(2)
int(2)
int(1)
enum(Gd\Tiff\Compression::None)
int(2)
enum(Gd\Tiff\Photometric::Rgb)
enum(Gd\Tiff\ResolutionUnit::Inch)
bool(false)
int(0)
int(2)
int(2)
int(1)
int(3)
int(1)
int(1)
enum(Gd\Tiff\Compression::None)
int(2)
enum(Gd\Tiff\Photometric::Rgb)
int(1)
enum(Gd\Tiff\PlanarConfiguration::Contiguous)
bool(false)
int(16711680)
int(65280)
NULL
NULL
int(2)
int(3)
int(65000)
NULL
int(65001)
NULL
