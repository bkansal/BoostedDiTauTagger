#! /bin/sh
# test/testLorentzVector.sh.  Generated from testLorentzVector.sh.in by configure.


./testLorentzVector < "./testLorentzVector.input" \
  | diff -q -b - "./testLorentzVector.output" > /dev/null
