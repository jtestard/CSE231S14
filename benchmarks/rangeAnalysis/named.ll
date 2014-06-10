; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/rangeAnalysis/named.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = add nsw i32 5, 5
  %tmp1 = icmp eq i32 5, 5
  br i1 %tmp1, label %bb2, label %bb3

bb2:                                              ; preds = %bb
  br label %bb4

bb3:                                              ; preds = %bb
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %b.0 = phi i32 [ 2, %bb2 ], [ 3, %bb3 ]
  %tmp5 = mul nsw i32 %b.0, 2
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
