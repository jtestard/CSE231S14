; ModuleID = '/home/king/workspace/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/pointerAnalysis/pointerAnalysis.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca float*, align 8
  %d = alloca float, align 4
  %e = alloca float*, align 8
  %f = alloca float*, align 8
  %g = alloca float*, align 8
  %h = alloca float**, align 8
  %j = alloca float**, align 8
  %l = alloca float*, align 8
  %k = alloca i32, align 4
  store i32 0, i32* %1
  store float 2.000000e+00, float* %a, align 4
  store float 3.000000e+00, float* %b, align 4
  store float* %a, float** %c, align 8
  store float* %b, float** %c, align 8
  store float 5.000000e-01, float* %d, align 4
  store float* %d, float** %e, align 8
  store float* %b, float** %l, align 8
  %2 = load float** %e, align 8
  store float* %2, float** %f, align 8
  %3 = load float** %f, align 8
  store float* %3, float** %g, align 8
  store float** %f, float*** %h, align 8
  store float* %a, float** %e, align 8
  %4 = load float** %e, align 8
  %5 = load float*** %h, align 8
  store float* %4, float** %5, align 8
  store float** %l, float*** %j, align 8
  %6 = load float*** %j, align 8
  %7 = load float** %6, align 8
  store float* %7, float** %f, align 8
  %8 = load float*** %j, align 8
  %9 = load float** %8, align 8
  store float* %9, float** %e, align 8
  store float* null, float** %c, align 8
  %10 = load float* %a, align 4
  %11 = fcmp oeq float %10, 3.000000e+00
  br i1 %11, label %12, label %13

; <label>:12                                      ; preds = %0
  store float* %d, float** %e, align 8
  br label %14

; <label>:13                                      ; preds = %0
  store float* %d, float** %f, align 8
  br label %14

; <label>:14                                      ; preds = %13, %12
  store i32 0, i32* %k, align 4
  br label %15

; <label>:15                                      ; preds = %27, %14
  %16 = load i32* %k, align 4
  %17 = icmp slt i32 %16, 10
  br i1 %17, label %18, label %30

; <label>:18                                      ; preds = %15
  %19 = load i32* %k, align 4
  %20 = icmp eq i32 %19, 5
  br i1 %20, label %21, label %22

; <label>:21                                      ; preds = %18
  store float** %e, float*** %j, align 8
  br label %22

; <label>:22                                      ; preds = %21, %18
  %23 = load i32* %k, align 4
  %24 = icmp eq i32 %23, 9
  br i1 %24, label %25, label %26

; <label>:25                                      ; preds = %22
  store float** %f, float*** %j, align 8
  br label %26

; <label>:26                                      ; preds = %25, %22
  br label %27

; <label>:27                                      ; preds = %26
  %28 = load i32* %k, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %k, align 4
  br label %15

; <label>:30                                      ; preds = %15
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
