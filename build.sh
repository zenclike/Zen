#!/bin/bash

case "$OSTYPE" in
  msys*)
    gcc -Ofast -ftree-loop-linear -march=native -Wall -Wextra -static-libgcc -o bin/zenc.out src/zen.c src/zen-extended.c src/floats.c
    ;;
  *)
    gcc -Ofast -ftree-loop-linear -march=native -Wall -Wextra -static-libgcc -o bin/zenc src/zen.c src/zen-extended.c src/floats.c
    ;;
esac
