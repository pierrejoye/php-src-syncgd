<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Gd\Webp {
#ifdef HAVE_GD_WEBP
    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $quality;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            int $quality = -1,
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

#if defined(HAVE_GD_WEBP_ANIM_READ_API) || defined(HAVE_GD_WEBP_ANIM_WRITE_API)
    enum DisposeMethod: int
    {
        case None = 0;
        case Background = 1;
    }

    enum BlendMethod: int
    {
        case Alpha = 0;
        case None = 1;
    }
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_WEBP_ANIM_READ_API)
    enum FormatFlag: int
    {
        case Animation = 0x00000002;
        case Xmp = 0x00000004;
        case Exif = 0x00000008;
        case Alpha = 0x00000010;
        case Iccp = 0x00000020;
    }

    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public int $frameCount;
        public int $loopCount;
        public int $backgroundColor;
        public int $formatFlags;
        public bool $isAnimated;
        public \Gd\Metadata $metadata;

        public function __construct(
            int $width,
            int $height,
            int $frameCount,
            int $loopCount,
            int $backgroundColor,
            int $formatFlags,
            bool $isAnimated,
            \Gd\Metadata $metadata,
        ) {}
    }
#endif

#ifdef HAVE_GD_BUNDLED
    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Webp\Reader {}
        public static function fromString(string $bytes): \Gd\Webp\Reader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Webp\Reader {}

        public function info(): Info {}
        public function read(): \GdImage {}
    }
#endif

#ifdef HAVE_GD_WEBP_ANIM_READ_API

    /** @strict-properties */
    final readonly class Frame
    {
        public \GdImage $image;
        public int $frameIndex;
        public int $x;
        public int $y;
        public int $width;
        public int $height;
        public int $durationMs;
        public int $timestampMs;
        public int $disposeTag;
        public ?DisposeMethod $dispose;
        public int $blendTag;
        public ?BlendMethod $blend;
        public bool $hasAlpha;
        public bool $complete;

        public function __construct(
            \GdImage $image,
            int $frameIndex,
            int $x,
            int $y,
            int $width,
            int $height,
            int $durationMs,
            int $timestampMs,
            int $disposeTag,
            ?DisposeMethod $dispose,
            int $blendTag,
            ?BlendMethod $blend,
            bool $hasAlpha,
            bool $complete,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class AnimReader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Webp\AnimReader {}
        public static function fromString(string $bytes): \Gd\Webp\AnimReader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Webp\AnimReader {}

        public static function isAnimatedFile(string $path): bool {}
        public static function isAnimatedString(string $bytes): bool {}

        /** @param resource $stream */
        public static function isAnimatedStream($stream): bool {}

        public function info(): Info {}
        public function next(): ?Frame {}
    }
#endif

#ifdef HAVE_GD_WEBP_ANIM_WRITE_API
    /** @strict-properties */
    final readonly class AnimWriteOptions
    {
        public int $canvasWidth;
        public int $canvasHeight;
        public int $loopCount;
        public int $backgroundColor;
        public int $quality;
        public bool $lossless;
        public int $method;
        public bool $minimizeSize;
        public int $kmin;
        public int $kmax;
        public bool $allowMixed;

        public function __construct(
            int $canvasWidth = 0,
            int $canvasHeight = 0,
            int $loopCount = 0,
            int $backgroundColor = 0x00000000,
            int $quality = -1,
            bool $lossless = false,
            int $method = 4,
            bool $minimizeSize = false,
            int $kmin = 9,
            int $kmax = 17,
            bool $allowMixed = false,
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
        ): \Gd\Webp\AnimWriter {}

        /** @param resource $stream */
        public static function toStream(
            $stream,
            AnimWriteOptions $options = new AnimWriteOptions(),
        ): \Gd\Webp\AnimWriter {}

        public static function toMemory(
            AnimWriteOptions $options = new AnimWriteOptions(),
        ): \Gd\Webp\AnimWriter {}

        public function addFrame(\GdImage $image, int $durationMs): static {}

        public function finish(): ?string {}
    }
#endif
}
