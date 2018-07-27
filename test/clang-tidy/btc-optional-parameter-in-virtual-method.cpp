// RUN: %check_clang_tidy %s btc-optional-parameter-in-virtual-method %t

class P {
 public:
  virtual void f(int x = 17);  // f has an unparsed default argument now
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: parameter 'x' of virtual function
  // 'f' is optional [btc-optional-parameter-in-virtual-method]
  virtual void g();
  void h();
};  // f has a regular default argument now

template <class P>
class C {
 public:
  virtual void f(int y = 21);  // f has an uninstantiated default argument
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: parameter 'y' of virtual function
  // 'f' is optional [btc-optional-parameter-in-virtual-method]
  virtual void g();
  void h();
};
