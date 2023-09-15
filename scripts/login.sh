#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $SCRIPT_DIR/..
if [ -f .env ]; then
    set -o allexport
    source .env
    set +o allexport
fi
if [ ! -z "$DOCKER_USERNAME" ]; then
    if [ ! -z "$DOCKER_PASSWORD" ]; then
        echo "Going to login to docker hub as $DOCKER_USERNAME"
        docker login -u $DOCKER_USERNAME -p $DOCKER_PASSWORD
    fi
fi