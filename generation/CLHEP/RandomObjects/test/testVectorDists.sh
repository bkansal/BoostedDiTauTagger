#! /bin/sh
# test/testVectorDists.sh.  Generated from testVectorDists.sh.in by configure.


./testVectorDists < "./testVectorDists.input" \
  | diff -q -b - "./testVectorDists.output" > /dev/null
