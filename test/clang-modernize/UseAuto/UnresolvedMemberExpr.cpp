// RUN: grep -Ev "// *[A-Z-]+:" %s > %t.cpp
// RUN: clang-modernize -use-auto %t.cpp -- --std=c++11 -I %S/Inputs
#define CONTAINER vector
#include "test_std_container.h"
#undef CONTAINER

namespace A 
{
  namespace B
  {
    class C {
      public:
      class iterator;
      typedef iterator const_iterator;

    };
  }
}

template <class ParserT>
class ParseWorker
{
  public:
    void Run() {
      for (std::vector<A::B::C>::const_iterator pathIt=m_paths.begin();
          pathIt!=m_paths.end(); ++pathIt)
      {

      }
    }

  private:
    std::vector<A::B::C> m_paths;
};

