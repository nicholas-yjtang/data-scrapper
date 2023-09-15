#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/../test
rm -rf cmake_install.cmake CMakeCache.txt Makefile DataScrapperTest data conf
cmake -Wno-dev . -DCMAKE_BUILD_TYPE=Debug 
make -j
# failed to build, exit with status 1
if [ ! -f DataScrapperTest ]; then
    exit 1
fi
./DataScrapperTest
# some error while running tests, exit with status 1
if [ $? -ne 0 ]; then
    exit 1
fi
popd
