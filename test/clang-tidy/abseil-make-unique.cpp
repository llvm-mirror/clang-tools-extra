// RUN: %check_clang_tidy %s abseil-make-unique %t
namespace std {
template <typename T>
struct default_delete {};

template <typename T, typename D = default_delete<T>>
class unique_ptr {
 public:
  unique_ptr();
  explicit unique_ptr(T*);
  void reset(T*);
};
}  // namespace std

class A {
 int x;
 int y;

 public:
   A(int _x, int _y): x(_x), y(_y) {}
};

struct B {
  int x;
  int y;
};

int* ReturnPointer();
void ExpectPointer(std::unique_ptr<int> p);

std::unique_ptr<int> MakeAndReturnPointer() {
  // Make smart pointer in return statement
  return std::unique_ptr<int>(new int(0));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: return absl::make_unique<int>(0);
}

void Positives() {
  std::unique_ptr<int> a(new int(1));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique] 
  // CHECK-FIXES: auto a = absl::make_unique<int>(1);  

  std::unique_ptr<int> b;
  b.reset(new int(2));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to resetting unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: b = absl::make_unique<int>(2);

  // Non-primitive paramter
  std::unique_ptr<A> c(new A(1, 2));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique] 
  // CHECK-FIXES: auto c = absl::make_unique<A>(1, 2);

  std::unique_ptr<A> d;
  d.reset(new A(3, 4));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to resetting unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: d = absl::make_unique<A>(3, 4);  

  // No arguments to new expression
  std::unique_ptr<int> e(new int);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: auto e = absl::make_unique<int>();

  std::unique_ptr<int> f;
  f.reset(new int);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to resetting unique_ptr with new [abseil-make-unique] 
  // CHECK-FIXES: f = absl::make_unique<int>(); 

  // Nested parentheses
  std::unique_ptr<int> g((new int(3)));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: auto g = absl::make_unique<int>(3);

  std::unique_ptr<int> h(((new int(4))));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: auto h = absl::make_unique<int>(4);

  std::unique_ptr<int> i;
  i.reset((new int(5)));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to resetting unique_ptr with new [abseil-make-unique] 
  // CHECK-FIXES: i = absl::make_unique<int>(5);

  std::unique_ptr<int> j;
  j.reset(((new int(6))));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to resetting unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: j = absl::make_unique<int>(6);
  
  // Construct unique_ptr within a function
  ExpectPointer(std::unique_ptr<int>(new int(5)));
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: ExpectPointer(absl::make_unique<int>(5));
}

// Checks within namespaces 
namespace std {
  unique_ptr<int> k(new int(7));
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: prefer absl::make_unique to constructing unique_ptr with new [abseil-make-unique]
  // CHECK-FIXES: auto k = absl::make_unique<int>(7); 
}

void Negatives() {
  // Do not warn for functions that return a pointer
  std::unique_ptr<int> a(ReturnPointer());
  
  std::unique_ptr<int> b;
  b.reset(ReturnPointer());

  // Do not warn for aggregate initialization
  std::unique_ptr<B> c(new B{.x = 1, .y = 2});
}
