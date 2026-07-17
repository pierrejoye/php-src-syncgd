<?php

/**
 * @generate-class-entries
 */

namespace Gd {
    enum ExtendMode
    {
        case None;
        case Repeat;
        case Reflect;
        case Pad;
    }

    enum LineCap
    {
        case Butt;
        case Round;
        case Square;
    }

    enum LineJoin
    {
        case Miter;
        case Round;
        case Bevel;
    }

    enum FillRule
    {
        case NonZero;
        case EvenOdd;
    }

    enum CompositeOperator
    {
        case Clear;
        case Source;
        case Over;
        case In;
        case Out;
        case Atop;
        case Dest;
        case DestOver;
        case DestIn;
        case DestOut;
        case DestAtop;
        case Xor;
        case Add;
        case Saturate;
        case Multiply;
        case Screen;
        case Overlay;
        case Darken;
        case Lighten;
        case ColorDodge;
        case ColorBurn;
        case HardLight;
        case SoftLight;
        case Difference;
        case Exclusion;
        case HslHue;
        case HslSaturation;
        case HslColor;
        case HslLuminosity;

        public function isUnbounded(): bool {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    abstract class Paint
    {
        private function __construct() {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    abstract class Gradient extends Paint
    {
        private function __construct() {}

        public function addColorStopRgb(float $offset, float $r, float $g, float $b): Gradient {}

        public function addColorStopRgba(float $offset, float $r, float $g, float $b, float $a): Gradient {}

        public function setExtend(ExtendMode $extend): Gradient {}

        public function setMatrix(Matrix $matrix): Gradient {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class LinearGradient extends Gradient
    {
        public function __construct(float $x0, float $y0, float $x1, float $y1) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class RadialGradient extends Gradient
    {
        public function __construct(float $x0, float $y0, float $r0, float $x1, float $y1, float $r1) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Matrix
    {
        public function __construct() {}

        public static function translation(float $x, float $y): Matrix {}

        public static function scaling(float $x, float $y): Matrix {}

        public static function shearing(float $x, float $y): Matrix {}

        public static function rotation(float $radians): Matrix {}

        public static function rotationAround(float $radians, float $x, float $y): Matrix {}

        public static function fromCoefficients(
            float $m00,
            float $m10,
            float $m01,
            float $m11,
            float $m02,
            float $m12
        ): Matrix {}

        public function translate(float $x, float $y): Matrix {}

        public function scale(float $x, float $y): Matrix {}

        public function shear(float $x, float $y): Matrix {}

        public function rotate(float $radians): Matrix {}

        public function rotateAround(float $radians, float $x, float $y): Matrix {}

        public function multiply(Matrix $other): Matrix {}

        public function inverted(): ?Matrix {}

        public function mapPoint(float $x, float $y): array {}

        public function mapRect(Rect $rect): Rect {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Rect
    {
        public readonly float $x;

        public readonly float $y;

        public readonly float $width;

        public readonly float $height;

        public function __construct(float $x, float $y, float $width, float $height) {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Path
    {
        public function __construct() {}

        public function moveTo(float $x, float $y): Path {}

        public function relMoveTo(float $dx, float $dy): Path {}

        public function lineTo(float $x, float $y): Path {}

        public function relLineTo(float $dx, float $dy): Path {}

        public function curveTo(
            float $x1,
            float $y1,
            float $x2,
            float $y2,
            float $x3,
            float $y3
        ): Path {}

        public function relCurveTo(
            float $dx1,
            float $dy1,
            float $dx2,
            float $dy2,
            float $dx3,
            float $dy3
        ): Path {}

        public function quadTo(float $x1, float $y1, float $x2, float $y2): Path {}

        public function relQuadTo(float $dx1, float $dy1, float $dx2, float $dy2): Path {}

        public function arc(float $cx, float $cy, float $radius, float $a0, float $a1): Path {}

        public function negativeArc(float $cx, float $cy, float $radius, float $a0, float $a1): Path {}

        public function rect(float $x, float $y, float $width, float $height): Path {}

        public function close(): Path {}

        public function appendPath(Path $other): Path {}

        public function transform(Matrix $matrix): Path {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Pattern extends Paint
    {
        private function __construct() {}

        public function setExtend(ExtendMode $extend): Pattern {}

        public function setMatrix(Matrix $matrix): Pattern {}

        public function setOpacity(float $opacity): Pattern {}
    }

    /**
     * @strict-properties
     * @not-serializable
     */
    final class Context
    {
        private function __construct() {}

        public static function createForImage(\GdImage $image): Context {}

        public function getImage(): \GdImage {}

        public function flushImage(): void {}

        public function reloadImage(): void {}

        public function destroy(bool $flush = true): void {}

        public function createPattern(\GdImage $image): Pattern {}

        public function appendPath(Path $path): Context {}

        public function save(): Context {}

        public function restore(): Context {}

        public function clip(): Context {}

        public function clipPreserve(): Context {}

        public function setSource(Paint $source): Context {}

        public function setSourceRgb(float $r, float $g, float $b): Context {}

        public function setSourceRgba(float $r, float $g, float $b, float $a): Context {}

        public function setSourceImage(\GdImage $image, float $x = 0, float $y = 0): Context {}

        public function setOperator(CompositeOperator $operator): Context {}

        public function setOpacity(float $opacity): Context {}

        public function setLineWidth(float $width): Context {}

        public function setLineCap(LineCap $cap): Context {}

        public function setLineJoin(LineJoin $join): Context {}

        public function setDash(float $offset, array $pattern): Context {}

        public function setFillRule(FillRule $rule): Context {}

        public function beginPath(): Context {}

        public function scale(float $x, float $y): Context {}

        public function translate(float $x, float $y): Context {}

        public function rotate(float $radians): Context {}

        public function transform(Matrix $matrix): Context {}

        public function moveTo(float $x, float $y): Context {}

        public function relMoveTo(float $dx, float $dy): Context {}

        public function lineTo(float $x, float $y): Context {}

        public function relLineTo(float $dx, float $dy): Context {}

        public function curveTo(
            float $x1,
            float $y1,
            float $x2,
            float $y2,
            float $x3,
            float $y3
        ): Context {}

        public function relCurveTo(
            float $dx1,
            float $dy1,
            float $dx2,
            float $dy2,
            float $dx3,
            float $dy3
        ): Context {}

        public function quadTo(float $x1, float $y1, float $x2, float $y2): Context {}

        public function relQuadTo(float $dx1, float $dy1, float $dx2, float $dy2): Context {}

        public function arc(float $cx, float $cy, float $radius, float $a0, float $a1): Context {}

        public function negativeArc(float $cx, float $cy, float $radius, float $a0, float $a1): Context {}

        public function rect(float $x, float $y, float $width, float $height): Context {}

        public function closePath(): Context {}

        public function fill(): Context {}

        public function fillPreserve(): Context {}

        public function stroke(): Context {}

        public function strokePreserve(): Context {}

        public function paint(): Context {}
    }
}
