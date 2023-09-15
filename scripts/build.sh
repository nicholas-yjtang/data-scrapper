#/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/..
if [ ! -d "include/crow" ]; then
    if [ ! -z "$HTTPS_PROXY" ]; then
        echo "Setting https proxy"
        git config --global http.proxy $HTTP_PROXY
        git config --global https.proxy $HTTPS_PROXY
        git config --global http.sslVerify false
    fi
    git clone --depth 1 --branch v1.0+5 https://github.com/CrowCpp/Crow.git
    mv Crow/include/ .
    rm -rf Crow
fi
./scripts/test.sh
# check if tests passed
if [ $? -ne 0 ]; then
    exit 1
fi
cmake -Wno-dev .
make -j
# failed to build, exit with status 1
if [ ! -f DataScrapper ]; then
    exit 1
fi
popd