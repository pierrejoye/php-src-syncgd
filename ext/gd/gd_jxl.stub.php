<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Gd\Jxl {
#ifdef HAVE_GD_JXL_CODEC
    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public bool $lossless;
        public float $distance;
        public int $effort;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            bool $lossless = false,
            float $distance = 1.0,
            int $effort = 7,
            ?\Gd\Metadata $metadata = null,
        ) {}
    }

    final class Codec
    {
        private function __construct() {}

        public static function fromFile(string $path): \GdImage {}
        public static function fromString(string $bytes): \GdImage {}

        /** @param resource $stream */
        public static function fromStream($stream): \GdImage {}

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

#if defined(HAVE_GD_JXL_CODEC) || defined(HAVE_GD_JXL_ANIM_READ_API)
    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public bool $animated;
        public int $loopCount;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            int $width,
            int $height,
            bool $animated,
            int $loopCount,
            ?\Gd\Metadata $metadata = null,
        ) {}
    }
#endif

#ifdef HAVE_GD_JXL_ANIM_READ_API
    /** @strict-properties */
    final readonly class Frame
    {
        public \GdImage $image;
        public int $frameIndex;
        public int $delayMs;
        public int $timestampMs;

        public function __construct(
            \GdImage $image,
            int $frameIndex,
            int $delayMs,
            int $timestampMs,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class AnimReader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Jxl\AnimReader {}
        public static function fromString(string $bytes): \Gd\Jxl\AnimReader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Jxl\AnimReader {}

        public function info(): Info {}
        public function next(): ?Frame {}
    }
#endif

#ifdef HAVE_GD_JXL_CODEC
    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Jxl\Reader {}
        public static function fromString(string $bytes): \Gd\Jxl\Reader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Jxl\Reader {}

        public function info(): Info {}
        public function read(): \GdImage {}
    }
#endif

#ifdef HAVE_GD_JXL_ANIM_WRITE_API
    /** @strict-properties */
    final readonly class AnimWriteOptions
    {
        public int $canvasWidth;
        public int $canvasHeight;
        public int $loopCount;
        public bool $lossless;
        public float $distance;
        public int $effort;

        public function __construct(
            int $canvasWidth = 0,
            int $canvasHeight = 0,
            int $loopCount = 0,
            bool $lossless = false,
            float $distance = 1.0,
            int $effort = 7,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class AnimWriter
    {
        private function __construct() {}

        public static function toFile(
            string $path,
            AnimWriteOptions $options = new AnimWriteOptions(),
        ): \Gd\Jxl\AnimWriter {}

        /** @param resource $stream */
        public static function toStream(
            $stream,
            AnimWriteOptions $options = new AnimWriteOptions(),
        ): \Gd\Jxl\AnimWriter {}

        public static function toMemory(
            AnimWriteOptions $options = new AnimWriteOptions(),
        ): \Gd\Jxl\AnimWriter {}

        public function addFrame(\GdImage $image, int $delayMs = 0): static {}
        public function finish(): ?string {}
    }
#endif
}
