// RUN: %check_clang_tidy %s btc-multiple-parameters-of-same-type %t

void f(int y, int z);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'f' has multiple parameters
// of same type [btc-multiple-parameters-of-same-type]
// CHECK-MESSAGES: :[[@LINE-3]]:12: warning: parameter 'y' with type ''int''
// (total number of parameters with this type: 2)
// [btc-multiple-parameters-of-same-type]
// CHECK-MESSAGES: :[[@LINE-6]]:19: warning: parameter 'z' with type ''int''
// (total number of parameters with this type: 2)
// [btc-multiple-parameters-of-same-type]

void awesome_f2(int i, double d, float u);
