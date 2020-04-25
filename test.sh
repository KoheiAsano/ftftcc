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

assert 0 0
assert 42 42
assert 21 "5+14+2"
assert 21 "5+18-2"
assert 42 " 12 + 2 + 13 + 12 + 6 - 3"
assert 18 "1+2+3*(4+(3-2))"
assert 2 "-3+5"
assert 10 "20+-10"
assert 15 "-3*-5"

echo OK