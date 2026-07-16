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

namespace Gd\Jpeg {
#ifdef HAVE_GD_JPEG_CODEC
    enum ColorSpace: int
    {
        case Unknown = 0;
        case Grayscale = 1;
        case Rgb = 2;
        case YCbCr = 3;
        case Cmyk = 4;
        case Ycck = 5;
    }

    enum DensityUnit: int
    {
        case None = 0;
        case Dpi = 1;
        case Dpcm = 2;
    }

    enum DctMethod: int
    {
        case Default = 0;
        case Slow = 1;
        case Fast = 2;

        /**
         * Floating-point IDCT may produce non-bit-identical pixels across
         * architectures/compilers.
         */
        case Float = 3;
    }

    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public int $bitsPerSample;
        public int $components;
        public int $colorSpaceTag;
        public ?ColorSpace $colorSpace;
        public bool $progressive;
        public int $densityUnitTag;
        public ?DensityUnit $densityUnit;
        public ?int $xDensity;
        public ?int $yDensity;
        public bool $hasExif;
        public bool $hasXmp;
        public bool $hasIcc;
        public bool $hasIptc;

        public function __construct(
            int $width,
            int $height,
            int $bitsPerSample,
            int $components,
            int $colorSpaceTag,
            ?ColorSpace $colorSpace,
            bool $progressive,
            int $densityUnitTag,
            ?DensityUnit $densityUnit,
            ?int $xDensity,
            ?int $yDensity,
            bool $hasExif,
            bool $hasXmp,
            bool $hasIcc,
            bool $hasIptc,
        ) {}
    }

    /** @strict-properties */
    final readonly class ReadOptions
    {
        public bool $ignoreWarnings;
        public int $scaleNumerator;
        public int $scaleDenominator;
        public DctMethod $dctMethod;

        public function __construct(
            bool $ignoreWarnings = true,
            int $scaleNumerator = 1,
            int $scaleDenominator = 1,
            DctMethod $dctMethod = DctMethod::Default,
        ) {}
    }

    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public int $quality;
        public bool $progressive;
        public bool $forceNoSubsampling;
        public ?string $exif;
        public ?string $xmp;
        public ?string $icc;
        public ?string $iptc;

        public function __construct(
            int $quality = -1,
            bool $progressive = false,
            bool $forceNoSubsampling = false,
            ?string $exif = null,
            ?string $xmp = null,
            ?string $icc = null,
            ?string $iptc = null,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(
            string $path,
            ReadOptions $options = new ReadOptions(),
        ): \Gd\Jpeg\Reader {}

        public static function fromString(
            string $bytes,
            ReadOptions $options = new ReadOptions(),
        ): \Gd\Jpeg\Reader {}

        /** @param resource $stream */
        public static function fromStream(
            $stream,
            ReadOptions $options = new ReadOptions(),
        ): \Gd\Jpeg\Reader {}

        public function info(): \Gd\Jpeg\Info {}
        public function read(): \GdImage {}
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
