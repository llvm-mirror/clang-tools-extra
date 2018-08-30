// RUN: %check_clang_tidy %s btc-unmanaged-derived-from-sharable %t
template <typename T>
class SharedPtr
{T* data;
int x;
public:
SharedPtr(){data = new T;}
};

template <typename T>
class UniquePtr
{
T* data;
int y;
public:
UniquePtr(){data = new T;}
};

template <typename T>
class AutoPtr
{T* data;
int z;
public:
AutoPtr(){data = new T;}
};

template <typename T>
SharedPtr<T> CreateShared()
{
 return SharedPtr<T>();
}

template <typename T>
UniquePtr<T> CreateUnique()
{
 return UniquePtr<T>();
}

template <typename T>
AutoPtr<T> CreateAuto()
{
 return AutoPtr<T>();
}

class Sharable {
 public:
  Sharable(int value) : val(value) {}
  int do_Sharable() { return val; }

 private:
  int val;
};

class DerivedFromSharable : public Sharable {
 public:
  DerivedFromSharable() : Sharable(0), val2(1) {}
  int do_DerivedFromSharable() { return val2 + do_Sharable(); }

 private:
  int val2;
};

class VirtualyDerivedFromSharable : virtual public Sharable {
 public:
  VirtualyDerivedFromSharable() : Sharable(0), val2(1) {}
  int do_VirtualyDerivedFromSharable() { return val2 + do_Sharable(); }

 private:
  int val2;
};

class GodObject {
 public:
  GodObject();
  int do_GodObject() { return 1 + ds.do_DerivedFromSharable() + vds.do_VirtualyDerivedFromSharable(); }

 private:
  DerivedFromSharable ds;
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: Field 'ds' has type derived from
  // Sharable [btc-unmanaged-derived-from-sharable]
  AutoPtr<DerivedFromSharable> ds2 = CreateAuto<DerivedFromSharable>();
  SharedPtr<DerivedFromSharable> ds3 = CreateShared<DerivedFromSharable>();
  UniquePtr<DerivedFromSharable> ds4 = CreateUnique<DerivedFromSharable>();

  VirtualyDerivedFromSharable vds;
  // CHECK-MESSAGES: :[[@LINE-1]]:31: warning: Field 'vds' has type derived from
  // Sharable [btc-unmanaged-derived-from-sharable]
  AutoPtr<VirtualyDerivedFromSharable> vds2 = CreateAuto<VirtualyDerivedFromSharable>();
  SharedPtr<VirtualyDerivedFromSharable> vds3 = CreateShared<VirtualyDerivedFromSharable>();
  UniquePtr<VirtualyDerivedFromSharable> vds4 = CreateUnique<VirtualyDerivedFromSharable>();



};

void f() {
  GodObject b;
  b.do_GodObject();
  DerivedFromSharable lds;
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: Local variable 'lds' has type
  // derived from Sharable [btc-unmanaged-derived-from-sharable]
  VirtualyDerivedFromSharable lvds;
  // CHECK-MESSAGES: :[[@LINE-1]]:31: warning: Local variable 'lvds' has type
  // derived from Sharable [btc-unmanaged-derived-from-sharable]
  lds.do_DerivedFromSharable();
  lvds.do_VirtualyDerivedFromSharable();
}
