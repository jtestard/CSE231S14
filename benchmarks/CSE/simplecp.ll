; ModuleID = '/home/xhrdx/Desktop/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/CSE/simplecp.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@llvm.global_ctors = appending global [1 x { i32, void ()* }] [{ i32, void ()* } { i32 65535, void ()* @_GLOBAL__I_a }]

define internal void @__cxx_global_var_init() section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #1
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) #0

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) #0

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main() #2 {
entry:
  %retval = alloca i32, align 4
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
  store i32 0, i32* %retval
  store float 0x4015333340000000, float* %a, align 4
  store float 0x402E9999A0000000, float* %b, align 4
  %0 = load float* %a, align 4
  %cmp = fcmp oeq float %0, 6.000000e+00
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load float* %a, align 4
  %2 = load float* %b, align 4
  %add = fadd float %1, %2
  %conv = fptosi float %add to i32
  store i32 %conv, i32* %c, align 4
  %3 = load i32* %c, align 4
  %add1 = add nsw i32 %3, 10
  store i32 %add1, i32* %d, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %4 = load float* %a, align 4
  %5 = load float* %b, align 4
  %add2 = fadd float %4, %5
  %conv3 = fptosi float %add2 to i32
  store i32 %conv3, i32* %c, align 4
  %6 = load i32* %c, align 4
  %add4 = add nsw i32 %6, 10
  store i32 %add4, i32* %d, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  store i16 2, i16* %short1, align 2
  %7 = load i16* %short1, align 2
  %conv5 = zext i16 %7 to i32
  %shr = ashr i32 %conv5, 2
  %conv6 = trunc i32 %shr to i16
  store i16 %conv6, i16* %short2, align 2
  %8 = load i32* %c, align 4
  %9 = load i32* %d, align 4
  %add7 = add nsw i32 %8, %9
  store i32 %add7, i32* %h, align 4
  %10 = load i32* %d, align 4
  store i32 %10, i32* %ff, align 4
  %11 = load i32* %c, align 4
  %12 = load i32* %d, align 4
  %add8 = add nsw i32 %11, %12
  store i32 %add8, i32* %f, align 4
  %13 = load i32* %f, align 4
  %14 = load i32* %d, align 4
  %add9 = add nsw i32 %13, %14
  store i32 %add9, i32* %g, align 4
  ret i32 0
}

define internal void @_GLOBAL__I_a() section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
