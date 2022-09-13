#!/bin/sh

make

for i in *.c; do
  x1=`./edonr-old < $i`
  x2=`./edonr-new < $i`
  echo "$x1 ($i)"
  if [ "$x1" != "$x2" ]; then
    echo "$x1 != $x2 @ file $i"
  fi
done
