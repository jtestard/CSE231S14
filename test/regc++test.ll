; ModuleID = 'regc++test.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_get"* }
%"class.std::ios_base" = type { i32 (...)**, i64, i64, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i64 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i64, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type { i32 (...)**, i32 }
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type { %"class.std::locale::facet", %struct.__locale_struct*, i8, i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8 }
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet" }
%"class.std::num_get" = type { %"class.std::locale::facet" }
%"class.std::basic_string" = type { %"struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider" }
%"struct.std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Alloc_hider" = type { i8* }
%"class.std::allocator" = type { i8 }
%class.Variable = type { %"class.std::basic_string" }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@.str = private unnamed_addr constant [8 x i8] c"example\00", align 1
@_ZSt4cout = external global %"class.std::basic_ostream"
@.str1 = private unnamed_addr constant [36 x i8] c"I created a new variable with name \00", align 1
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

; Function Attrs: uwtable
define i32 @main() #2 {
  call void @_Z12testVariablev()
  ret i32 0
}

; Function Attrs: uwtable
define void @_Z12testVariablev() #2 {
  %example = alloca %"class.std::basic_string", align 8
  %1 = alloca %"class.std::allocator", align 1
  %v = alloca %class.Variable, align 8
  %2 = alloca %"class.std::basic_string", align 8
  %3 = alloca %"class.std::basic_string", align 8
  call void @_ZNSaIcEC1Ev(%"class.std::allocator"* %1) #1
  invoke void @_ZNSsC1EPKcRKSaIcE(%"class.std::basic_string"* %example, i8* getelementptr inbounds ([8 x i8]* @.str, i32 0, i32 0), %"class.std::allocator"* %1)
          to label %4 unwind label %17

; <label>:4                                       ; preds = %0
  call void @_ZNSaIcED1Ev(%"class.std::allocator"* %1) #1
  invoke void @_ZNSsC1ERKSs(%"class.std::basic_string"* %2, %"class.std::basic_string"* %example)
          to label %5 unwind label %21

; <label>:5                                       ; preds = %4
  invoke void @_ZN8VariableC1ESs(%class.Variable* %v, %"class.std::basic_string"* %2)
          to label %6 unwind label %25

; <label>:6                                       ; preds = %5
  invoke void @_ZNSsD1Ev(%"class.std::basic_string"* %2)
          to label %7 unwind label %21

; <label>:7                                       ; preds = %6
  %8 = invoke %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* @_ZSt4cout, i8* getelementptr inbounds ([36 x i8]* @.str1, i32 0, i32 0))
          to label %9 unwind label %30

; <label>:9                                       ; preds = %7
  invoke void @_ZN8Variable7GetNameEv(%"class.std::basic_string"* sret %3, %class.Variable* %v)
          to label %10 unwind label %30

; <label>:10                                      ; preds = %9
  %11 = invoke %"class.std::basic_ostream"* @_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKSbIS4_S5_T1_E(%"class.std::basic_ostream"* %8, %"class.std::basic_string"* %3)
          to label %12 unwind label %34

; <label>:12                                      ; preds = %10
  %13 = invoke %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* %11, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
          to label %14 unwind label %34

; <label>:14                                      ; preds = %12
  invoke void @_ZNSsD1Ev(%"class.std::basic_string"* %3)
          to label %15 unwind label %30

; <label>:15                                      ; preds = %14
  invoke void @_ZN8VariableD1Ev(%class.Variable* %v)
          to label %16 unwind label %21

; <label>:16                                      ; preds = %15
  call void @_ZNSsD1Ev(%"class.std::basic_string"* %example)
  ret void

; <label>:17                                      ; preds = %0
  %18 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          cleanup
  %19 = extractvalue { i8*, i32 } %18, 0
  %20 = extractvalue { i8*, i32 } %18, 1
  call void @_ZNSaIcED1Ev(%"class.std::allocator"* %1) #1
  br label %43

; <label>:21                                      ; preds = %15, %6, %4
  %22 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          cleanup
  %23 = extractvalue { i8*, i32 } %22, 0
  %24 = extractvalue { i8*, i32 } %22, 1
  br label %41

; <label>:25                                      ; preds = %5
  %26 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          cleanup
  %27 = extractvalue { i8*, i32 } %26, 0
  %28 = extractvalue { i8*, i32 } %26, 1
  invoke void @_ZNSsD1Ev(%"class.std::basic_string"* %2)
          to label %29 unwind label %46

; <label>:29                                      ; preds = %25
  br label %41

; <label>:30                                      ; preds = %14, %9, %7
  %31 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          cleanup
  %32 = extractvalue { i8*, i32 } %31, 0
  %33 = extractvalue { i8*, i32 } %31, 1
  br label %39

; <label>:34                                      ; preds = %12, %10
  %35 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          cleanup
  %36 = extractvalue { i8*, i32 } %35, 0
  %37 = extractvalue { i8*, i32 } %35, 1
  invoke void @_ZNSsD1Ev(%"class.std::basic_string"* %3)
          to label %38 unwind label %46

; <label>:38                                      ; preds = %34
  br label %39

; <label>:39                                      ; preds = %38, %30
  %.01 = phi i32 [ %33, %30 ], [ %37, %38 ]
  %.0 = phi i8* [ %32, %30 ], [ %36, %38 ]
  invoke void @_ZN8VariableD1Ev(%class.Variable* %v)
          to label %40 unwind label %46

; <label>:40                                      ; preds = %39
  br label %41

; <label>:41                                      ; preds = %40, %29, %21
  %.12 = phi i32 [ %24, %21 ], [ %.01, %40 ], [ %28, %29 ]
  %.1 = phi i8* [ %23, %21 ], [ %.0, %40 ], [ %27, %29 ]
  invoke void @_ZNSsD1Ev(%"class.std::basic_string"* %example)
          to label %42 unwind label %46

; <label>:42                                      ; preds = %41
  br label %43

; <label>:43                                      ; preds = %42, %17
  %.23 = phi i32 [ %.12, %42 ], [ %20, %17 ]
  %.2 = phi i8* [ %.1, %42 ], [ %19, %17 ]
  %44 = insertvalue { i8*, i32 } undef, i8* %.2, 0
  %45 = insertvalue { i8*, i32 } %44, i32 %.23, 1
  resume { i8*, i32 } %45

; <label>:46                                      ; preds = %41, %39, %34, %25
  %47 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*)
          catch i8* null
  %48 = extractvalue { i8*, i32 } %47, 0
  call void @__clang_call_terminate(i8* %48) #5
  unreachable
}

declare void @_ZNSsC1EPKcRKSaIcE(%"class.std::basic_string"*, i8*, %"class.std::allocator"*) #0

; Function Attrs: nounwind
declare void @_ZNSaIcEC1Ev(%"class.std::allocator"*) #3

declare i32 @__gxx_personality_v0(...)

; Function Attrs: nounwind
declare void @_ZNSaIcED1Ev(%"class.std::allocator"*) #3

declare void @_ZN8VariableC1ESs(%class.Variable*, %"class.std::basic_string"*) #0

declare void @_ZNSsC1ERKSs(%"class.std::basic_string"*, %"class.std::basic_string"*) #0

declare void @_ZNSsD1Ev(%"class.std::basic_string"*) #0

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #4 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #1
  call void @_ZSt9terminatev() #5
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

declare %"class.std::basic_ostream"* @_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKSbIS4_S5_T1_E(%"class.std::basic_ostream"*, %"class.std::basic_string"*) #0

declare %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"*, i8*) #0

declare void @_ZN8Variable7GetNameEv(%"class.std::basic_string"* sret, %class.Variable*) #0

declare %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"*, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)*) #0

declare %"class.std::basic_ostream"* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_(%"class.std::basic_ostream"*) #0

declare void @_ZN8VariableD1Ev(%class.Variable*) #0

define internal void @_GLOBAL__I_a() section ".text.startup" {
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noinline noreturn nounwind }
attributes #5 = { noreturn nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"}
