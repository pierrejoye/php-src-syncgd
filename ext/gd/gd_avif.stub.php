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

    /** @strict-properties */
    final readonly class ReadOptions
    {
        public function __construct() {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $quality;
        public int $speed;
        public bool $lossless;
        public ?ChromaSubsampling $chromaSubsampling;

        public function __construct(
            int $quality = -1,
            int $speed = -1,
            bool $lossless = false,
            ?ChromaSubsampling $chromaSubsampling = null,
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
