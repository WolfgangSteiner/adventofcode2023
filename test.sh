#! /usr/bin/bash

build_and_run() {
    TEST_NAME=$1
    FILE_STEM="test_${TEST_NAME}"
    BUILD_CMD="gcc -Wall -std=c11 -g -o bin/${FILE_STEM} src/${FILE_STEM}.c src/aoc.c"
    RUN_CMD="bin/${FILE_STEM}"
    echo "Building ${TEST_NAME}..."
    eval $BUILD_CMD
    
    # check if build succeeded
    if [ $? -ne 0 ]; then
        echo "Build failed for ${TEST_NAME}"
        exit 1
    fi
    echo "Running ${TEST_NAME}..."
    eval $RUN_CMD
}


build_and_run str
build_and_run strarr
build_and_run int_arr