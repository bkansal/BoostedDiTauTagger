#! /bin/sh
# test/testGenericFunctions.sh.  Generated from testGenericFunctions.sh.in by configure.


./testGenericFunctions  \
  | diff -q -b - "./testGenericFunctions.output" > /dev/null
