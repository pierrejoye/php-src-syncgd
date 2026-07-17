<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Gd\Codec {
    enum Format
    {
        case Png;
        case Jpeg;
        case Webp;
        case Bmp;
        case Avif;
        case Heif;
        case Jxl;
        case Tiff;
        case Gif;
        case Qoi;
    }

    interface WriteOptions
    {
    }
}

namespace Gd {
    enum ColorModel
    {
        case TrueColor;
        case Palette;
    }

    enum PerceptualDiffMode
    {
        case None;
        case Overlay;
        case Mask;
    }

    enum ScaleFit
    {
        case Cover;
        case Contain;
        case Fill;
        case Inside;
        case Outside;
    }

    enum ScaleGravity
    {
        case Northwest;
        case North;
        case Northeast;
        case West;
        case Center;
        case East;
        case Southwest;
        case South;
        case Southeast;
    }

    enum ScaleStrategy
    {
        case Entropy;
        case Attention;
    }

    enum InterpolationMethod
    {
        case Default;
        case Bell;
        case Bessel;
        case Linear;
        case CubicSpline;
        case Blackman;
        case Box;
        case BSpline;
        case CatmullRom;
        case Gaussian;
        case GeneralizedCubic;
        case Hermite;
        case Hamming;
        case Hanning;
        case Mitchell;
        case NearestNeighbour;
        case Power;
        case Quadratic;
        case Sinc;
        case Triangle;
        case Weighted4;
        case Lanczos3;
        case Lanczos8;
        case BlackmanBessel;
        case BlackmanSinc;
        case QuadraticBSpline;
        case Cosine;
        case Welsh;
    }

    enum AutoCropMode
    {
        case Default;
        case Transparent;
        case Black;
        case White;
        case Sides;
        case Threshold;
    }

    /** @strict-properties */
    final readonly class PerceptualDiffOptions
    {
        public PerceptualDiffMode $mode;
        public int $highlightColor;

        public function __construct(
            PerceptualDiffMode $mode = PerceptualDiffMode::None,
            int $highlightColor = 0xFF0000,
        ) {}
    }

    /** @strict-properties */
    final readonly class PerceptualDiffResult
    {
        public int $pixelsChanged;
        public float $maximumDelta;
        public ?\GdImage $diffImage;

        public function __construct(
            int $pixelsChanged,
            float $maximumDelta,
            ?\GdImage $diffImage,
        ) {}
    }

    /** @strict-properties */
    final readonly class RotateOptions
    {
        public int $backgroundColor;

        public function __construct(
            int $backgroundColor = 0x7f000000,
        ) {}
    }

    /** @strict-properties */
    final readonly class ScaleOptions
    {
        public ?int $width;
        public ?int $height;
        public ScaleFit $fit;
        public ScaleGravity $gravity;
        public ?ScaleStrategy $strategy;
        public int $backgroundColor;
        public ?InterpolationMethod $interpolation;

        public function __construct(
            ?int $width = null,
            ?int $height = null,
            ScaleFit $fit = ScaleFit::Cover,
            ScaleGravity $gravity = ScaleGravity::Center,
            ?ScaleStrategy $strategy = null,
            int $backgroundColor = 0x7f000000,
            ?InterpolationMethod $interpolation = null,
        ) {}
    }

#ifdef HAVE_GD_BUNDLED
    /** @strict-properties */
    final readonly class TransformOptions
    {
        public ?Rect $clip;
        public int $backgroundColor;
        public ?InterpolationMethod $interpolation;

        public function __construct(
            ?Rect $clip = null,
            int $backgroundColor = 0x7f000000,
            ?InterpolationMethod $interpolation = null,
        ) {}
    }
#endif

    /** @strict-properties */
    final readonly class AutoCropOptions
    {
        public AutoCropMode $mode;
        public float $threshold;
        public int $color;

        public function __construct(
            AutoCropMode $mode = AutoCropMode::Default,
            float $threshold = 0.5,
            int $color = -1,
        ) {}
    }

    final class Image
    {
        private function __construct() {}

        /** @refcount 1 */
        public static function create(
            int $width,
            int $height,
            ColorModel $colorModel = ColorModel::TrueColor,
        ): \GdImage {}

        /** @refcount 1 */
        public static function loadFromFile(string $path): \GdImage {}

        /**
         * @param resource $stream
         * @refcount 1
         */
        public static function loadFromStream($stream): \GdImage {}

        /** @refcount 1 */
        public static function loadFromString(string $bytes): \GdImage {}
    }
}
