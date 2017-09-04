#!/bin/sh

mkdir -p build
cd build; cmake .. && make -j 5 && ctest; cd -
