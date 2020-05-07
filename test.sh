#!/usr/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./ftftcc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 "return 0;"
assert 42 "return 42;"
assert 21 "return 5+14+2;"
assert 21 "return 5+18-2;"
assert 42 "return  12 + 2 + 13 + 12 + 6 - 3;"
assert 18 "return  1 + 2 + 3 * ( 4 + ( 3 - 2 ) ) ;"
assert 2 "return -3+5;"
assert 10 "return 20+-10;"
assert 15 "return -3*-5;"
assert 3 "return - - 3;"
assert 1 "return 3 < 4;"
assert 1 "return 3 < 4 < 4 < 3;"
assert 0 "return 3 < 4 < (4 < 3);"
assert 1 "return 4 <= 4;"
assert 1 "return 4 == 4;"
assert 1 "return 4 >= 4;"
assert 0 "return 4 != 4;"
assert 1 "return 10 != 4;"

assert 1 "1;return 10 != 4;"
assert 3 "1;2;return 3;"
assert 2 "1;return 2;3;"
assert 1 "return 1;2;3;"

assert 3 "a=3; return a;"
assert 8 "a=3; z=5; return a+z;"

echo OK