#!/bin/bash -eu

# Copy all fuzzer executables to $OUT/
$CXX $CFLAGS $LIB_FUZZING_ENGINE \
  $SRC/happly/.clusterfuzzlite/ply_fuzzer.cpp \
  -o $OUT/ply_fuzzer \
  -I$SRC/happly
