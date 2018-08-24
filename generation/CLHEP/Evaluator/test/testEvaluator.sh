#! /bin/sh
# test/testEvaluator.sh.  Generated from testEvaluator.sh.in by configure.


./testEvaluator < "./testEvaluator.input" \
  | diff -q -b - "./testEvaluator.output" > /dev/null
