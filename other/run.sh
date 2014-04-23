#!/bin/bash
source startenv.sh
make llvm
echo "------------------------"
echo "PART 1"
echo "------------------------"
echo "---------------"
echo "WELCOME EXAMPLE"
echo "---------------"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/welcome/welcome.bc > /dev/null

echo "---------------"
echo "COMPRESSION EXAMPLE"
echo "---------------"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/compression/compression.bc > /dev/null

echo "---------------"
echo "GCD EXAMPLE"
echo "---------------"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/gcd/gcd.bc > /dev/null

echo "---------------"
echo "HADMARD EXAMPLE"
echo "---------------"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/hadamard/hadamard.bc > /dev/null