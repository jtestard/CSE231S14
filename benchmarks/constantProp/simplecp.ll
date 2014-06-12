; ModuleID = '/home/king/workspace/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/simplecp.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@llvm.global_ctors = appending global [1 x { i32, void ()* }] [{ i32, void ()* } { i32 65535, void ()* @_GLOBAL__I_a }]

define internal void @__cxx_global_var_init() section ".text.startup" {
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %1 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #1
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) #0

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) #0

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main() #2 {
  %1 = alloca i32, align 4
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca i16, align 2
  %d = alloca i32*, align 8
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
  store float 0x4214000000000000, float* %a, align 4
  store float 0x4249000000000000, float* %b, align 4
  %2 = load float* %a, align 4
  %3 = fcmp oeq float %2, 6.000000e+00
  br i1 %3, label %4, label %21

; <label>:4                                       ; preds = %0
  %5 = load float* %a, align 4
  %6 = load float* %b, align 4
  %7 = fadd float %5, %6
  %8 = fptosi float %7 to i16
  store i16 %8, i16* %c, align 2
  %9 = load float* %a, align 4
  %10 = load float* %b, align 4
  %11 = fadd float %9, %10
  %12 = fptosi float %11 to i16
  store i16 %12, i16* %c, align 2
  %13 = load float* %a, align 4
  %14 = load float* %b, align 4
  %15 = fadd float %13, %14
  %16 = fptosi float %15 to i16
  store i16 %16, i16* %c, align 2
  %17 = load i16* %c, align 2
  %18 = sext i16 %17 to i32
  %19 = add nsw i32 %18, 10
  %20 = load i32** %d, align 8
  store i32 %19, i32* %20, align 4
  br label %30

; <label>:21                                      ; preds = %0
  %22 = load float* %a, align 4
  %23 = load float* %b, align 4
  %24 = fadd float %22, %23
  %25 = fptosi float %24 to i16
  store i16 %25, i16* %c, align 2
  %26 = load i16* %c, align 2
  %27 = sext i16 %26 to i32
  %28 = add nsw i32 %27, 9
  %29 = load i32** %d, align 8
  store i32 %28, i32* %29, align 4
  br label %30

; <label>:30                                      ; preds = %21, %4
  store i32 0, i32* %i, align 4
  br label %31

; <label>:31                                      ; preds = %37, %30
  %32 = load i32* %i, align 4
  %33 = icmp slt i32 %32, 10
  br i1 %33, label %34, label %40

; <label>:34                                      ; preds = %31
  %35 = load i32* %temp, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, i32* %temp, align 4
  br label %37

; <label>:37                                      ; preds = %34
  %38 = load i32* %i, align 4
  %39 = add nsw i32 %38, 1
  store i32 %39, i32* %i, align 4
  br label %31

; <label>:40                                      ; preds = %31
  store i16 2, i16* %short1, align 2
  %41 = load i16* %short1, align 2
  %42 = zext i16 %41 to i32
  %43 = ashr i32 %42, 2
  %44 = trunc i32 %43 to i16
  store i16 %44, i16* %short2, align 2
  %45 = load i16* %c, align 2
  %46 = sext i16 %45 to i32
  %47 = add nsw i32 %46, 9
  store i32 %47, i32* %h, align 4
  %48 = load i16* %c, align 2
  %49 = sext i16 %48 to i32
  %50 = add nsw i32 10, %49
  store i32 %50, i32* %f, align 4
  ret i32 0
}

define internal void @_GLOBAL__I_a() section ".text.startup" {
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
