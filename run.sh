#!/bin/bash
source startenv.sh
make llvm
echo "---------------"
echo "WELCOME EXAMPLE"
echo "---------------"
opt -load $LLVMLIB/CSE231.so -staticInstructionCount < $BENCHMARKS/welcome/welcome.bc > /dev/null
