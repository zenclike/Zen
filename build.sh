#!/bin/bash

case "$OSTYPE" in
  msys*)
    gcc -Wall -Wextra --static -o bin/zenc.out src/zen.c src/zen-extended.c
    ;;
  *)
    gcc -Wall -Wextra --static -o bin/zenc src/zen.c src/zen-extended.c
    ;;
esac
