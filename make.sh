#!/bin/bash
export ONAME="cpp-ge-1_0.0.1"

CC=clang CXX=clang++ cmake -B build -DONAME="$ONAME"
cmake --build build

#run or no
run=''
read -r -p "run? (Y/n)" run
case "$run" in
    ""|[Yy])
        clear
        "./$ONAME"
        ;;
    *)
        echo "cancelled"
        ;;
esac
