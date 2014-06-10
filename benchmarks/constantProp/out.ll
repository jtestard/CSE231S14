; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/out.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = icmp eq i32 5, 6
  br i1 %tmp, label %bb1, label %bb4

bb1:                                              ; preds = %bb
  %tmp2 = add nsw i32 5, 15
  %tmp3 = add nsw i32 %tmp2, 10
  br label %bb7

bb4:                                              ; preds = %bb
  %tmp5 = add nsw i32 5, 15
  %tmp6 = add nsw i32 %tmp5, 9
  br label %bb7

bb7:                                              ; preds = %bb4, %bb1
  %c.0 = phi i32 [ %tmp2, %bb1 ], [ %tmp5, %bb4 ]
  %d.0 = phi i32 [ %tmp3, %bb1 ], [ %tmp6, %bb4 ]
  br label %bb8

bb8:                                              ; preds = %bb12, %bb7
  %temp.0 = phi i32 [ 0, %bb7 ], [ %tmp11, %bb12 ]
  %i.0 = phi i32 [ 0, %bb7 ], [ %tmp13, %bb12 ]
  %tmp9 = icmp slt i32 %i.0, 10
  br i1 %tmp9, label %bb10, label %bb14

bb10:                                             ; preds = %bb8
  %tmp11 = add nsw i32 %temp.0, 1
  br label %bb12

bb12:                                             ; preds = %bb10
  %tmp13 = add nsw i32 %i.0, 1
  br label %bb8

bb14:                                             ; preds = %bb8
  %tmp15 = zext i16 2 to i32
  %tmp16 = ashr i32 %tmp15, 2
  %tmp17 = trunc i32 %tmp16 to i16
  %tmp18 = add nsw i32 %c.0, 9
  %tmp19 = add nsw i32 %d.0, %c.0
  %tmp20 = add nsw i32 10, %c.0
  %tmp21 = add nsw i32 %tmp20, %d.0
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
