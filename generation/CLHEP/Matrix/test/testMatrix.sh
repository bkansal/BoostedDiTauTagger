#! /bin/sh
# test/testMatrix.sh.  Generated from testMatrix.sh.in by configure.


./testMatrix  \
  | diff -q -b - "./testMatrix.output" > /dev/null
