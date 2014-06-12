!/bin/bash
source startenv.sh
export CSE231SRC="${CSE231ROOT}/llvm/src/lib/CSE231"
export INSTRUMENTATION="${CSE231SRC}/instrumentation"
export TEST="${CSE231SRC}/test"
export PROJ2BENCHMARKS="${CSE231SRC}/benchmarks"
make all

#Compile benchmarks
#echo "COMPILING BENCHMARKS"
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.cpp -o $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysisExtra.cpp -o $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysisExtra.bc
llvm-dis $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc
llvm-dis $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysisExtra.bc
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/constantProp/simplecp.cpp -o $PROJ2BENCHMARKS/constantProp/simplecp.bc
llvm-dis $PROJ2BENCHMARKS/constantProp/simplecp.bc
clang -O0 -emit-llvm -c $PROJ2BENCHMARKS/CSE/simplecp.cpp -o $PROJ2BENCHMARKS/CSE/simplecp.bc
llvm-dis $PROJ2BENCHMARKS/CSE/simplecp.bc

#Dummy optimization
if [ "$1" == "dummy" ]
then
	#echo "DUMMY OPTIMIZATION"
	opt -load $LLVMLIB/CSE231.dylib -dummyOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze
fi

if [ "$1" == "pointerAnalysis" ]
then
	echo "POINTER ANALYSIS OPTIMIZATION"
	opt -load $LLVMLIB/CSE231.dylib -pointerAnalysisOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysis.bc -analyze >> $PROJ2BENCHMARKS/results/pointerAnalysisRegularTypes.output
	opt -load $LLVMLIB/CSE231.dylib -pointerAnalysisOptimization < $PROJ2BENCHMARKS/pointerAnalysis/pointerAnalysisExtra.bc -analyze >> $PROJ2BENCHMARKS/results/pointerAnalysisFancyTypes.output
fi

if [ "$1" == "constantPropagation" ]
then
	echo "CONSTANT PROPAGATION ANALYSIS OPTIMIZATION"
	opt -mem2reg -instnamer < $PROJ2BENCHMARKS/constantProp/simplecp.bc > $PROJ2BENCHMARKS/constantProp/out.opt
	mv $PROJ2BENCHMARKS/constantProp/out.opt $PROJ2BENCHMARKS/constantProp/out.bc
	llvm-dis $PROJ2BENCHMARKS/constantProp/out.bc
	opt -load $LLVMLIB/CSE231.dylib -ConstantPropAnalysisOptimization < $PROJ2BENCHMARKS/constantProp/out.bc -analyze >> $PROJ2BENCHMARKS/results/constantProp.output
fi
if [ "$1" == "CSE" ]
then
	echo "AVAILABLE EXPRESSIONS ANALYSIS OPTIMIZATION"
	opt -load -instnamer $LLVMLIB/LLVMHello.dylib -mem2reg < $PROJ2BENCHMARKS/CSE/simplecp.bc > $PROJ2BENCHMARKS/CSE/out.opt
	mv $PROJ2BENCHMARKS/CSE/out.opt $PROJ2BENCHMARKS/CSE/out.bc
	llvm-dis $PROJ2BENCHMARKS/CSE/out.bc
	opt -load $LLVMLIB/CSE231.dylib -AvailableExpressionAnalysisOptimization < $PROJ2BENCHMARKS/CSE/out.bc -analyze >> $PROJ2BENCHMARKS/results/AvailableExpressions.output
fi
if [ "$1" == "RangeAnalysis" ]
then
	echo "RANGE ANALYSIS"
	opt -mem2reg -instnamer $PROJ2BENCHMARKS/rangeAnalysis/simplecp.bc > $PROJ2BENCHMARKS/rangeAnalysis/named.bc
	opt -load $LLVMLIB/CSE231.dylib -rangeAnalysisOptimization < $PROJ2BENCHMARKS/rangeAnalysis/named.bc -analyze >> $PROJ2BENCHMARKS/results/RangeAnalysis.output
	llvm-dis $PROJ2BENCHMARKS/rangeAnalysis/named.bc
fi
