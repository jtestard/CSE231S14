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

echo "HADAMARD EXAMPLE"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/hadamard/hadamard.bc -analyze >> $OUTPUTLOGS/hadamard.static.log

echo "------------------------"
echo "PART 2"
echo "------------------------"
echo "WELCOME EXAMPLE (LINK FIRST)"
CPPFLAGS=
LDFLAGS=
echo "WELCOME EXAMPLE"
clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/dynamic/helper.cpp -o $INSTRUMENTATION/dynamic/helper.bc
llvm-link $INSTRUMENTATION/dynamic/helper.bc $BENCHMARKS/welcome/welcome.bc -o $BENCHMARKS/welcome/welcome.linked.bc
opt -load $LLVMLIB/CSE231.so -dynamicInstructionCount < $BENCHMARKS/welcome/welcome.linked.bc > $BENCHMARKS/welcome/welcome_instrumented.bc
llc -filetype=obj $BENCHMARKS/welcome/welcome_instrumented.bc -o=$BENCHMARKS/welcome/welcome.o
g++ $BENCHMARKS/welcome/welcome.o $LDFLAGS -o $BENCHMARKS/welcome/welcome
$BENCHMARKS/welcome/welcome >> $OUTPUTLOGS/welcome.dynamic.log

echo "COMPRESSION EXAMPLE"
clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/dynamic/helper.cpp -o $INSTRUMENTATION/dynamic/helper.bc
llvm-link $INSTRUMENTATION/dynamic/helper.bc $BENCHMARKS/compression/compression.bc -o $BENCHMARKS/compression/compression.linked.bc
opt -load $LLVMLIB/CSE231.so -dynamicInstructionCount < $BENCHMARKS/compression/compression.linked.bc > $BENCHMARKS/compression/compression_instrumented.bc
llc -filetype=obj $BENCHMARKS/compression/compression_instrumented.bc -o=$BENCHMARKS/compression/compression.o
g++ $BENCHMARKS/compression/compression.o $LDFLAGS -o $BENCHMARKS/compression/compression
$BENCHMARKS/compression/compression >> $OUTPUTLOGS/compression.dynamic.log

echo "GCD EXAMPLE"
clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/dynamic/helper.cpp -o $INSTRUMENTATION/dynamic/helper.bc
llvm-link $INSTRUMENTATION/dynamic/helper.bc $BENCHMARKS/gcd/gcd.bc -o $BENCHMARKS/gcd/gcd.linked.bc
opt -load $LLVMLIB/CSE231.so -dynamicInstructionCount < $BENCHMARKS/gcd/gcd.linked.bc > $BENCHMARKS/gcd/gcd_instrumented.bc
llc -filetype=obj $BENCHMARKS/gcd/gcd_instrumented.bc -o=$BENCHMARKS/gcd/gcd.o
g++ $BENCHMARKS/gcd/gcd.o $LDFLAGS -o $BENCHMARKS/gcd/gcd
$BENCHMARKS/gcd/gcd >> $OUTPUTLOGS/gcd.dynamic.log

echo "HADAMARD EXAMPLE"
clang $CPPFLAGS -O0 -emit-llvm -c $INSTRUMENTATION/dynamic/helper.cpp -o $INSTRUMENTATION/dynamic/helper.bc
llvm-link $INSTRUMENTATION/dynamic/helper.bc $BENCHMARKS/hadamard/hadamard.bc -o $BENCHMARKS/hadamard/hadamard.linked.bc
opt -load $LLVMLIB/CSE231.so -dynamicInstructionCount < $BENCHMARKS/hadamard/hadamard.linked.bc > $BENCHMARKS/hadamard/hadamard_instrumented.bc
llc -filetype=obj $BENCHMARKS/hadamard/hadamard_instrumented.bc -o=$BENCHMARKS/hadamard/hadamard.o
g++ $BENCHMARKS/hadamard/hadamard.o $LDFLAGS -o $BENCHMARKS/hadamard/hadamard
$BENCHMARKS/hadamard/hadamard >> $OUTPUTLOGS/hadamard/hadamard.dynamic.log