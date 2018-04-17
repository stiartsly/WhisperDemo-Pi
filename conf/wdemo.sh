#!/bin/sh

[ -e src/wdemo ] && DYLD_LIBRARY_PATH=$PWD/../deps/libs ./src/wdemo $*
