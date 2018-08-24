#! /bin/sh
# test/testBasicVector3D.sh.  Generated from testBasicVector3D.sh.in by configure.


./testBasicVector3D < "./testBasicVector3D.input" \
  | diff -q -b - "./testBasicVector3D.output" > /dev/null
