<?php

/**
 * @generate-class-entries
 */

namespace Gd {
    /**
     * @strict-properties
     * @not-serializable
     */
    final class Metadata implements \Countable, \IteratorAggregate
    {
        private function __construct() {}

        public static function create(): static {}

        public function has(string $key): bool {}
        public function get(string $key): ?string {}

        /** @return list<string> */
        public function keys(): array {}

        public function count(): int {}
        public function getIterator(): \Iterator {}

        public function with(string $key, string $data): static {}
        public function without(string $key): static {}
    }
}
