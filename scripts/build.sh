#/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/..
if [ ! -d "include/crow" ]; then
    git clone --depth 1 --branch v1.0+5 https://github.com/CrowCpp/Crow.git
    mv Crow/include/ .
    rm -rf Crow
fi
./scripts/test.sh
if [ $? -ne 0 ]; then
    exit 1
fi
cmake -Wno-dev .
make -j
if [ ! -f DataScrapper ]; then
    exit 1
fi
popd