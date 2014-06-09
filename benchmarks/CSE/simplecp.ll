; ModuleID = './../../../../../llvm/src/lib/CSE231/benchmarks/CSE/simplecp.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %g = alloca i32, align 4
  %f = alloca i32, align 4
  %h = alloca i32, align 4
  %short1 = alloca i16, align 2
  %short2 = alloca i16, align 2
  %ff = alloca i32, align 4
  store i32 0, i32* %1
  store float 0x4015333340000000, float* %a, align 4
  store float 0x402E9999A0000000, float* %b, align 4
  store i16 2, i16* %short1, align 2
  %2 = load i16* %short1, align 2
  %3 = zext i16 %2 to i32
  %4 = ashr i32 %3, 2
  %5 = trunc i32 %4 to i16
  store i16 %5, i16* %short2, align 2
  %6 = load float* %a, align 4
  %7 = load float* %b, align 4
  %8 = fadd float %6, %7
  %9 = fptosi float %8 to i32
  store i32 %9, i32* %c, align 4
  %10 = load i32* %c, align 4
  %11 = add nsw i32 %10, 9
  store i32 %11, i32* %d, align 4
  %12 = load i32* %c, align 4
  %13 = load i32* %d, align 4
  %14 = add nsw i32 %12, %13
  store i32 %14, i32* %h, align 4
  %15 = load i32* %d, align 4
  store i32 %15, i32* %ff, align 4
  %16 = load i32* %c, align 4
  %17 = load i32* %d, align 4
  %18 = add nsw i32 %16, %17
  store i32 %18, i32* %f, align 4
  %19 = load i32* %f, align 4
  %20 = load i32* %d, align 4
  %21 = add nsw i32 %19, %20
  store i32 %21, i32* %g, align 4
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
