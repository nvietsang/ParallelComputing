#!/bin/bash

# This script runs "make clean" in all directories

for x in `find . -name "Makefile"`; do
    echo "Cleaning using $x"
    cd `dirname $x`
    pwd
    make clean
    rm -v -f *~ *.optrpt \#* *.stf*
    cd $OLDPWD
done

