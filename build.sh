#!/bin/bash

if [ "`uname -s`" == "Darwin" ]; then
	no_warnings="-Wno-deprecated -Wno-c++11-compat-deprecated-writable-strings"
else
	no_warnings="-Wno-write-strings"
fi

g++ -c -I. $no_warnings src/crx.c

if [ -f crx.o ]; then
	ls -l crx.o

	if [ ! -d output ]; then mkdir output; fi
	g++ -I. $no_warnings crx.o src/usage.c -o output/usage
	g++ -I. $no_warnings crx.o src/unittest.c -o output/unittest && output/unittest
fi
