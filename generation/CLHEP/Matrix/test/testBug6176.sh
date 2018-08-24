#! /bin/sh
# test/testBug6176.sh.  Generated from testBug6176.sh.in by configure.

./testBug6176  \
  | diff -q -b - "./testBug6176.output" > /dev/null
