#!/bin/bash
source startenv.sh
export CSE231SRC="${CSE231ROOT}/llvm/src/lib/CSE231"
export INSTRUMENTATION="${CSE231SRC}/instrumentation"
make all
echo "------------------------"
echo "PART 1"
echo "------------------------"
echo "WELCOME EXAMPLE"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/welcome/welcome.bc -analyze >> $OUTPUTLOGS/welcome.static.log

echo "COMPRESSION EXAMPLE"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/compression/compression.bc -analyze >> $OUTPUTLOGS/compression.static.log

echo "GCD EXAMPLE"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/gcd/gcd.bc > /dev/null -analyze >> $OUTPUTLOGS/gcd.static.log

echo "HADMARD EXAMPLE"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/hadamard/hadamard.bc -analyze >> $OUTPUTLOGS/hadamard.static.log

echo "------------------------"
echo "PART 2"
echo "------------------------"
echo "WELCOME EXAMPLE (LINK FIRST)"
CPPFLAGS=
LDFLAGS=
clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/dynamic/helper.cpp -o $INSTRUMENTATION/dynamic/helper.bc
llvm-link $BENCHMARKS/welcome/welcome.bc $INSTRUMENTATION/dynamic/helper.bc -o $BENCHMARKS/welcome/welcome.linked.bc
opt -load $LLVMLIB/CSE231.so -dynamicInstructionCount < $BENCHMARKS/welcome/welcome.linked.bc > $BENCHMARKS/welcome/welcome_instrumented.bc
llc -filetype=obj $BENCHMARKS/welcome/welcome_instrumented.bc -o=$BENCHMARKS/welcome/welcome.o
g++ $BENCHMARKS/welcome/welcome.o $LDFLAGS -o $BENCHMARKS/welcome/welcome.exe
$BENCHMARKS/welcome/welcome.exe








#echo "WELCOME EXAMPLE (LINK AFTER)"
#clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/dynamic/helper.cpp -o $INSTRUMENTATION/dynamic/helper.bc
#opt -load $LLVMLIB/CSE231.so -dynamicInstructionCount < $BENCHMARKS/welcome/welcome.bc > $BENCHMARKS/welcome/welcome_instrumented.bc
#llvm-link $BENCHMARKS/welcome/welcome_instrumented.bc $INSTRUMENTATION/dynamic/helper.bc -o $BENCHMARKS/welcome/welcome.linked.bc
#llc -filetype=obj $BENCHMARKS/welcome/welcome.linked.bc -o=$BENCHMARKS/welcome/welcome.o
#g++ $BENCHMARKS/welcome/welcome.o $LDFLAGS -o $BENCHMARKS/welcome/welcome.exe
#$BENCHMARKS/welcome/welcome.exe