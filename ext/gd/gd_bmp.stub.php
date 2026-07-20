<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Gd\Bmp {
#ifdef HAVE_GD_BUNDLED
    /** @strict-properties */
    final readonly class Info
    {
        public int $headerType;
        public int $width;
        public int $height;
        public bool $topDown;
        public int $planes;
        public int $bitsPerPixel;
        public int $compressionTag;
        public int $imageSize;
        public int $horizontalResolution;
        public int $verticalResolution;
        public int $colorsUsed;
        public int $importantColors;
        public int $paletteType;
        public int $paletteEntries;
        public int $redMask;
        public int $greenMask;
        public int $blueMask;
        public int $alphaMask;

        public function __construct(
            int $headerType,
            int $width,
            int $height,
            bool $topDown,
            int $planes,
            int $bitsPerPixel,
            int $compressionTag,
            int $imageSize,
            int $horizontalResolution,
            int $verticalResolution,
            int $colorsUsed,
            int $importantColors,
            int $paletteType,
            int $paletteEntries,
            int $redMask,
            int $greenMask,
            int $blueMask,
            int $alphaMask,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path, ReadOptions $options = new ReadOptions()): Reader {}
        /** @param resource $stream */
        public static function fromStream($stream, ReadOptions $options = new ReadOptions()): Reader {}
        public static function fromString(string $bytes, ReadOptions $options = new ReadOptions()): Reader {}
        public function info(): Info {}
        public function read(): \GdImage {}
    }
#endif

    enum Compression
    {
        case None;
        case Rle4;
        case Rle8;
    }

    /** @strict-properties */
    final readonly class ReadOptions
    {
        public function __construct() {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $bitsPerPixel;
        public ?Compression $compression;
        public bool $quantize;
        public bool $forceV4Header;
        public bool $rgb555;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            int $bitsPerPixel = 0,
            ?Compression $compression = null,
            bool $quantize = false,
            bool $forceV4Header = false,
            bool $rgb555 = false,
            ?\Gd\Metadata $metadata = null,
        ) {}
    }

    final class Codec
    {
        private function __construct() {}

        public static function fromFile(
            string $path,
            ReadOptions $options = new ReadOptions(),
        ): \GdImage {}

        public static function fromString(
            string $bytes,
            ReadOptions $options = new ReadOptions(),
        ): \GdImage {}

        /** @param resource $stream */
        public static function fromStream(
            $stream,
            ReadOptions $options = new ReadOptions(),
        ): \GdImage {}

        public static function toFile(
            \GdImage $image,
            string $path,
            WriteOptions $options = new WriteOptions(),
        ): void {}

        /** @param resource $stream */
        public static function toStream(
            \GdImage $image,
            $stream,
            WriteOptions $options = new WriteOptions(),
        ): void {}

        public static function toString(
            \GdImage $image,
            WriteOptions $options = new WriteOptions(),
        ): string {}
    }
}
