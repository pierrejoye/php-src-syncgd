--TEST--
Gd\Metadata is immutable and iterable
--EXTENSIONS--
gd
--FILE--
<?php
$empty = Gd\Metadata::create();
$one = $empty->with('example', "a\0b");
$two = $one->with('second', 'value')->without('example');

var_dump($empty->count(), bin2hex($one->get('example')), $two->keys(), $two->has('example'));
var_dump(iterator_to_array($two));
try {
    $one->with('', 'invalid');
} catch (ValueError $e) {
    echo $e::class, "\n";
}
--EXPECT--
int(0)
string(6) "610062"
array(1) {
  [0]=>
  string(6) "second"
}
bool(false)
array(1) {
  ["second"]=>
  string(5) "value"
}
ValueError
