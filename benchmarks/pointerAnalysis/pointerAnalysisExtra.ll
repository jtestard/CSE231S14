; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/pointerAnalysis/pointerAnalysisExtra.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

%struct.Node = type { i8, i8* }

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32*, align 8
  %c = alloca i32*, align 8
  %n = alloca %struct.Node, align 8
  %n_ptr = alloca %struct.Node*, align 8
  %d = alloca i8, align 1
  store i32 0, i32* %1
  store i32 2, i32* %a, align 4
  store i32* %a, i32** %b, align 8
  %2 = load i32** %b, align 8
  %3 = getelementptr inbounds i32* %2, i64 1
  store i32* %3, i32** %c, align 8
  store %struct.Node* %n, %struct.Node** %n_ptr, align 8
  store i8 98, i8* %d, align 1
  %4 = getelementptr inbounds %struct.Node* %n, i32 0, i32 1
  store i8* %d, i8** %4, align 8
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
