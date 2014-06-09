; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/out.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = icmp eq i32 5, 6
  br i1 %1, label %2, label %5

; <label>:2                                       ; preds = %0
  %3 = add nsw i32 5, 15
  %4 = add nsw i32 %3, 10
  br label %8

; <label>:5                                       ; preds = %0
  %6 = add nsw i32 5, 15
  %7 = add nsw i32 %6, 9
  br label %8

; <label>:8                                       ; preds = %5, %2
  %c.0 = phi i32 [ %3, %2 ], [ %6, %5 ]
  %d.0 = phi i32 [ %4, %2 ], [ %7, %5 ]
  br label %9

; <label>:9                                       ; preds = %13, %8
  %temp.0 = phi i32 [ 0, %8 ], [ %12, %13 ]
  %i.0 = phi i32 [ 0, %8 ], [ %14, %13 ]
  %10 = icmp slt i32 %i.0, 10
  br i1 %10, label %11, label %15

; <label>:11                                      ; preds = %9
  %12 = add nsw i32 %temp.0, 1
  br label %13

; <label>:13                                      ; preds = %11
  %14 = add nsw i32 %i.0, 1
  br label %9

; <label>:15                                      ; preds = %9
  %16 = zext i16 2 to i32
  %17 = ashr i32 %16, 2
  %18 = trunc i32 %17 to i16
  %19 = add nsw i32 %c.0, 9
  %20 = add nsw i32 %d.0, %c.0
  %21 = add nsw i32 10, %c.0
  %22 = add nsw i32 %21, %d.0
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
