; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/out.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = icmp eq i32 5, 6
  br i1 %1, label %2, label %6

; <label>:2                                       ; preds = %0
  %3 = add nsw i32 5, 15
  %4 = fadd float 1.500000e+01, 1.000000e+01
  %5 = fptosi float %4 to i32
  br label %10

; <label>:6                                       ; preds = %0
  %7 = add nsw i32 5, 15
  %8 = fadd float 1.500000e+01, 1.000000e+01
  %9 = fptosi float %8 to i32
  br label %10

; <label>:10                                      ; preds = %6, %2
  %d.0 = phi i32 [ %5, %2 ], [ %9, %6 ]
  %c.0 = phi i32 [ %3, %2 ], [ %7, %6 ]
  br label %11

; <label>:11                                      ; preds = %15, %10
  %temp.0 = phi i32 [ 0, %10 ], [ %14, %15 ]
  %i.0 = phi i32 [ 0, %10 ], [ %16, %15 ]
  %12 = icmp slt i32 %i.0, 10
  br i1 %12, label %13, label %17

; <label>:13                                      ; preds = %11
  %14 = add nsw i32 %temp.0, 1
  br label %15

; <label>:15                                      ; preds = %13
  %16 = add nsw i32 %i.0, 1
  br label %11

; <label>:17                                      ; preds = %11
  %18 = zext i16 2 to i32
  %19 = ashr i32 %18, 2
  %20 = trunc i32 %19 to i16
  %21 = add nsw i32 %c.0, 9
  %22 = add nsw i32 %d.0, %c.0
  %23 = add nsw i32 10, %c.0
  %24 = add nsw i32 %23, %d.0
  %25 = sitofp i32 %24 to float
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
