#!/bin/bash
IMAGE=mspemu:latest

docker run \
        -ti \
        --rm \
        -u $(id -u ${USER}):$(id -g ${USER}) \
        -v ${PWD}:/home/msp/workspace \
        "$IMAGE" \
        "$@"
