#!/bin/bash

case "$OSTYPE" in
  *msys)
    ZENC="$PWD/bin/zenc.out"
    echo "REMOVING THE \"$ZENC\" FILE"
    rm $ZENC
    result=$?
    if (( result == 0 )); then
      echo "REMOVED \"$ZENC\" FILE"
    else
      echo "CANNOT REMOVE THE \"$ZENC\" FILE"
    fi
    ;;
  *)
    ZENC="$PWD/bin/zenc"
    echo "REMOVING THE \"$ZENC\" FILE"
    rm $ZENC
    result=$?
    if (( result == 0 )); then
      echo "REMOVED \"$ZENC\" FILE"
    else
      echo "CANNOT REMOVE THE \"$ZENC\" FILE"
    fi
    ;;
esac
