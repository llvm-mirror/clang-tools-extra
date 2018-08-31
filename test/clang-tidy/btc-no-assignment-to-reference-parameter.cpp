// RUN: %check_clang_tidy %s btc-no-assignment-to-reference-parameter %t
int y = 1;
void function(int &x) {
  if (x = 2)
    // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: reference parameter x must not
    // be assigned to parameter declared above with type 'int &'
    // [btc-no-assignment-to-reference-parameter]
    x += 1;
}
void Xmain() { function(y); }
