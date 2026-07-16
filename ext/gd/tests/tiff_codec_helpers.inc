<?php

function tiff_codec_inspect(string $data): array
{
    $little = substr($data, 0, 2) === "II";
    if (!$little && substr($data, 0, 2) !== "MM") {
        throw new Exception('Invalid TIFF byte order');
    }

    $u16 = static function (int $offset) use ($data, $little): int {
        return unpack($little ? 'v' : 'n', substr($data, $offset, 2))[1];
    };
    $u32 = static function (int $offset) use ($data, $little): int {
        return unpack($little ? 'V' : 'N', substr($data, $offset, 4))[1];
    };

    if ($u16(2) !== 42) {
        throw new Exception('Not a classic TIFF');
    }

    $directories = [];
    $ifdOffset = $u32(4);
    while ($ifdOffset !== 0) {
        $count = $u16($ifdOffset);
        $tags = [];
        for ($i = 0; $i < $count; $i++) {
            $entry = $ifdOffset + 2 + $i * 12;
            $tag = $u16($entry);
            $type = $u16($entry + 2);
            $valueCount = $u32($entry + 4);
            $valueOffset = $entry + 8;

            if ($type === 3 && $valueCount === 1) {
                $value = $u16($valueOffset);
            } elseif ($type === 4 && $valueCount === 1) {
                $value = $u32($valueOffset);
            } elseif ($type === 5 && $valueCount === 1) {
                $rationalOffset = $u32($valueOffset);
                $value = $u32($rationalOffset) / $u32($rationalOffset + 4);
            } elseif ($type === 3) {
                $arrayOffset = $u32($valueOffset);
                $value = [];
                for ($j = 0; $j < $valueCount; $j++) {
                    $value[] = $u16($arrayOffset + $j * 2);
                }
            } elseif ($type === 4) {
                $arrayOffset = $u32($valueOffset);
                $value = [];
                for ($j = 0; $j < $valueCount; $j++) {
                    $value[] = $u32($arrayOffset + $j * 4);
                }
            } else {
                continue;
            }
            $tags[$tag] = $value;
        }

        $directories[] = $tags;
        $ifdOffset = $u32($ifdOffset + 2 + $count * 12);
    }

    return $directories;
}

