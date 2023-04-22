#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/..
pushd test
rm -rf cmake_install.cmake CMakeCache.txt Makefile DataScrapperTest data conf
popd
rm -rf build
rm -rf cmake_install.cmake CMakeCache.txt Makefile DataScrapper data conf
popd