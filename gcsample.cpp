#include <iostream>
#include <vector>
#include <gc.h>
using namespace std;

struct A;

struct B { 
  gc_ptr<A> ptr;
  B() : ptr(this) {cout<<"B\n";} 
  ~B(){cout<<"~B\n";} 
};

struct A {
  gc_ptr<B> ptr;
  gc_ptr<B> ptr2;
  A() : ptr(this), ptr2(this) {cout<<"A\n";}
  ~A(){cout<<"~A\n";}
  void f()
  {
    {
      auto a = gc_ptr(new B());
      ptr = gc_ptr(new B());
      auto b = gc_ptr(new B());
    }
  }
};

int main() 
{
  {
    auto no = gc_ptr(new B());
    auto a = gc_ptr(new A());
    {
      auto a = gc_ptr(new A());
      auto p = gc_ptr(new A());
      p->f();
      p->ptr2 = no; 
    }
  }
  gc();

  {
      auto a = gc_ptr(new A());
      auto b = gc_ptr(new B());
      a->ptr = b;
      b->ptr = a;
  }
  gc();

  cout << "vector test\n";
  {
    auto a = gc_ptr(new A());
    {
      vector<gc_ptr<A>> ha(10);
      ha.emplace_back(a);
      ha.emplace_back(a);
      ha.emplace_back(a);
    }
  }
  gc();

  {
    auto a = gc_ptr(new A());
    auto b = gc_ptr(new A());
  }
  gc();
  cout << "-- The End -- \n";
}
