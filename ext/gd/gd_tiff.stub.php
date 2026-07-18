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

namespace Gd\Tiff {
#if defined(HAVE_GD_TIFF_WRITE_API) || defined(HAVE_GD_TIFF_READ_API)
    enum ColorSpace
    {
        case Rgb;
        case Rgba;
        case Gray;
    }

    enum Compression: int
    {
        case None = 1;
        case CcittRle = 2;
        case CcittFax3 = 3;
        case CcittFax4 = 4;
        case Lzw = 5;
        case Jpeg = 7;
        case AdobeDeflate = 8;
        case Deflate = 32946;
        case PackBits = 32773;
    }

    enum ResolutionUnit
    {
        case None;
        case Inch;
        case Centimeter;
    }
#endif

#ifdef HAVE_GD_TIFF_WRITE_API
    /** @strict-properties */
    final readonly class WriteOptions implements \Gd\Codec\WriteOptions
    {
        public Compression $compression;
        public ColorSpace $colorSpace;
        public bool $minIsWhite;
        public ResolutionUnit $resolutionUnit;
        public ?float $xResolution;
        public ?float $yResolution;
        public ?\Gd\Metadata $metadata;

        public function __construct(
            Compression $compression = Compression::Deflate,
            ColorSpace $colorSpace = ColorSpace::Rgba,
            bool $minIsWhite = false,
            ResolutionUnit $resolutionUnit = ResolutionUnit::Inch,
            ?float $xResolution = null,
            ?float $yResolution = null,
            ?\Gd\Metadata $metadata = null,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Writer
    {
        private function __construct() {}

        public static function toFile(
            string $path,
            WriteOptions $options = new WriteOptions(),
        ): \Gd\Tiff\Writer {}

        /** @param resource $stream */
        public static function toStream(
            $stream,
            WriteOptions $options = new WriteOptions(),
        ): \Gd\Tiff\Writer {}

        public static function toMemory(
            WriteOptions $options = new WriteOptions(),
        ): \Gd\Tiff\Writer {}

        public function addPage(\GdImage $image): static {}

        public function finish(): ?string {}
    }
#endif

#if defined(HAVE_GD_TIFF_WRITE_API) && defined(HAVE_GD_TIFF_READ_API)
    final class Codec
    {
        private function __construct() {}

        /** Throws Gd\Codec\CodecException if the TIFF has more than one page. */
        public static function fromFile(string $path): \GdImage {}

        /** Throws Gd\Codec\CodecException if the TIFF has more than one page. */
        public static function fromString(string $bytes): \GdImage {}

        /**
         * @param resource $stream
         * Throws Gd\Codec\CodecException if the TIFF has more than one page.
         */
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

#ifdef HAVE_GD_TIFF_READ_API
    enum Photometric: int
    {
        case MinIsWhite = 0;
        case MinIsBlack = 1;
        case Rgb = 2;
        case Palette = 3;
        case TransparencyMask = 4;
        case Separated = 5;
        case YCbCr = 6;
        case CieLab = 8;
    }

    enum PlanarConfiguration: int
    {
        case Contiguous = 1;
        case Separate = 2;
    }

    /** @strict-properties */
    final readonly class Info
    {
        public int $width;
        public int $height;
        public int $pageCount;
        public int $bitsPerSample;
        public int $samplesPerPixel;
        public int $compressionTag;
        public ?Compression $compression;
        public int $photometricTag;
        public ?Photometric $photometric;
        public bool $minIsWhite;
        public ?float $xResolution;
        public ?float $yResolution;
        public ?ResolutionUnit $resolutionUnit;
        public \Gd\Metadata $metadata;

        public function __construct(
            int $width,
            int $height,
            int $pageCount,
            int $bitsPerSample,
            int $samplesPerPixel,
            int $compressionTag,
            ?Compression $compression,
            int $photometricTag,
            ?Photometric $photometric,
            bool $minIsWhite,
            ?float $xResolution,
            ?float $yResolution,
            ?ResolutionUnit $resolutionUnit,
            \Gd\Metadata $metadata,
        ) {}
    }

    /** @strict-properties */
    final readonly class Page
    {
        public \GdImage $image;
        public int $pageIndex;
        public int $width;
        public int $height;
        public int $bitsPerSample;
        public int $samplesPerPixel;
        public int $compressionTag;
        public ?Compression $compression;
        public int $photometricTag;
        public ?Photometric $photometric;
        public int $planarConfigurationTag;
        public ?PlanarConfiguration $planarConfiguration;
        public bool $minIsWhite;
        public bool $hasAlpha;
        public bool $isTiled;
        public ?float $xResolution;
        public ?float $yResolution;
        public ?ResolutionUnit $resolutionUnit;

        public function __construct(
            \GdImage $image,
            int $pageIndex,
            int $width,
            int $height,
            int $bitsPerSample,
            int $samplesPerPixel,
            int $compressionTag,
            ?Compression $compression,
            int $photometricTag,
            ?Photometric $photometric,
            int $planarConfigurationTag,
            ?PlanarConfiguration $planarConfiguration,
            bool $minIsWhite,
            bool $hasAlpha,
            bool $isTiled,
            ?float $xResolution,
            ?float $yResolution,
            ?ResolutionUnit $resolutionUnit,
        ) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Reader
    {
        private function __construct() {}

        public static function fromFile(string $path): \Gd\Tiff\Reader {}

        public static function fromString(string $bytes): \Gd\Tiff\Reader {}

        /** @param resource $stream */
        public static function fromStream($stream): \Gd\Tiff\Reader {}

        public function info(): Info {}

        public function next(): ?Page {}
    }
#endif
}
