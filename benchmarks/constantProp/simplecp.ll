; ModuleID = '/home/xhrdx/Desktop/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/simplecp.bc'
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
  %temp = alloca i32, align 4
  %g = alloca i32, align 4
  %f = alloca i32, align 4
  %h = alloca i32, align 4
  %i = alloca i32, align 4
  %short1 = alloca i16, align 2
  %short2 = alloca i16, align 2
  store i32 0, i32* %retval
  store i32 0, i32* %temp, align 4
  store float 5.000000e+00, float* %a, align 4
  store float 1.500000e+01, float* %b, align 4
  %0 = load float* %a, align 4
  %cmp = fcmp oeq float %0, 6.000000e+00
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load float* %a, align 4
  %2 = load float* %b, align 4
  %add = fadd float %1, %2
  %conv = fptosi float %add to i32
  store i32 %conv, i32* %c, align 4
  %3 = load float* %a, align 4
  %4 = load float* %b, align 4
  %add1 = fadd float %3, %4
  %conv2 = fptosi float %add1 to i32
  store i32 %conv2, i32* %c, align 4
  %5 = load float* %a, align 4
  %6 = load float* %b, align 4
  %add3 = fadd float %5, %6
  %conv4 = fptosi float %add3 to i32
  store i32 %conv4, i32* %c, align 4
  %7 = load i32* %c, align 4
  %add5 = add nsw i32 %7, 10
  store i32 %add5, i32* %d, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %8 = load float* %a, align 4
  %9 = load float* %b, align 4
  %add6 = fadd float %8, %9
  %conv7 = fptosi float %add6 to i32
  store i32 %conv7, i32* %c, align 4
  %10 = load i32* %c, align 4
  %add8 = add nsw i32 %10, 9
  store i32 %add8, i32* %d, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %11 = load i32* %i, align 4
  %cmp9 = icmp slt i32 %11, 10
  br i1 %cmp9, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %12 = load i32* %temp, align 4
  %inc = add nsw i32 %12, 1
  store i32 %inc, i32* %temp, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %13 = load i32* %i, align 4
  %inc10 = add nsw i32 %13, 1
  store i32 %inc10, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i16 2, i16* %short1, align 2
  %14 = load i16* %short1, align 2
  %conv11 = zext i16 %14 to i32
  %shr = ashr i32 %conv11, 2
  %conv12 = trunc i32 %shr to i16
  store i16 %conv12, i16* %short2, align 2
  %15 = load i32* %c, align 4
  %add13 = add nsw i32 %15, 9
  store i32 %add13, i32* %h, align 4
  %16 = load i32* %d, align 4
  %17 = load i32* %c, align 4
  %add14 = add nsw i32 %16, %17
  store i32 %add14, i32* %h, align 4
  %18 = load i32* %c, align 4
  %add15 = add nsw i32 10, %18
  store i32 %add15, i32* %f, align 4
  %19 = load i32* %f, align 4
  %20 = load i32* %d, align 4
  %add16 = add nsw i32 %19, %20
  store i32 %add16, i32* %g, align 4
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
