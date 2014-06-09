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
  store i16 2, i16* %short1, align 2
  %0 = load i16* %short1, align 2
  %conv = zext i16 %0 to i32
  %shr = ashr i32 %conv, 2
  %conv1 = trunc i32 %shr to i16
  store i16 %conv1, i16* %short2, align 2
  %1 = load float* %a, align 4
  %2 = load float* %b, align 4
  %add = fadd float %1, %2
  %conv2 = fptosi float %add to i32
  store i32 %conv2, i32* %c, align 4
  %3 = load i32* %c, align 4
  %add3 = add nsw i32 %3, 9
  store i32 %add3, i32* %d, align 4
  %4 = load i32* %c, align 4
  %5 = load i32* %d, align 4
  %add4 = add nsw i32 %4, %5
  store i32 %add4, i32* %h, align 4
  %6 = load i32* %d, align 4
  store i32 %6, i32* %ff, align 4
  %7 = load i32* %c, align 4
  %8 = load i32* %d, align 4
  %add5 = add nsw i32 %7, %8
  store i32 %add5, i32* %f, align 4
  %9 = load i32* %f, align 4
  %10 = load i32* %d, align 4
  %add6 = add nsw i32 %9, %10
  store i32 %add6, i32* %g, align 4
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
