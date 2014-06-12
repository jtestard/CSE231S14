; ModuleID = 'priorityQueue.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.9.0"

%struct.pri_queue_t = type { %struct.q_elem_t*, i32, i32 }
%struct.q_elem_t = type { i8*, i32 }

@.str = private unnamed_addr constant [13 x i8] c"Clear drains\00", align 1
@.str1 = private unnamed_addr constant [9 x i8] c"Feed cat\00", align 1
@.str2 = private unnamed_addr constant [9 x i8] c"Make tea\00", align 1
@.str3 = private unnamed_addr constant [15 x i8] c"Solve RC tasks\00", align 1
@.str4 = private unnamed_addr constant [11 x i8] c"Tax return\00", align 1
@main.pri = private unnamed_addr constant [5 x i32] [i32 3, i32 4, i32 5, i32 1, i32 2], align 16
@.str5 = private unnamed_addr constant [8 x i8] c"%d: %s\0A\00", align 1
@.str6 = private unnamed_addr constant [34 x i8] c"\0Aq has %d items, q2 has %d items\0A\00", align 1
@.str7 = private unnamed_addr constant [46 x i8] c"After merge, q has %d items, q2 has %d items\0A\00", align 1
@.str8 = private unnamed_addr constant [26 x i8] c"Popped %d items out of q\0A\00", align 1

; Function Attrs: nounwind ssp uwtable
define %struct.pri_queue_t* @priq_new(i32 %size) #0 {
  %1 = alloca i32, align 4
  %q = alloca %struct.pri_queue_t*, align 8
  store i32 %size, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = icmp slt i32 %2, 4
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %0
  store i32 4, i32* %1, align 4
  br label %5

; <label>:5                                       ; preds = %4, %0
  %6 = call i8* @malloc(i64 16)
  %7 = bitcast i8* %6 to %struct.pri_queue_t*
  store %struct.pri_queue_t* %7, %struct.pri_queue_t** %q, align 8
  %8 = load i32* %1, align 4
  %9 = sext i32 %8 to i64
  %10 = mul i64 16, %9
  %11 = call i8* @malloc(i64 %10)
  %12 = bitcast i8* %11 to %struct.q_elem_t*
  %13 = load %struct.pri_queue_t** %q, align 8
  %14 = getelementptr inbounds %struct.pri_queue_t* %13, i32 0, i32 0
  store %struct.q_elem_t* %12, %struct.q_elem_t** %14, align 8
  %15 = load i32* %1, align 4
  %16 = load %struct.pri_queue_t** %q, align 8
  %17 = getelementptr inbounds %struct.pri_queue_t* %16, i32 0, i32 2
  store i32 %15, i32* %17, align 4
  %18 = load %struct.pri_queue_t** %q, align 8
  %19 = getelementptr inbounds %struct.pri_queue_t* %18, i32 0, i32 1
  store i32 1, i32* %19, align 4
  %20 = load %struct.pri_queue_t** %q, align 8
  ret %struct.pri_queue_t* %20
}

declare i8* @malloc(i64) #1

; Function Attrs: nounwind ssp uwtable
define void @priq_push(%struct.pri_queue_t* %q, i8* %data, i32 %pri) #0 {
  %1 = alloca %struct.pri_queue_t*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %b = alloca %struct.q_elem_t*, align 8
  %n = alloca i32, align 4
  %m = alloca i32, align 4
  store %struct.pri_queue_t* %q, %struct.pri_queue_t** %1, align 8
  store i8* %data, i8** %2, align 8
  store i32 %pri, i32* %3, align 4
  %4 = load %struct.pri_queue_t** %1, align 8
  %5 = getelementptr inbounds %struct.pri_queue_t* %4, i32 0, i32 1
  %6 = load i32* %5, align 4
  %7 = load %struct.pri_queue_t** %1, align 8
  %8 = getelementptr inbounds %struct.pri_queue_t* %7, i32 0, i32 2
  %9 = load i32* %8, align 4
  %10 = icmp sge i32 %6, %9
  br i1 %10, label %11, label %29

; <label>:11                                      ; preds = %0
  %12 = load %struct.pri_queue_t** %1, align 8
  %13 = getelementptr inbounds %struct.pri_queue_t* %12, i32 0, i32 2
  %14 = load i32* %13, align 4
  %15 = mul nsw i32 %14, 2
  store i32 %15, i32* %13, align 4
  %16 = load %struct.pri_queue_t** %1, align 8
  %17 = getelementptr inbounds %struct.pri_queue_t* %16, i32 0, i32 0
  %18 = load %struct.q_elem_t** %17, align 8
  %19 = bitcast %struct.q_elem_t* %18 to i8*
  %20 = load %struct.pri_queue_t** %1, align 8
  %21 = getelementptr inbounds %struct.pri_queue_t* %20, i32 0, i32 2
  %22 = load i32* %21, align 4
  %23 = sext i32 %22 to i64
  %24 = mul i64 16, %23
  %25 = call i8* @realloc(i8* %19, i64 %24)
  %26 = bitcast i8* %25 to %struct.q_elem_t*
  %27 = load %struct.pri_queue_t** %1, align 8
  %28 = getelementptr inbounds %struct.pri_queue_t* %27, i32 0, i32 0
  store %struct.q_elem_t* %26, %struct.q_elem_t** %28, align 8
  store %struct.q_elem_t* %26, %struct.q_elem_t** %b, align 8
  br label %33

; <label>:29                                      ; preds = %0
  %30 = load %struct.pri_queue_t** %1, align 8
  %31 = getelementptr inbounds %struct.pri_queue_t* %30, i32 0, i32 0
  %32 = load %struct.q_elem_t** %31, align 8
  store %struct.q_elem_t* %32, %struct.q_elem_t** %b, align 8
  br label %33

; <label>:33                                      ; preds = %29, %11
  %34 = load %struct.pri_queue_t** %1, align 8
  %35 = getelementptr inbounds %struct.pri_queue_t* %34, i32 0, i32 1
  %36 = load i32* %35, align 4
  %37 = add nsw i32 %36, 1
  store i32 %37, i32* %35, align 4
  store i32 %36, i32* %n, align 4
  br label %38

; <label>:38                                      ; preds = %53, %33
  %39 = load i32* %n, align 4
  %40 = sdiv i32 %39, 2
  store i32 %40, i32* %m, align 4
  %41 = icmp ne i32 %40, 0
  br i1 %41, label %42, label %51

; <label>:42                                      ; preds = %38
  %43 = load i32* %3, align 4
  %44 = load i32* %m, align 4
  %45 = sext i32 %44 to i64
  %46 = load %struct.q_elem_t** %b, align 8
  %47 = getelementptr inbounds %struct.q_elem_t* %46, i64 %45
  %48 = getelementptr inbounds %struct.q_elem_t* %47, i32 0, i32 1
  %49 = load i32* %48, align 4
  %50 = icmp slt i32 %43, %49
  br label %51

; <label>:51                                      ; preds = %42, %38
  %52 = phi i1 [ false, %38 ], [ %50, %42 ]
  br i1 %52, label %53, label %65

; <label>:53                                      ; preds = %51
  %54 = load i32* %n, align 4
  %55 = sext i32 %54 to i64
  %56 = load %struct.q_elem_t** %b, align 8
  %57 = getelementptr inbounds %struct.q_elem_t* %56, i64 %55
  %58 = load i32* %m, align 4
  %59 = sext i32 %58 to i64
  %60 = load %struct.q_elem_t** %b, align 8
  %61 = getelementptr inbounds %struct.q_elem_t* %60, i64 %59
  %62 = bitcast %struct.q_elem_t* %57 to i8*
  %63 = bitcast %struct.q_elem_t* %61 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %62, i8* %63, i64 16, i32 8, i1 false)
  %64 = load i32* %m, align 4
  store i32 %64, i32* %n, align 4
  br label %38

; <label>:65                                      ; preds = %51
  %66 = load i8** %2, align 8
  %67 = load i32* %n, align 4
  %68 = sext i32 %67 to i64
  %69 = load %struct.q_elem_t** %b, align 8
  %70 = getelementptr inbounds %struct.q_elem_t* %69, i64 %68
  %71 = getelementptr inbounds %struct.q_elem_t* %70, i32 0, i32 0
  store i8* %66, i8** %71, align 8
  %72 = load i32* %3, align 4
  %73 = load i32* %n, align 4
  %74 = sext i32 %73 to i64
  %75 = load %struct.q_elem_t** %b, align 8
  %76 = getelementptr inbounds %struct.q_elem_t* %75, i64 %74
  %77 = getelementptr inbounds %struct.q_elem_t* %76, i32 0, i32 1
  store i32 %72, i32* %77, align 4
  ret void
}

declare i8* @realloc(i8*, i64) #1

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind ssp uwtable
define i8* @priq_pop(%struct.pri_queue_t* %q, i32* %pri) #0 {
  %1 = alloca i8*, align 8
  %2 = alloca %struct.pri_queue_t*, align 8
  %3 = alloca i32*, align 8
  %out = alloca i8*, align 8
  %b = alloca %struct.q_elem_t*, align 8
  %n = alloca i32, align 4
  %m = alloca i32, align 4
  store %struct.pri_queue_t* %q, %struct.pri_queue_t** %2, align 8
  store i32* %pri, i32** %3, align 8
  %4 = load %struct.pri_queue_t** %2, align 8
  %5 = getelementptr inbounds %struct.pri_queue_t* %4, i32 0, i32 1
  %6 = load i32* %5, align 4
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %9

; <label>:8                                       ; preds = %0
  store i8* null, i8** %1
  br label %134

; <label>:9                                       ; preds = %0
  %10 = load %struct.pri_queue_t** %2, align 8
  %11 = getelementptr inbounds %struct.pri_queue_t* %10, i32 0, i32 0
  %12 = load %struct.q_elem_t** %11, align 8
  store %struct.q_elem_t* %12, %struct.q_elem_t** %b, align 8
  %13 = load %struct.q_elem_t** %b, align 8
  %14 = getelementptr inbounds %struct.q_elem_t* %13, i64 1
  %15 = getelementptr inbounds %struct.q_elem_t* %14, i32 0, i32 0
  %16 = load i8** %15, align 8
  store i8* %16, i8** %out, align 8
  %17 = load i32** %3, align 8
  %18 = icmp ne i32* %17, null
  br i1 %18, label %19, label %25

; <label>:19                                      ; preds = %9
  %20 = load %struct.q_elem_t** %b, align 8
  %21 = getelementptr inbounds %struct.q_elem_t* %20, i64 1
  %22 = getelementptr inbounds %struct.q_elem_t* %21, i32 0, i32 1
  %23 = load i32* %22, align 4
  %24 = load i32** %3, align 8
  store i32 %23, i32* %24, align 4
  br label %25

; <label>:25                                      ; preds = %19, %9
  %26 = load %struct.pri_queue_t** %2, align 8
  %27 = getelementptr inbounds %struct.pri_queue_t* %26, i32 0, i32 1
  %28 = load i32* %27, align 4
  %29 = add nsw i32 %28, -1
  store i32 %29, i32* %27, align 4
  store i32 1, i32* %n, align 4
  br label %30

; <label>:30                                      ; preds = %79, %25
  %31 = load i32* %n, align 4
  %32 = mul nsw i32 %31, 2
  store i32 %32, i32* %m, align 4
  %33 = load %struct.pri_queue_t** %2, align 8
  %34 = getelementptr inbounds %struct.pri_queue_t* %33, i32 0, i32 1
  %35 = load i32* %34, align 4
  %36 = icmp slt i32 %32, %35
  br i1 %36, label %37, label %91

; <label>:37                                      ; preds = %30
  %38 = load i32* %m, align 4
  %39 = add nsw i32 %38, 1
  %40 = load %struct.pri_queue_t** %2, align 8
  %41 = getelementptr inbounds %struct.pri_queue_t* %40, i32 0, i32 1
  %42 = load i32* %41, align 4
  %43 = icmp slt i32 %39, %42
  br i1 %43, label %44, label %62

; <label>:44                                      ; preds = %37
  %45 = load i32* %m, align 4
  %46 = sext i32 %45 to i64
  %47 = load %struct.q_elem_t** %b, align 8
  %48 = getelementptr inbounds %struct.q_elem_t* %47, i64 %46
  %49 = getelementptr inbounds %struct.q_elem_t* %48, i32 0, i32 1
  %50 = load i32* %49, align 4
  %51 = load i32* %m, align 4
  %52 = add nsw i32 %51, 1
  %53 = sext i32 %52 to i64
  %54 = load %struct.q_elem_t** %b, align 8
  %55 = getelementptr inbounds %struct.q_elem_t* %54, i64 %53
  %56 = getelementptr inbounds %struct.q_elem_t* %55, i32 0, i32 1
  %57 = load i32* %56, align 4
  %58 = icmp sgt i32 %50, %57
  br i1 %58, label %59, label %62

; <label>:59                                      ; preds = %44
  %60 = load i32* %m, align 4
  %61 = add nsw i32 %60, 1
  store i32 %61, i32* %m, align 4
  br label %62

; <label>:62                                      ; preds = %59, %44, %37
  %63 = load %struct.pri_queue_t** %2, align 8
  %64 = getelementptr inbounds %struct.pri_queue_t* %63, i32 0, i32 1
  %65 = load i32* %64, align 4
  %66 = sext i32 %65 to i64
  %67 = load %struct.q_elem_t** %b, align 8
  %68 = getelementptr inbounds %struct.q_elem_t* %67, i64 %66
  %69 = getelementptr inbounds %struct.q_elem_t* %68, i32 0, i32 1
  %70 = load i32* %69, align 4
  %71 = load i32* %m, align 4
  %72 = sext i32 %71 to i64
  %73 = load %struct.q_elem_t** %b, align 8
  %74 = getelementptr inbounds %struct.q_elem_t* %73, i64 %72
  %75 = getelementptr inbounds %struct.q_elem_t* %74, i32 0, i32 1
  %76 = load i32* %75, align 4
  %77 = icmp sle i32 %70, %76
  br i1 %77, label %78, label %79

; <label>:78                                      ; preds = %62
  br label %91

; <label>:79                                      ; preds = %62
  %80 = load i32* %n, align 4
  %81 = sext i32 %80 to i64
  %82 = load %struct.q_elem_t** %b, align 8
  %83 = getelementptr inbounds %struct.q_elem_t* %82, i64 %81
  %84 = load i32* %m, align 4
  %85 = sext i32 %84 to i64
  %86 = load %struct.q_elem_t** %b, align 8
  %87 = getelementptr inbounds %struct.q_elem_t* %86, i64 %85
  %88 = bitcast %struct.q_elem_t* %83 to i8*
  %89 = bitcast %struct.q_elem_t* %87 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %88, i8* %89, i64 16, i32 8, i1 false)
  %90 = load i32* %m, align 4
  store i32 %90, i32* %n, align 4
  br label %30

; <label>:91                                      ; preds = %78, %30
  %92 = load i32* %n, align 4
  %93 = sext i32 %92 to i64
  %94 = load %struct.q_elem_t** %b, align 8
  %95 = getelementptr inbounds %struct.q_elem_t* %94, i64 %93
  %96 = load %struct.pri_queue_t** %2, align 8
  %97 = getelementptr inbounds %struct.pri_queue_t* %96, i32 0, i32 1
  %98 = load i32* %97, align 4
  %99 = sext i32 %98 to i64
  %100 = load %struct.q_elem_t** %b, align 8
  %101 = getelementptr inbounds %struct.q_elem_t* %100, i64 %99
  %102 = bitcast %struct.q_elem_t* %95 to i8*
  %103 = bitcast %struct.q_elem_t* %101 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %102, i8* %103, i64 16, i32 8, i1 false)
  %104 = load %struct.pri_queue_t** %2, align 8
  %105 = getelementptr inbounds %struct.pri_queue_t* %104, i32 0, i32 1
  %106 = load i32* %105, align 4
  %107 = load %struct.pri_queue_t** %2, align 8
  %108 = getelementptr inbounds %struct.pri_queue_t* %107, i32 0, i32 2
  %109 = load i32* %108, align 4
  %110 = sdiv i32 %109, 2
  %111 = icmp slt i32 %106, %110
  br i1 %111, label %112, label %132

; <label>:112                                     ; preds = %91
  %113 = load %struct.pri_queue_t** %2, align 8
  %114 = getelementptr inbounds %struct.pri_queue_t* %113, i32 0, i32 1
  %115 = load i32* %114, align 4
  %116 = icmp sge i32 %115, 16
  br i1 %116, label %117, label %132

; <label>:117                                     ; preds = %112
  %118 = load %struct.pri_queue_t** %2, align 8
  %119 = getelementptr inbounds %struct.pri_queue_t* %118, i32 0, i32 0
  %120 = load %struct.q_elem_t** %119, align 8
  %121 = bitcast %struct.q_elem_t* %120 to i8*
  %122 = load %struct.pri_queue_t** %2, align 8
  %123 = getelementptr inbounds %struct.pri_queue_t* %122, i32 0, i32 2
  %124 = load i32* %123, align 4
  %125 = sdiv i32 %124, 2
  store i32 %125, i32* %123, align 4
  %126 = sext i32 %125 to i64
  %127 = mul i64 %126, 16
  %128 = call i8* @realloc(i8* %121, i64 %127)
  %129 = bitcast i8* %128 to %struct.q_elem_t*
  %130 = load %struct.pri_queue_t** %2, align 8
  %131 = getelementptr inbounds %struct.pri_queue_t* %130, i32 0, i32 0
  store %struct.q_elem_t* %129, %struct.q_elem_t** %131, align 8
  br label %132

; <label>:132                                     ; preds = %117, %112, %91
  %133 = load i8** %out, align 8
  store i8* %133, i8** %1
  br label %134

; <label>:134                                     ; preds = %132, %8
  %135 = load i8** %1
  ret i8* %135
}

; Function Attrs: nounwind ssp uwtable
define i8* @priq_top(%struct.pri_queue_t* %q, i32* %pri) #0 {
  %1 = alloca i8*, align 8
  %2 = alloca %struct.pri_queue_t*, align 8
  %3 = alloca i32*, align 8
  store %struct.pri_queue_t* %q, %struct.pri_queue_t** %2, align 8
  store i32* %pri, i32** %3, align 8
  %4 = load %struct.pri_queue_t** %2, align 8
  %5 = getelementptr inbounds %struct.pri_queue_t* %4, i32 0, i32 1
  %6 = load i32* %5, align 4
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %9

; <label>:8                                       ; preds = %0
  store i8* null, i8** %1
  br label %27

; <label>:9                                       ; preds = %0
  %10 = load i32** %3, align 8
  %11 = icmp ne i32* %10, null
  br i1 %11, label %12, label %20

; <label>:12                                      ; preds = %9
  %13 = load %struct.pri_queue_t** %2, align 8
  %14 = getelementptr inbounds %struct.pri_queue_t* %13, i32 0, i32 0
  %15 = load %struct.q_elem_t** %14, align 8
  %16 = getelementptr inbounds %struct.q_elem_t* %15, i64 1
  %17 = getelementptr inbounds %struct.q_elem_t* %16, i32 0, i32 1
  %18 = load i32* %17, align 4
  %19 = load i32** %3, align 8
  store i32 %18, i32* %19, align 4
  br label %20

; <label>:20                                      ; preds = %12, %9
  %21 = load %struct.pri_queue_t** %2, align 8
  %22 = getelementptr inbounds %struct.pri_queue_t* %21, i32 0, i32 0
  %23 = load %struct.q_elem_t** %22, align 8
  %24 = getelementptr inbounds %struct.q_elem_t* %23, i64 1
  %25 = getelementptr inbounds %struct.q_elem_t* %24, i32 0, i32 0
  %26 = load i8** %25, align 8
  store i8* %26, i8** %1
  br label %27

; <label>:27                                      ; preds = %20, %8
  %28 = load i8** %1
  ret i8* %28
}

; Function Attrs: nounwind ssp uwtable
define void @priq_combine(%struct.pri_queue_t* %q, %struct.pri_queue_t* %q2) #0 {
  %1 = alloca %struct.pri_queue_t*, align 8
  %2 = alloca %struct.pri_queue_t*, align 8
  %i = alloca i32, align 4
  %e = alloca %struct.q_elem_t*, align 8
  store %struct.pri_queue_t* %q, %struct.pri_queue_t** %1, align 8
  store %struct.pri_queue_t* %q2, %struct.pri_queue_t** %2, align 8
  %3 = load %struct.pri_queue_t** %2, align 8
  %4 = getelementptr inbounds %struct.pri_queue_t* %3, i32 0, i32 0
  %5 = load %struct.q_elem_t** %4, align 8
  %6 = getelementptr inbounds %struct.q_elem_t* %5, i64 1
  store %struct.q_elem_t* %6, %struct.q_elem_t** %e, align 8
  %7 = load %struct.pri_queue_t** %2, align 8
  %8 = getelementptr inbounds %struct.pri_queue_t* %7, i32 0, i32 1
  %9 = load i32* %8, align 4
  %10 = sub nsw i32 %9, 1
  store i32 %10, i32* %i, align 4
  br label %11

; <label>:11                                      ; preds = %22, %0
  %12 = load i32* %i, align 4
  %13 = icmp sge i32 %12, 1
  br i1 %13, label %14, label %27

; <label>:14                                      ; preds = %11
  %15 = load %struct.pri_queue_t** %1, align 8
  %16 = load %struct.q_elem_t** %e, align 8
  %17 = getelementptr inbounds %struct.q_elem_t* %16, i32 0, i32 0
  %18 = load i8** %17, align 8
  %19 = load %struct.q_elem_t** %e, align 8
  %20 = getelementptr inbounds %struct.q_elem_t* %19, i32 0, i32 1
  %21 = load i32* %20, align 4
  call void @priq_push(%struct.pri_queue_t* %15, i8* %18, i32 %21)
  br label %22

; <label>:22                                      ; preds = %14
  %23 = load i32* %i, align 4
  %24 = add nsw i32 %23, -1
  store i32 %24, i32* %i, align 4
  %25 = load %struct.q_elem_t** %e, align 8
  %26 = getelementptr inbounds %struct.q_elem_t* %25, i32 1
  store %struct.q_elem_t* %26, %struct.q_elem_t** %e, align 8
  br label %11

; <label>:27                                      ; preds = %11
  %28 = load %struct.pri_queue_t** %2, align 8
  %29 = getelementptr inbounds %struct.pri_queue_t* %28, i32 0, i32 1
  store i32 1, i32* %29, align 4
  ret void
}

; Function Attrs: nounwind ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %i = alloca i32, align 4
  %p = alloca i32, align 4
  %c = alloca i8*, align 8
  %tasks = alloca [5 x i8*], align 16
  %pri = alloca [5 x i32], align 16
  %q = alloca %struct.pri_queue_t*, align 8
  %q2 = alloca %struct.pri_queue_t*, align 8
  store i32 0, i32* %1
  %2 = bitcast [5 x i8*]* %tasks to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 40, i32 16, i1 false)
  %3 = bitcast i8* %2 to [5 x i8*]*
  %4 = getelementptr [5 x i8*]* %3, i32 0, i32 0
  store i8* getelementptr inbounds ([13 x i8]* @.str, i32 0, i32 0), i8** %4
  %5 = getelementptr [5 x i8*]* %3, i32 0, i32 1
  store i8* getelementptr inbounds ([9 x i8]* @.str1, i32 0, i32 0), i8** %5
  %6 = getelementptr [5 x i8*]* %3, i32 0, i32 2
  store i8* getelementptr inbounds ([9 x i8]* @.str2, i32 0, i32 0), i8** %6
  %7 = getelementptr [5 x i8*]* %3, i32 0, i32 3
  store i8* getelementptr inbounds ([15 x i8]* @.str3, i32 0, i32 0), i8** %7
  %8 = getelementptr [5 x i8*]* %3, i32 0, i32 4
  store i8* getelementptr inbounds ([11 x i8]* @.str4, i32 0, i32 0), i8** %8
  %9 = bitcast [5 x i32]* %pri to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %9, i8* bitcast ([5 x i32]* @main.pri to i8*), i64 20, i32 16, i1 false)
  %10 = call %struct.pri_queue_t* @priq_new(i32 0)
  store %struct.pri_queue_t* %10, %struct.pri_queue_t** %q, align 8
  %11 = call %struct.pri_queue_t* @priq_new(i32 0)
  store %struct.pri_queue_t* %11, %struct.pri_queue_t** %q2, align 8
  store i32 0, i32* %i, align 4
  br label %12

; <label>:12                                      ; preds = %25, %0
  %13 = load i32* %i, align 4
  %14 = icmp slt i32 %13, 5
  br i1 %14, label %15, label %28

; <label>:15                                      ; preds = %12
  %16 = load %struct.pri_queue_t** %q, align 8
  %17 = load i32* %i, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds [5 x i8*]* %tasks, i32 0, i64 %18
  %20 = load i8** %19, align 8
  %21 = load i32* %i, align 4
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds [5 x i32]* %pri, i32 0, i64 %22
  %24 = load i32* %23, align 4
  call void @priq_push(%struct.pri_queue_t* %16, i8* %20, i32 %24)
  br label %25

; <label>:25                                      ; preds = %15
  %26 = load i32* %i, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, i32* %i, align 4
  br label %12

; <label>:28                                      ; preds = %12
  br label %29

; <label>:29                                      ; preds = %33, %28
  %30 = load %struct.pri_queue_t** %q, align 8
  %31 = call i8* @priq_pop(%struct.pri_queue_t* %30, i32* %p)
  store i8* %31, i8** %c, align 8
  %32 = icmp ne i8* %31, null
  br i1 %32, label %33, label %37

; <label>:33                                      ; preds = %29
  %34 = load i32* %p, align 4
  %35 = load i8** %c, align 8
  %36 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8]* @.str5, i32 0, i32 0), i32 %34, i8* %35)
  br label %29

; <label>:37                                      ; preds = %29
  store i32 0, i32* %i, align 4
  br label %38

; <label>:38                                      ; preds = %64, %37
  %39 = load i32* %i, align 4
  %40 = icmp slt i32 %39, 1048576
  br i1 %40, label %41, label %67

; <label>:41                                      ; preds = %38
  %42 = call i32 @rand()
  %43 = sdiv i32 %42, 429496729
  store i32 %43, i32* %p, align 4
  %44 = load %struct.pri_queue_t** %q, align 8
  %45 = load i32* %p, align 4
  %46 = sext i32 %45 to i64
  %47 = getelementptr inbounds [5 x i8*]* %tasks, i32 0, i64 %46
  %48 = load i8** %47, align 8
  %49 = load i32* %p, align 4
  %50 = sext i32 %49 to i64
  %51 = getelementptr inbounds [5 x i32]* %pri, i32 0, i64 %50
  %52 = load i32* %51, align 4
  call void @priq_push(%struct.pri_queue_t* %44, i8* %48, i32 %52)
  %53 = call i32 @rand()
  %54 = sdiv i32 %53, 429496729
  store i32 %54, i32* %p, align 4
  %55 = load %struct.pri_queue_t** %q2, align 8
  %56 = load i32* %p, align 4
  %57 = sext i32 %56 to i64
  %58 = getelementptr inbounds [5 x i8*]* %tasks, i32 0, i64 %57
  %59 = load i8** %58, align 8
  %60 = load i32* %p, align 4
  %61 = sext i32 %60 to i64
  %62 = getelementptr inbounds [5 x i32]* %pri, i32 0, i64 %61
  %63 = load i32* %62, align 4
  call void @priq_push(%struct.pri_queue_t* %55, i8* %59, i32 %63)
  br label %64

; <label>:64                                      ; preds = %41
  %65 = load i32* %i, align 4
  %66 = add nsw i32 %65, 1
  store i32 %66, i32* %i, align 4
  br label %38

; <label>:67                                      ; preds = %38
  %68 = load %struct.pri_queue_t** %q, align 8
  %69 = getelementptr inbounds %struct.pri_queue_t* %68, i32 0, i32 1
  %70 = load i32* %69, align 4
  %71 = sub nsw i32 %70, 1
  %72 = load %struct.pri_queue_t** %q2, align 8
  %73 = getelementptr inbounds %struct.pri_queue_t* %72, i32 0, i32 1
  %74 = load i32* %73, align 4
  %75 = sub nsw i32 %74, 1
  %76 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([34 x i8]* @.str6, i32 0, i32 0), i32 %71, i32 %75)
  %77 = load %struct.pri_queue_t** %q, align 8
  %78 = load %struct.pri_queue_t** %q2, align 8
  call void @priq_combine(%struct.pri_queue_t* %77, %struct.pri_queue_t* %78)
  %79 = load %struct.pri_queue_t** %q, align 8
  %80 = getelementptr inbounds %struct.pri_queue_t* %79, i32 0, i32 1
  %81 = load i32* %80, align 4
  %82 = sub nsw i32 %81, 1
  %83 = load %struct.pri_queue_t** %q2, align 8
  %84 = getelementptr inbounds %struct.pri_queue_t* %83, i32 0, i32 1
  %85 = load i32* %84, align 4
  %86 = sub nsw i32 %85, 1
  %87 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([46 x i8]* @.str7, i32 0, i32 0), i32 %82, i32 %86)
  store i32 0, i32* %i, align 4
  br label %88

; <label>:88                                      ; preds = %93, %67
  %89 = load %struct.pri_queue_t** %q, align 8
  %90 = call i8* @priq_pop(%struct.pri_queue_t* %89, i32* null)
  store i8* %90, i8** %c, align 8
  %91 = icmp ne i8* %90, null
  br i1 %91, label %92, label %96

; <label>:92                                      ; preds = %88
  br label %93

; <label>:93                                      ; preds = %92
  %94 = load i32* %i, align 4
  %95 = add nsw i32 %94, 1
  store i32 %95, i32* %i, align 4
  br label %88

; <label>:96                                      ; preds = %88
  %97 = load i32* %i, align 4
  %98 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([26 x i8]* @.str8, i32 0, i32 0), i32 %97)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) #2

declare i32 @printf(i8*, ...) #1

declare i32 @rand() #1

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)"}
