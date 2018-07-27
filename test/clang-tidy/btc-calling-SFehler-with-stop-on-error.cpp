// RUN: %check_clang_tidy %s btc-calling-SFehler-with-stop-on-error %t
int SFehler(int a, int b, int c, int d, int e) { return a + b; }
int SFehler(int a, int b, int c, int d, int e, int f) { return b * b; }
int SFehler(int a, int b, int c, float d, float e, float f) { return a + c; }
void f() {
  auto R = SFehler(9, 9, 9, 9, 9, 1);
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: SFehler function called with
  // STOPONERROR parameter. [btc-calling-SFehler-with-stop-on-error]
  R = SFehler(9, 9, 9, 9, 9, 9);
  R = SFehler(9, 9, 9, 9, 1);
  SFehler(2, 2, 2, 2.0f, 1.0f, 2.0f);
}
