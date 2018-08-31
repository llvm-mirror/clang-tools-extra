// RUN: %check_clang_tidy %s btc-singleton %t
class Singleton {};
class A : Singleton {};
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: Use of singleton detected.
// [btc-singleton]

class C {};
