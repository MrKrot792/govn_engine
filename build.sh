#!/usr/bin/env bash

if [ "$1" == "r" ] || [ "$1" == "run" ]; then
    build/govnengine
elif [ ! -d "build/" ]; then
    mkdir build
    cd build
    cmake ..
    make
elif [ "$1" == "b" ] || [ "$1" == "build" ]; then
    cd build
    cmake ..
    make
else
    echo "Nothig to do: abort"
fi

