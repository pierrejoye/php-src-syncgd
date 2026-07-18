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

namespace Gd\Gif {
    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public ?\Gd\Metadata $metadata;

        public function __construct(?\Gd\Metadata $metadata = null) {}
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

#if defined(HAVE_GD_GIF_ANIM_READ_API) || defined(HAVE_GD_GIF_ANIM_WRITE_API)
    enum DisposalMethod: int
    {
        case Unknown = 0;
        case None = 1;
        case RestoreBackground = 2;
        case RestorePrevious = 3;
    }
#endif

#if defined(HAVE_GD_BUNDLED) || defined(HAVE_GD_GIF_ANIM_READ_API)
#ifdef HAVE_GD_BUNDLED
    /** @strict-properties */
    final readonly class Info
    {
        public string $version;
        public int $width;
        public int $height;
        public int $backgroundIndex;
        public bool $globalColorTable;
        public int $colorResolution;
        public float $pixelAspectRatio;
        public ?int $loopCount;

        public function __construct(
            string $version,
            int $width,
            int $height,
            int $backgroundIndex,
            bool $globalColorTable,
            int $colorResolution,
            float $pixelAspectRatio,
            ?int $loopCount,
        ) {}
    }
#endif
#endif

#ifdef HAVE_GD_BUNDLED
    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Gif\Reader {}
        public static function fromString(string $bytes): \Gd\Gif\Reader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Gif\Reader {}

        public function info(): Info {}
        public function read(): \GdImage {}
    }
#endif

#ifdef HAVE_GD_GIF_ANIM_READ_API

    /** @strict-properties */
    final readonly class Frame
    {
        public \GdImage $image;
        public int $frameIndex;
        public int $x;
        public int $y;
        public int $width;
        public int $height;
        public int $delayMs;
        public int $disposalTag;
        public ?DisposalMethod $disposal;
        public ?int $transparentIndex;
        public bool $localColorTable;
        public bool $interlaced;

        public function __construct(
            \GdImage $image,
            int $frameIndex,
            int $x,
            int $y,
            int $width,
            int $height,
            int $delayMs,
            int $disposalTag,
            ?DisposalMethod $disposal,
            ?int $transparentIndex,
            bool $localColorTable,
            bool $interlaced,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class AnimReader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Gif\AnimReader {}
        public static function fromString(string $bytes): \Gd\Gif\AnimReader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Gif\AnimReader {}

        public static function isAnimatedFile(string $path): bool {}
        public static function isAnimatedString(string $bytes): bool {}

        /** @param resource $stream */
        public static function isAnimatedStream($stream): bool {}

        public function info(): Info {}
        public function next(): ?Frame {}
    }
#endif

#ifdef HAVE_GD_GIF_ANIM_WRITE_API
    /** @strict-properties */
    final readonly class FrameOptions
    {
        public int $x;
        public int $y;
        public int $delayMs;
        public DisposalMethod $disposal;
        public bool $localColorMap;
        public bool $optimizeAgainstPrevious;

        public function __construct(
            int $x = 0,
            int $y = 0,
            int $delayMs = 0,
            DisposalMethod $disposal = DisposalMethod::None,
            bool $localColorMap = false,
            bool $optimizeAgainstPrevious = true,
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
            bool $globalColorMap = true,
            int $loops = 0,
        ): \Gd\Gif\AnimWriter {}

        /** @param resource $stream */
        public static function toStream(
            $stream,
            bool $globalColorMap = true,
            int $loops = 0,
        ): \Gd\Gif\AnimWriter {}

        public static function toMemory(
            bool $globalColorMap = true,
            int $loops = 0,
        ): \Gd\Gif\AnimWriter {}

        public function addFrame(
            \GdImage $image,
            FrameOptions $options = new FrameOptions(),
        ): static {}

        public function finish(): ?string {}
    }
#endif
}
