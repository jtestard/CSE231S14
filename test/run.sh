#!/bin/bash
source startenv.sh
export CSE231SRC="${CSE231ROOT}/llvm/src/lib/CSE231"
export INSTRUMENTATION="${CSE231SRC}/instrumentation"
export TEST="${CSE231SRC}/test"
export PROJ2BENCHMARKS="${CSE231SRC}/benchmarks"
export PROJ2RANGEANALYSISBENCHMARKS="${TEST}/rangeanalysis"
make all

#Compile benchmarks
echo "COMPILING BENCHMARKS"
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.cpp -o $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc
llvm-dis $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc

#Dummy optimization
#echo "DUMMY OPTIMIZATION"
#opt -load $LLVMLIB/CSE231.so -dummyOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze

echo "POINTER ANALYSIS OPTIMIZATION"
opt -load $LLVMLIB/CSE231.so -pointerAnalysisOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze

#echo "RANGE ANALYSIS OPTIMIZATION"
#clang -O0 -emit-llvm -c $PROJ2RANGEANALYSISBENCHMARKS/rangeanalysis.cpp -o $PROJ2RANGEANALYSISBENCHMARKS/rangeanalysis.bc
#llvm-dis $PROJ2RANGEANALYSISBENCHMARKS/rangeanalysis.bc #will give human readable version
#opt -mem2reg $PROJ2RANGEANALYSISBENCHMARKS/rangeanalysis.bc > $PROJ2RANGEANALYSISBENCHMARKS/memrangeanalysis.bc #get rid of stack vars
#llvm-dis $PROJ2RANGEANALYSISBENCHMARKS/memrangeanalysis.bc #get human readable version


#echo "DUMMY CONST PROP"
#opt -load $LLVMLIB/CSE231.so -ConstantPropOptimization < $PROJ2BENCHMARKS/constantProp/simplecp.bc -analyze

#opt -load $LLVMLIB/LLVMHello.so -mem2reg < $PROJ2BENCHMARKS/constantProp/simplecp.bc > $PROJ2BENCHMARKS/constantProp/out.opt
#mv $PROJ2BENCHMARKS/constantProp/out.opt $PROJ2BENCHMARKS/constantProp/out.bc
#llvm-dis $PROJ2BENCHMARKS/constantProp/out.bc
