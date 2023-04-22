#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/../test
rm -rf cmake_install.cmake CMakeCache.txt Makefile DataScrapperTest data conf
cmake -Wno-dev . -DCMAKE_BUILD_TYPE=Debug 
make -j
./DataScrapperTest
popd
