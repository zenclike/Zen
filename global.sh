#!/bin/bash

case "$OSTYPE" in
  msys*)
    alias zenc="$PWD/bin/zenc.out"
    ;;
  *)
    alias zenc="$PWD/bin/zenc"
    alias zen-readme="cat $PWD/README.md"
    ;;
esac
