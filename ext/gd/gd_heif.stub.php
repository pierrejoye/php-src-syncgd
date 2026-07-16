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

namespace Gd\Heif {
#ifdef HAVE_GD_HEIF
    enum CompressionFormat
    {
        case Hevc;
        case Av1;
    }

    enum ChromaSubsampling
    {
        case Yuv420;
        case Yuv422;
        case Yuv444;
    }

    /** @strict-properties */
    final readonly class ReadOptions
    {
        public bool $ignoreTransformations;

        public function __construct(
            bool $ignoreTransformations = true,
        ) {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $quality;
        public bool $lossless;
        public CompressionFormat $codec;
        public ChromaSubsampling $chromaSubsampling;

        public function __construct(
            int $quality = -1,
            bool $lossless = false,
            CompressionFormat $codec = CompressionFormat::Hevc,
            ChromaSubsampling $chromaSubsampling = ChromaSubsampling::Yuv444,
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
