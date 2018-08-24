#! /bin/sh
# test/testInversion.sh.  Generated from testInversion.sh.in by configure.

rm -f testInversion.cout
"./testInversion" > testInversion.cout
diff -q -b testInversion.cout "./testInversion.output"
