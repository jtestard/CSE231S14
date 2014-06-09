; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/simplecp.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %temp = alloca i32, align 4
  %g = alloca i32, align 4
  %f = alloca i32, align 4
  %h = alloca i32, align 4
  %i = alloca i32, align 4
  %short1 = alloca i16, align 2
  %short2 = alloca i16, align 2
  store i32 0, i32* %1
  store i32 0, i32* %temp, align 4
  store i32 5, i32* %a, align 4
  store i32 15, i32* %b, align 4
  %2 = load i32* %a, align 4
  %3 = icmp eq i32 %2, 6
  br i1 %3, label %4, label %10

; <label>:4                                       ; preds = %0
  %5 = load i32* %a, align 4
  %6 = load i32* %b, align 4
  %7 = add nsw i32 %5, %6
  store i32 %7, i32* %c, align 4
  %8 = load i32* %c, align 4
  %9 = add nsw i32 %8, 10
  store i32 %9, i32* %d, align 4
  br label %16

; <label>:10                                      ; preds = %0
  %11 = load i32* %a, align 4
  %12 = load i32* %b, align 4
  %13 = add nsw i32 %11, %12
  store i32 %13, i32* %c, align 4
  %14 = load i32* %c, align 4
  %15 = add nsw i32 %14, 9
  store i32 %15, i32* %d, align 4
  br label %16

; <label>:16                                      ; preds = %10, %4
  store i32 0, i32* %i, align 4
  br label %17

; <label>:17                                      ; preds = %23, %16
  %18 = load i32* %i, align 4
  %19 = icmp slt i32 %18, 10
  br i1 %19, label %20, label %26

; <label>:20                                      ; preds = %17
  %21 = load i32* %temp, align 4
  %22 = add nsw i32 %21, 1
  store i32 %22, i32* %temp, align 4
  br label %23

; <label>:23                                      ; preds = %20
  %24 = load i32* %i, align 4
  %25 = add nsw i32 %24, 1
  store i32 %25, i32* %i, align 4
  br label %17

; <label>:26                                      ; preds = %17
  store i16 2, i16* %short1, align 2
  %27 = load i16* %short1, align 2
  %28 = zext i16 %27 to i32
  %29 = ashr i32 %28, 2
  %30 = trunc i32 %29 to i16
  store i16 %30, i16* %short2, align 2
  %31 = load i32* %c, align 4
  %32 = add nsw i32 %31, 9
  store i32 %32, i32* %h, align 4
  %33 = load i32* %d, align 4
  %34 = load i32* %c, align 4
  %35 = add nsw i32 %33, %34
  store i32 %35, i32* %h, align 4
  %36 = load i32* %c, align 4
  %37 = add nsw i32 10, %36
  store i32 %37, i32* %f, align 4
  %38 = load i32* %f, align 4
  %39 = load i32* %d, align 4
  %40 = add nsw i32 %38, %39
  store i32 %40, i32* %g, align 4
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
