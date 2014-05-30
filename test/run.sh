#!/bin/bash
source startenv.sh
export CSE231SRC="${CSE231ROOT}/llvm/src/lib/CSE231"
export INSTRUMENTATION="${CSE231SRC}/instrumentation"
export TEST="${CSE231SRC}/test"
export PROJ2BENCHMARKS="${CSE231ROOT}/proj2/benchmarks"
make all

#Compile benchmarks
echo "COMPILING BENCHMARKS"
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.cpp -o $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc
llvm-dis $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc

#Dummy optimization
echo "DUMMY OPTIMIZATION"
opt -load $LLVMLIB/CSE231.so -dummyOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze