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

namespace Gd\Avif {
#ifdef HAVE_GD_AVIF
    enum ChromaSubsampling
    {
        case Yuv420;
        case Yuv444;
    }

    /**
     * AVIF read options are reserved for future decoder options. The current
     * implementation has no active read options.
     * @strict-properties
     */
    final readonly class ReadOptions
    {
        public function __construct() {}
    }

    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public bool $isAnimation;
        public bool $isProgressive;
        public int $frameCount;
        public float $duration;
        public bool $hasAlpha;
        public int $bitDepth;
        public int $yuvFormat;
        public \Gd\Metadata $metadata;

        public function __construct(
            int $width,
            int $height,
            bool $isAnimation,
            bool $isProgressive,
            int $frameCount,
            float $duration,
            bool $hasAlpha,
            int $bitDepth,
            int $yuvFormat,
            \Gd\Metadata $metadata,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Avif\Reader {}
        public static function fromString(string $bytes): \Gd\Avif\Reader {}
        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Avif\Reader {}
        public function info(): Info {}
        public function read(): \GdImage {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $quality;
        public int $speed;
        public bool $lossless;
        public ?ChromaSubsampling $chromaSubsampling;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            int $quality = -1,
            int $speed = -1,
            bool $lossless = false,
            ?ChromaSubsampling $chromaSubsampling = null,
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
#endif
}
