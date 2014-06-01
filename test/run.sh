#!/bin/bash
source startenv.sh
export CSE231SRC="${CSE231ROOT}/llvm/src/lib/CSE231"
export INSTRUMENTATION="${CSE231SRC}/instrumentation"
export TEST="${CSE231SRC}/test"
export PROJ2BENCHMARKS="${CSE231SRC}/benchmarks"
make all

#Compile benchmarks
echo "COMPILING BENCHMARKS"
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.cpp -o $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc
llvm-dis $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc

#Dummy optimization
echo "DUMMY OPTIMIZATION"
opt -load $LLVMLIB/CSE231.so -dummyOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze

echo "POINTER ANALYSIS OPTIMIZATION"
opt -load $LLVMLIB/CSE231.so -pointerAnalysisOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze

#echo "DUMMY CONST PROP"
#opt -load $LLVMLIB/CSE231.so -ConstantPropOptimization < $PROJ2BENCHMARKS/constantProp/simplecp.bc -analyze

#opt -load $LLVMLIB/LLVMHello.so -mem2reg < $PROJ2BENCHMARKS/constantProp/simplecp.bc > $PROJ2BENCHMARKS/constantProp/out.opt
#mv $PROJ2BENCHMARKS/constantProp/out.opt $PROJ2BENCHMARKS/constantProp/out.bc
#llvm-dis $PROJ2BENCHMARKS/constantProp/out.bc
