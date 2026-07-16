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
    enum Colorspace
    {
        case SRGB;
        case Linear;
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public Colorspace $colorspace;

        public function __construct(
            Colorspace $colorspace = Colorspace::SRGB,
        ) {}
    }

    final class Codec
    {
        private function __construct() {}

        public static function fromFile(string $path): \GdImage {}

        /** @param resource $stream */
        public static function fromStream($stream): \GdImage {}

        public static function fromString(string $bytes): \GdImage {}

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
