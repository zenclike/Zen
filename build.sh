#!/bin/bash

CC="gcc"
STANDARD_FLAGS="-std=c99 -Wall -Wextra"
PORTABILITY_FLAGS="-static-libgcc"
OPTIMIZATION_FLAGS="-Ofast -ftree-loop-linear -march=native"
CFLAGS="$STANDARD_FLAGS $PORTABILITY_FLAGS $OPTIMIZATION_FLAGS"
SOURCES="src/zen.c src/zen-extended.c src/floats.c"

case "$OSTYPE" in
  msys*)
    TARGET="bin/zenc.out"
    echo "COMPILING \"$TARGET\""
    $CC $CFLAGS -o $TARGET $SOURCES
    result=$?
    if (( $result == 0 )); then
      echo "COMPILATION SUCCESSFUL"
    else
      echo "COMPILATION TERMINATED"
    fi
    ;;
  *)
    TARGET="bin/zenc"
    echo "COMPILING \"$TARGET\""
    $CC $CFLAGS -o $TARGET $SOURCES
    result=$?
    if (( $result == 0 )); then
      echo "COMPILATION SUCCESSFUL"
    else
      echo "COMPILATION TERMINATED"
    fi
    ;;
esac
