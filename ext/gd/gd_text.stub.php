<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Gd\Text {
    /** @strict-properties */
    class TextException extends \Gd\GdException
    {
    }

    /** @strict-properties */
    class InvalidTextException extends TextException
    {
    }

    /** @strict-properties */
    class UnavailableException extends TextException
    {
    }

    /** @strict-properties */
    class FontException extends TextException
    {
    }

    /** @strict-properties */
    class LayoutException extends TextException
    {
    }

    /** @strict-properties */
    class MemoryException extends TextException
    {
    }
}

namespace Gd {
    enum TextShaping
    {
        case None;
        case Raqm;
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class FontFace
    {
        private function __construct() {}

        public static function fromFile(string $path, int $faceIndex = 0): FontFace {}

        public static function fromString(string $bytes, int $faceIndex = 0): FontFace {}
    }

    /** @strict-properties */
    final readonly class TextOptions
    {
        public TextShaping $shaping;

        public float $lineSpacing;

        public function __construct(TextShaping $shaping = TextShaping::None, float $lineSpacing = 1.0) {}
    }

    /** @strict-properties */
    final readonly class TextExtents
    {
        public float $xBearing;

        public float $yBearing;

        public float $width;

        public float $height;

        public float $xAdvance;

        public float $yAdvance;

        public function __construct(
            float $xBearing,
            float $yBearing,
            float $width,
            float $height,
            float $xAdvance,
            float $yAdvance,
        ) {}
    }

    final class Context
    {
        public function setFontFace(FontFace $font): Context {}

        public function setFontSize(float $size): Context {}

        public function textPath(string $text, float $x, float $y, ?TextOptions $options = null): Context {}

        public function showText(string $text, float $x, float $y, ?TextOptions $options = null): Context {}

        public function textExtents(string $text, ?TextOptions $options = null): TextExtents {}
    }
}
