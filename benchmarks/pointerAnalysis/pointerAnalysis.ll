; ModuleID = '/home/xhrdx/Desktop/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/pointerAnalysis/pointerAnalysis.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
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
  store i32 0, i32* %retval
  store float 2.000000e+00, float* %a, align 4
  store float 3.000000e+00, float* %b, align 4
  store float* %a, float** %c, align 8
  store float* %b, float** %c, align 8
  store float 5.000000e-01, float* %d, align 4
  store float* %d, float** %e, align 8
  store float* %b, float** %l, align 8
  %0 = load float** %e, align 8
  store float* %0, float** %f, align 8
  %1 = load float** %f, align 8
  store float* %1, float** %g, align 8
  store float** %f, float*** %h, align 8
  store float* %a, float** %e, align 8
  %2 = load float** %e, align 8
  %3 = load float*** %h, align 8
  store float* %2, float** %3, align 8
  store float** %l, float*** %j, align 8
  %4 = load float*** %j, align 8
  %5 = load float** %4, align 8
  store float* %5, float** %f, align 8
  %6 = load float*** %j, align 8
  %7 = load float** %6, align 8
  store float* %7, float** %e, align 8
  store float* null, float** %c, align 8
  %8 = load float* %a, align 4
  %cmp = fcmp oeq float %8, 3.000000e+00
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store float* %d, float** %e, align 8
  br label %if.end

if.else:                                          ; preds = %entry
  store float* %d, float** %f, align 8
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  store i32 0, i32* %k, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %9 = load i32* %k, align 4
  %cmp1 = icmp slt i32 %9, 10
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %10 = load i32* %k, align 4
  %cmp2 = icmp eq i32 %10, 5
  br i1 %cmp2, label %if.then3, label %if.end4

if.then3:                                         ; preds = %for.body
  store float** %e, float*** %j, align 8
  br label %if.end4

if.end4:                                          ; preds = %if.then3, %for.body
  %11 = load i32* %k, align 4
  %cmp5 = icmp eq i32 %11, 9
  br i1 %cmp5, label %if.then6, label %if.end7

if.then6:                                         ; preds = %if.end4
  store float** %f, float*** %j, align 8
  br label %if.end7

if.end7:                                          ; preds = %if.then6, %if.end4
  br label %for.inc

for.inc:                                          ; preds = %if.end7
  %12 = load i32* %k, align 4
  %inc = add nsw i32 %12, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
