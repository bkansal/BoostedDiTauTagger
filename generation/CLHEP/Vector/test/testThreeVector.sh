#! /bin/sh
# test/testThreeVector.sh.  Generated from testThreeVector.sh.in by configure.


./testThreeVector < "./testThreeVector.input" \
  | diff -q -b - "./testThreeVector.output" > /dev/null
