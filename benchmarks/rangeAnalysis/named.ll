; ModuleID = '/home/king/workspace/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/rangeAnalysis/named.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
bb:
  %tmp = add nsw i32 100, 5
  %tmp1 = icmp eq i32 5, 5
  br i1 %tmp1, label %bb2, label %bb3

bb2:                                              ; preds = %bb
  br label %bb4

bb3:                                              ; preds = %bb
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %b.0 = phi i32 [ 2, %bb2 ], [ 10, %bb3 ]
  %tmp5 = mul nsw i32 %b.0, 50
  %tmp6 = sdiv i32 %b.0, 25
  %tmp7 = srem i32 %b.0, 25
  %tmp8 = add nsw i32 %b.0, 100
  %tmp9 = sub nsw i32 %b.0, 100
  %tmp10 = ashr i32 %b.0, 2
  %tmp11 = shl i32 %b.0, 2
  %tmp12 = and i32 %b.0, 5
  %tmp13 = or i32 %b.0, 5
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
