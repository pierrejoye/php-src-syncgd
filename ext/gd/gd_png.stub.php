<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Gd\Codec {
    /** @strict-properties */
    class CodecException extends \RuntimeException
    {
    }
}

namespace Gd\Png {
#ifdef HAVE_GD_PNG_CODEC
    enum ColorType: int
    {
        case Grayscale = 0;
        case Rgb = 2;
        case Palette = 3;
        case GrayscaleAlpha = 4;
        case Rgba = 6;
    }

    enum InterlaceMethod: int
    {
        case None = 0;
        case Adam7 = 1;
    }

    enum PhysicalUnit: int
    {
        case Unknown = 0;
        case Meter = 1;
    }

    enum Filter
    {
        case None;
        case Sub;
        case Up;
        case Average;
        case Paeth;
    }

    enum CompressionStrategy
    {
        case Default;
        case Filtered;
        case HuffmanOnly;
        case Rle;
        case Fixed;
    }

    /**
     * PNG read options are reserved for future decoder options. The current
     * implementation has no active read options.
     * @strict-properties
     */
    final readonly class ReadOptions
    {
        public function __construct() {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $compressionLevel;
        public array $filters;
        public CompressionStrategy $compressionStrategy;
        public array $comments;
        public ?\Gd\Metadata $metadata;

        /**
         * @param list<Filter> $filters
         * @param array<string, string> $comments
         */
        public function __construct(
            int $compressionLevel = -1,
            array $filters = [],
            CompressionStrategy $compressionStrategy = CompressionStrategy::Default,
            array $comments = [],
            ?\Gd\Metadata $metadata = null,
        ) {}
    }

    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public int $bitDepth;
        public int $colorTypeTag;
        public ?ColorType $colorType;
        public bool $hasAlpha;
        public bool $hasTransparency;
        public ?int $paletteEntries;
        public int $interlaceMethodTag;
        public ?InterlaceMethod $interlaceMethod;
        public ?int $xPixelsPerUnit;
        public ?int $yPixelsPerUnit;
        public ?int $resolutionX;
        public ?int $resolutionY;
        public ?PhysicalUnit $physicalUnit;
        public array $comments;
        public \Gd\Metadata $metadata;
        public bool $decodedTrueColor;

        /** @param array<string, string> $comments */
        public function __construct(
            int $width,
            int $height,
            int $bitDepth,
            int $colorTypeTag,
            ?ColorType $colorType,
            bool $hasAlpha,
            bool $hasTransparency,
            ?int $paletteEntries,
            int $interlaceMethodTag,
            ?InterlaceMethod $interlaceMethod,
            ?int $xPixelsPerUnit,
            ?int $yPixelsPerUnit,
            ?int $resolutionX,
            ?int $resolutionY,
            ?PhysicalUnit $physicalUnit,
            array $comments,
            \Gd\Metadata $metadata,
            bool $decodedTrueColor,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path, ReadOptions $options = new ReadOptions()): \Gd\Png\Reader {}
        public static function fromString(string $bytes, ReadOptions $options = new ReadOptions()): \Gd\Png\Reader {}

        /** @param resource $stream */
        public static function fromStream($stream, ReadOptions $options = new ReadOptions()): \Gd\Png\Reader {}

        public function info(): \Gd\Png\Info {}
        public function read(): \GdImage {}
    }

    final class Codec
    {
        private function __construct() {}

        public static function fromFile(string $path, ReadOptions $options = new ReadOptions()): \GdImage {}
        public static function fromString(string $bytes, ReadOptions $options = new ReadOptions()): \GdImage {}

        /** @param resource $stream */
        public static function fromStream($stream, ReadOptions $options = new ReadOptions()): \GdImage {}

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
#endif
}
