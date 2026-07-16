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

namespace Gd\Bmp {
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

        public function __construct(
            int $bitsPerPixel = 0,
            ?Compression $compression = null,
            bool $quantize = false,
            bool $forceV4Header = false,
            bool $rgb555 = false,
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
