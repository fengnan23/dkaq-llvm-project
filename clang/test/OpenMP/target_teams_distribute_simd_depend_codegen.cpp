// Test host codegen.
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-64 --check-prefix OMP50 --check-prefix OMP50-64
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-64 --check-prefix OMP45
// RUN: %clang_cc1 -fopenmp -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-64 --check-prefix OMP50 --check-prefix OMP50-64
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-32 --check-prefix OMP50 --check-prefix OMP50-32
// RUN: %clang_cc1 -fopenmp -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-32 --check-prefix OMP50 --check-prefix OMP50-32


// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-64 --check-prefix OMP45
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-32 --check-prefix OMP45
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=45 -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck %s --check-prefix CHECK --check-prefix CHECK-32 --check-prefix OMP45


// RUN: %clang_cc1 -verify -Wno-vla -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp-simd -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY0 %s
// SIMD-ONLY0-NOT: {{__kmpc|__tgt}}

// Test target codegen - host bc file has to be created first.
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o - | FileCheck %s --check-prefix TCHECK --check-prefix TCHECK-64
// RUN: %clang_cc1 -fopenmp -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-ppc-host.bc -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck %s --check-prefix TCHECK --check-prefix TCHECK-64
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm-bc %s -o %t-x86-host.bc
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-x86-host.bc -o - | FileCheck %s --check-prefix TCHECK --check-prefix TCHECK-32
// RUN: %clang_cc1 -fopenmp -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-x86-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-x86-host.bc -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck %s --check-prefix TCHECK --check-prefix TCHECK-32

// RUN: %clang_cc1 -verify -Wno-vla -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm-bc %s -o %t-ppc-host.bc
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-llvm %s -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -std=c++11 -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -emit-pch -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-ppc-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -triple powerpc64le-unknown-unknown -fopenmp-targets=powerpc64le-ibm-linux-gnu -std=c++11 -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-ppc-host.bc -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp-simd -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm-bc %s -o %t-x86-host.bc
// RUN: %clang_cc1 -verify -Wno-vla -fopenmp-simd -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-llvm %s -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-x86-host.bc -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -std=c++11 -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -emit-pch -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-x86-host.bc -o %t %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -triple i386-unknown-unknown -fopenmp-targets=i386-pc-linux-gnu -std=c++11 -fopenmp-is-target-device -fopenmp-host-ir-file-path %t-x86-host.bc -include-pch %t -verify -Wno-vla %s -emit-llvm -o - | FileCheck --check-prefix SIMD-ONLY1 %s
// SIMD-ONLY1-NOT: {{__kmpc|__tgt}}

// expected-no-diagnostics
#ifndef HEADER
#define HEADER

// CHECK-DAG: [[TT:%.+]] = type { i64, i8 }
// CHECK-DAG: [[ENTTY:%.+]] = type { i64, i16, i16, i32, ptr, ptr, i64, i64, ptr }

// TCHECK: [[ENTTY:%.+]] = type { i64, i16, i16, i32, ptr, ptr, i64, i64, ptr }

// OMP45-DAG: [[SIZET:@.+]] = private unnamed_addr constant [2 x i64] [i64 0, i64 4]
// OMP45-DAG: [[MAPT:@.+]] = private unnamed_addr constant [2 x i64] [i64 544, i64 800]
// OMP45-DAG: @{{.*}} = weak constant i8 0

// OMP50-DAG: [[SIZET:@.+]] = private unnamed_addr constant [3 x i64] [i64 0, i64 4, i64 1]
// OMP50-DAG: [[MAPT:@.+]] = private unnamed_addr constant [3 x i64] [i64 544, i64 800, i64 800]
// OMP50-DAG: @{{.*}} = weak constant i8 0


// TCHECK: @{{.+}} = weak constant [[ENTTY]]
// TCHECK: @{{.+}} = {{.*}}constant [[ENTTY]]
// TCHECK-NOT: @{{.+}} = weak constant [[ENTTY]]

template<typename tx, typename ty>
struct TT{
  tx X;
  ty Y;
};

int global;
extern int global;

// CHECK: define {{.*}}[[FOO:@.+]](
int foo(int n) {
  int a = 0;
  short aa = 0;
  float b[10];
  float bn[n];
  double c[5][10];
  double cn[5][n];
  TT<long long, char> d;
  static long *plocal;

  // CHECK:       [[ADD:%.+]] = add nsw i32
  // CHECK:       store i32 [[ADD]], ptr [[DEVICE_CAP:%.+]],
  // CHECK:       [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 0
  // CHECK:       [[DEV:%.+]] = load i32, ptr [[DEVICE_CAP]],
  // CHECK:       store i32 [[DEV]], ptr [[GEP]],
  // CHECK:       [[TASK:%.+]] = call ptr @__kmpc_omp_task_alloc(ptr [[ID:@.+]], i32 [[GTID:%.+]], i32 1, i[[SZ:32|64]] {{20|40}}, i[[SZ]] 4, ptr [[TASK_ENTRY0:@.+]])
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 0
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 1
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 2
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 3
  // CHECK:       call void @__kmpc_omp_taskwait_deps_51(ptr [[ID]], i32 [[GTID]], i32 4, ptr %{{.+}}, i32 0, ptr null, i32 0)
  // CHECK:       call void @__kmpc_omp_task_begin_if0(ptr [[ID]], i32 [[GTID]], ptr [[TASK]])
  // CHECK:       call i32 [[TASK_ENTRY0]](i32 [[GTID]], ptr [[TASK]])
  // CHECK:       call void @__kmpc_omp_task_complete_if0(ptr [[ID]], i32 [[GTID]], ptr [[TASK]])
  #pragma omp target teams distribute simd device(global + a) depend(in: global) depend(out: a, b, cn[4])
  for (int i = 0; i < 10; ++i) {
  }

  // CHECK:       [[ADD:%.+]] = add nsw i32
  // CHECK:       store i32 [[ADD]], ptr [[DEVICE_CAP:%.+]],

  // OMP45:       [[BOOL:%.+]] = icmp ne i32 %{{.+}}, 0
  // OMP45:       br i1 [[BOOL]], label %[[THEN:.+]], label %[[ELSE:.+]]
  // OMP50:       br i1 {{.+}}, label %[[THEN:.+]], label %[[ELSE:.+]]
  // CHECK:       [[THEN]]:
  // OMP45-DAG:   [[BPADDR0:%.+]] = getelementptr inbounds [2 x ptr], ptr [[BP:%.+]], i32 0, i32 0
  // OMP45-DAG:   [[PADDR0:%.+]] = getelementptr inbounds [2 x ptr], ptr [[P:%.+]], i32 0, i32 0
  // OMP50-DAG:   [[BPADDR0:%.+]] = getelementptr inbounds [3 x ptr], ptr [[BP:%.+]], i32 0, i32 0
  // OMP50-DAG:   [[PADDR0:%.+]] = getelementptr inbounds [3 x ptr], ptr [[P:%.+]], i32 0, i32 0
  // CHECK-DAG:   store ptr [[BP0:%[^,]+]], ptr [[BPADDR0]]
  // CHECK-DAG:   store ptr [[BP0]], ptr [[PADDR0]]

  // OMP45-DAG:   [[BPADDR1:%.+]] = getelementptr inbounds [2 x ptr], ptr [[BP]], i32 0, i32 1
  // OMP45-DAG:   [[PADDR1:%.+]] = getelementptr inbounds [2 x ptr], ptr [[P]], i32 0, i32 1

  // OMP50-DAG:   [[BPADDR1:%.+]] = getelementptr inbounds [3 x ptr], ptr [[BP]], i32 0, i32 1
  // OMP50-DAG:   [[PADDR1:%.+]] = getelementptr inbounds [3 x ptr], ptr [[P]], i32 0, i32 1
  // CHECK-DAG:   store i[[SZ]] [[BP1:%[^,]+]], ptr [[BPADDR1]]
  // CHECK-DAG:   store i[[SZ]] [[BP1]], ptr [[PADDR1]]

  // OMP50-DAG:   [[BPADDR2:%.+]] = getelementptr inbounds [3 x ptr], ptr [[BP]], i32 0, i32 2
  // OMP50-DAG:   [[PADDR2:%.+]] = getelementptr inbounds [3 x ptr], ptr [[P]], i32 0, i32 2
  // OMP50-DAG:   store i[[SZ]] [[BP2:%[^,]+]], ptr [[BPADDR2]]
  // OMP50-DAG:   store i[[SZ]] [[BP2]], ptr [[PADDR2]]


  // OMP45-DAG:   getelementptr inbounds [2 x ptr], ptr [[BP]], i32 0, i32 0
  // OMP45-DAG:   getelementptr inbounds [2 x ptr], ptr [[P]], i32 0, i32 0
  // OMP50-DAG:   getelementptr inbounds [3 x ptr], ptr [[BP]], i32 0, i32 0
  // OMP50-DAG:   getelementptr inbounds [3 x ptr], ptr [[P]], i32 0, i32 0

  // OMP45:       [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 2
  // OMP50-64:       [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 4
  // OMP50-32:    [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 3
  // CHECK:       [[DEV:%.+]] = load i32, ptr [[DEVICE_CAP]],
  // CHECK:       store i32 [[DEV]], ptr [[GEP]],
  // CHECK:       [[DEV1:%.+]] = load i32, ptr [[DEVICE_CAP]],
  // CHECK:       [[DEV2:%.+]] = sext i32 [[DEV1]] to i64

  // OMP45:       [[TASK:%.+]] = call ptr @__kmpc_omp_target_task_alloc(ptr [[ID]], i32 [[GTID]], i32 1, i[[SZ]] {{104|60}}, i[[SZ]] {{16|12}}, ptr [[TASK_ENTRY1_:@.+]], i64 [[DEV2]])
  // OMP50:       [[TASK:%.+]] = call ptr @__kmpc_omp_target_task_alloc(ptr [[ID]], i32 [[GTID]], i32 1, i[[SZ]] {{28|128|76}}, i[[SZ]] {{16|12|24}}, ptr [[TASK_ENTRY1_:@.+]], i64 [[DEV2]])
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 0
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 1
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 2
  // CHECK:       call i32 @__kmpc_omp_task_with_deps(ptr [[ID]], i32 [[GTID]], ptr [[TASK]], i32 3, ptr %{{.+}}, i32 0, ptr null)
  // CHECK:       br label %[[EXIT:.+]]

  // CHECK:       [[ELSE]]:
  // OMP45-NOT:   getelementptr inbounds [2 x ptr], ptr
  // OMP50-NOT:   getelementptr inbounds [3 x ptr], ptr
  // OMP45:       [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 2
  // OMP50-64:       [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 4
  // OMP50-32:       [[GEP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 3
  // CHECK:       [[DEV:%.+]] = load i32, ptr [[DEVICE_CAP]],
  // CHECK:       store i32 [[DEV]], ptr [[GEP]],
  // CHECK:       [[DEV1:%.+]] = load i32, ptr [[DEVICE_CAP]],
  // CHECK:       [[DEV2:%.+]] = sext i32 [[DEV1]] to i64

  // OMP45:       [[TASK:%.+]] = call ptr @__kmpc_omp_target_task_alloc(ptr [[ID]], i32 [[GTID]], i32 1, i[[SZ]] {{56|28}}, i[[SZ]] {{16|12}}, ptr [[TASK_ENTRY1__:@.+]], i64 [[DEV2]])
  // OMP50:       [[TASK:%.+]] = call ptr @__kmpc_omp_target_task_alloc(ptr [[ID]], i32 [[GTID]], i32 1, i[[SZ]] {{56|28}}, i[[SZ]] {{16|12|24}}, ptr [[TASK_ENTRY1__:@.+]], i64 [[DEV2]])
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 0
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 1
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 2
  // CHECK:       call i32 @__kmpc_omp_task_with_deps(ptr [[ID]], i32 [[GTID]], ptr [[TASK]], i32 3, ptr %{{.+}}, i32 0, ptr null)
  // CHECK:       br label %[[EXIT:.+]]
  // CHECK:       [[EXIT]]:

  #pragma omp target teams distribute simd device(global + a) nowait depend(inout: global, a, bn) if(a)
  for (int i = 0; i < *plocal; ++i) {
    static int local1;
    *plocal = global;
    local1 = global;
  }

  // CHECK:       [[TASK:%.+]] = call ptr @__kmpc_omp_task_alloc(ptr [[ID]], i32 [[GTID]], i32 1, i[[SZ]] {{48|24}}, i[[SZ]] 4, ptr [[TASK_ENTRY2:@.+]])
  // CHECK:       getelementptr %struct.kmp_depend_info, ptr %{{.+}}, i[[SZ]] 0
  // CHECK:       call void @__kmpc_omp_taskwait_deps_51(ptr [[ID]], i32 [[GTID]], i32 1, ptr %{{.+}}, i32 0, ptr null, i32 0)
  // CHECK:       call void @__kmpc_omp_task_begin_if0(ptr [[ID]], i32 [[GTID]], ptr [[TASK]])
  // CHECK:       call i32 [[TASK_ENTRY2]](i32 [[GTID]], ptr [[TASK]])
  // CHECK:       call void @__kmpc_omp_task_complete_if0(ptr [[ID]], i32 [[GTID]], ptr [[TASK]])
  #pragma omp target teams distribute simd if(0) firstprivate(global) depend(out:global)
  for (int i = 0; i < global; ++i) {
    global += 1;
  }

  return a;
}

// Check that the offloading functions are emitted and that the arguments are
// correct and loaded correctly for the target regions in foo().

// CHECK:       define internal void [[HVT0:@.+]]()

// CHECK:       define internal{{.*}} i32 [[TASK_ENTRY0]](i32{{.*}}, ptr noalias noundef %1)
// CHECK:       store ptr null, ptr %
// CHECK:       [[DEVICE_CAP:%.+]] = load ptr,
// CHECK:       [[DEV:%.+]] = load i32, ptr [[DEVICE_CAP]],
// CHECK:       [[DEVICE:%.+]] = sext i32 [[DEV]] to i64
// CHECK:       [[RET:%.+]] = call i32 @__tgt_target_kernel(ptr @{{.+}}, i64 [[DEVICE]], i32 0, i32 1, ptr @.{{.+}}.region_id, ptr %{{.+}})
// CHECK-NEXT:  [[ERROR:%.+]] = icmp ne i32 [[RET]], 0
// CHECK-NEXT:  br i1 [[ERROR]], label %[[FAIL:[^,]+]], label %[[END:[^,]+]]
// CHECK:       [[FAIL]]
// CHECK:       call void [[HVT0]]()
// CHECK-NEXT:  br label %[[END]]
// CHECK:       [[END]]
// CHECK:       ret i32 0

// CHECK:       define internal void [[HVT1:@.+]](ptr noundef %{{.+}}, i[[SZ]] noundef %{{.+}})

// CHECK:       define internal{{.*}} i32 [[TASK_ENTRY1_]](i32{{.*}}, ptr noalias noundef %1)
// CHECK:       call void {{%.*}}(
// OMP45:       [[DEVICE_CAP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 2
// OMP50-64:       [[DEVICE_CAP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 4
// OMP50-32:       [[DEVICE_CAP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 3
// CHECK:       [[DEV:%.+]] = load i32, ptr [[DEVICE_CAP]],
// CHECK:       [[DEVICE:%.+]] = sext i32 [[DEV]] to i64
// OMP45:       [[RET:%.+]] = call i32 @__tgt_target_kernel(ptr @{{.+}}, i64 [[DEVICE]], i32 0, i32 1, ptr @.{{.+}}.region_id, ptr %{{.+}})
// OMP50:       [[RET:%.+]] = call i32 @__tgt_target_kernel(ptr @{{.+}}, i64 [[DEVICE]], i32 0, i32 1, ptr @.{{.+}}.region_id, ptr %{{.+}})

// CHECK:       [[ERROR:%.+]] = icmp ne i32 [[RET]], 0
// CHECK-NEXT:  br i1 [[ERROR]], label %[[FAIL:[^,]+]], label %[[END:[^,]+]]
// CHECK:       [[FAIL]]
// CHECK:       [[BP0:%.+]] = load ptr, ptr %
// CHECK:       [[BP1_I32:%.+]] = load i32, ptr @
// CHECK-64:    store i32 [[BP1_I32]], ptr [[BP1_PTR:%[^,]+]],
// CHECK-32:    store i32 [[BP1_I32]], ptr [[BP1_PTR:%[^,]+]],
// CHECK:       [[BP1:%.+]] = load i[[SZ]], ptr [[BP1_PTR]],
// OMP45:       call void [[HVT1]](ptr [[BP0]], i[[SZ]] [[BP1]])

// OMP50:       [[BP2:%.+]] = load i[[SZ]], ptr
// OMP50:       call void [[HVT1]](ptr [[BP0]], i[[SZ]] [[BP1]], i[[SZ]] [[BP2]])
// CHECK-NEXT:  br label %[[END]]
// CHECK:       [[END]]
// CHECK:       ret i32 0

// CHECK:       define internal{{.*}} i32 [[TASK_ENTRY1__]](i32{{.*}}, ptr noalias noundef %1)
// CHECK:       call void {{%.*}}(
// CHECK:       [[DEVICE_CAP:%.+]] = getelementptr inbounds nuw %{{.+}}, ptr %{{.+}}, i32 0, i32 2
// CHECK:       [[BP0:%.+]] = load ptr, ptr %
// CHECK:       [[BP1_I32:%.+]] = load i32, ptr @
// CHECK-64:    store i32 [[BP1_I32]], ptr [[BP1_PTR:%[^,]+]],
// CHECK-32:    store i32 [[BP1_I32]], ptr [[BP1_PTR:%[^,]+]],
// CHECK:       [[BP1:%.+]] = load i[[SZ]], ptr [[BP1_PTR]],
// OMP50:       [[BP2:%.+]] = load i[[SZ]], ptr
// OMP45:       call void [[HVT1]](ptr [[BP0]], i[[SZ]] [[BP1]])
// OMP50:       call void [[HVT1]](ptr [[BP0]], i[[SZ]] [[BP1]], i[[SZ]] [[BP2]])

// CHECK:       ret i32 0

// CHECK:       define internal void [[HVT2:@.+]](i[[SZ]] noundef %{{.+}})
// Create stack storage and store argument in there.
// CHECK:       [[AA_ADDR:%.+]] = alloca i[[SZ]], align
// CHECK:       store i[[SZ]] %{{.+}}, ptr [[AA_ADDR]], align
// CHECK-64:    load i32, ptr [[AA_ADDR]], align
// CHECK-32:    load i32, ptr [[AA_ADDR]], align

// CHECK:       define internal{{.*}} i32 [[TASK_ENTRY2]](i32{{.*}}, ptr noalias noundef %1)
// CHECK:       call void {{%.*}}(
// CHECK:       [[BP1_I32:%.+]] = load i32, ptr %
// CHECK-64:    store i32 [[BP1_I32]], ptr [[BP1_PTR:%[^,]+]],
// CHECK-32:    store i32 [[BP1_I32]], ptr [[BP1_PTR:%[^,]+]],
// CHECK:       [[BP1:%.+]] = load i[[SZ]], ptr [[BP1_PTR]],
// CHECK:       call void [[HVT2]](i[[SZ]] [[BP1]])
// CHECK:       ret i32 0


#endif
