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

namespace Gd\Qoi {
#ifdef HAVE_GD_QOI
    /** @strict-properties */
    final readonly class ReadOptions
    {
        /** Reserved for future QOI read settings; currently a no-op. */
        public function __construct() {}
    }

    enum Colorspace
    {
        case SRGB;
        case Linear;
    }

    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public int $channels;
        public int $colorspaceTag;
        public Colorspace $colorspace;

        public function __construct(
            int $width,
            int $height,
            int $channels,
            int $colorspaceTag,
            Colorspace $colorspace,
        ) {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public Colorspace $colorspace;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            Colorspace $colorspace = Colorspace::SRGB,
            ?\Gd\Metadata $metadata = null,
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

    final class Codec
    {
        private function __construct() {}

        public static function fromFile(string $path, ReadOptions $options = new ReadOptions()): \GdImage {}

        /** @param resource $stream */
        public static function fromStream($stream, ReadOptions $options = new ReadOptions()): \GdImage {}

        public static function fromString(string $bytes, ReadOptions $options = new ReadOptions()): \GdImage {}

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
