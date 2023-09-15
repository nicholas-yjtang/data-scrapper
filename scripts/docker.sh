#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/..
if [ -f .env ]; then
    set -o allexport
    source .env
    set +o allexport
fi
if [ ! -z "$DOCKER_BUILD_PROFILE" ]; then
    docker buildx inspect $DOCKER_BUILD_PROFILE
    if [ $? -ne 0 ]; then
        docker buildx create --use --name $DOCKER_BUILD_PROFILE --platform linux/amd64,linux/arm64
    fi
    docker buildx use $DOCKER_BUILD_PROFILE
    if [ ! -z "$DOCKER_IMAGE_NAME" ]; then
        ./scripts/login.sh
        echo "Building image $DOCKER_IMAGE_NAME:$DOCKER_IMAGE_TAG"
        docker buildx build --platform linux/amd64,linux/arm64 --push -t $DOCKER_IMAGE_NAME:$DOCKER_IMAGE_TAG .
    fi    
fi
