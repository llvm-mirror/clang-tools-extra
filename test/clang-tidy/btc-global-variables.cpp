// RUN: %check_clang_tidy %s btc-global-variables %t

int globalInt = 1;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: Global variable detected.
// [btc-global-variables]

class Object {
 public:
  Object();
  int do_Object() { return 1; }
  int field = 1;
  // Should not trigger
 private:
  static int static_field;
};

int Object::static_field = 0;
// Should not trigger

void f() {
  static int staticLocalVariable = 2;
  // Should not trigger
}
