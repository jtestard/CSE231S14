#!/bin/bash
source startenv.sh
export CSE231SRC="${CSE231ROOT}/llvm/src/lib/CSE231"
export INSTRUMENTATION="${CSE231SRC}/instrumentation"
export TEST="${CSE231SRC}/test"
make all

#Dummy optimization
echo "DUMMY OPTIMIZATION"
opt -load $LLVMLIB/CSE231.so -dummyOptimization < $BENCHMARKS/welcome/welcome.bc -analyze