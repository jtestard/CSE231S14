; ModuleID = '/home/xhrdx/Desktop/cse231-proj1/llvm/src/lib/CSE231/test/../../../../../llvm/src/lib/CSE231/benchmarks/constantProp/out.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@llvm.global_ctors = appending global [1 x { i32, void ()* }] [{ i32, void ()* } { i32 65535, void ()* @_GLOBAL__I_a }]

define internal void @__cxx_global_var_init() section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %tmp = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #1
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) #0

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) #0

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main() #2 {
entry:
  %cmp = fcmp oeq float 0x4214000000000000, 6.000000e+00
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %add = fadd float 0x4214000000000000, 0x4249000000000000
  %conv = fptosi float %add to i16
  %add1 = fadd float 0x4214000000000000, 0x4249000000000000
  %conv2 = fptosi float %add1 to i16
  %add3 = fadd float 0x4214000000000000, 0x4249000000000000
  %conv4 = fptosi float %add3 to i16
  %conv5 = sext i16 %conv4 to i32
  %add6 = add nsw i32 %conv5, 10
  store i32 %add6, i32* undef, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %add7 = fadd float 0x4214000000000000, 0x4249000000000000
  %conv8 = fptosi float %add7 to i16
  %conv9 = sext i16 %conv8 to i32
  %add10 = add nsw i32 %conv9, 9
  store i32 %add10, i32* undef, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %c.0 = phi i16 [ %conv4, %if.then ], [ %conv8, %if.else ]
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %temp.0 = phi i32 [ 0, %if.end ], [ %inc, %for.inc ]
  %i.0 = phi i32 [ 0, %if.end ], [ %inc12, %for.inc ]
  %cmp11 = icmp slt i32 %i.0, 10
  br i1 %cmp11, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %inc = add nsw i32 %temp.0, 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc12 = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %conv13 = zext i16 2 to i32
  %shr = ashr i32 %conv13, 2
  %conv14 = trunc i32 %shr to i16
  %conv15 = sext i16 %c.0 to i32
  %add16 = add nsw i32 %conv15, 9
  %conv17 = sext i16 %c.0 to i32
  %add18 = add nsw i32 10, %conv17
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
