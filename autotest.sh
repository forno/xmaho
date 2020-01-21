#!/bin/sh

mkdir -p build
cd build; cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TEST=ON && make -j 5 && ctest -j 5; cd -
