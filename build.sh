#!/bin/bash

case "$OSTYPE" in
  msys*)
    gcc -Wall -Wextra --static -o bin/zenc.out src/zen.c src/zen-extended.c src/floats.c
    ;;
  *)
    gcc -Wall -Wextra --static -o bin/zenc src/zen.c src/zen-extended.c src/floats.c
    ;;
esac
