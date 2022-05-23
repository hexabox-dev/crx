#!/bin/bash

no_warnings="-Wno-deprecated -Wno-c++11-compat-deprecated-writable-strings"

g++ -c -I. $no_warnings src/crx.c

if [ -f crx.o ]; then
	ls -l crx.o

	g++ -I. $no_warnings crx.o src/usage.c -o output/usage
	g++ -I. $no_warnings crx.o src/unittest.c -o output/unittest && output/unittest
fi
