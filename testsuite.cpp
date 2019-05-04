#include <iostream>
#include <vector>
#include <gc.h>

#include <gtest/gtest.h>
using namespace std;

struct A;

struct B { 
  gc_ptr<A> ptr;
  B() {ptr.context=this; cout<<"B\n";} 
  ~B(){cout<<"~B\n";} 
};

struct A {
  gc_ptr<B> ptr;
  gc_ptr<B> ptr2;
  A()  { ptr.context=this; ptr2.context=this; cout<<"A\n";}
  ~A(){cout<<"~A\n";}
  void f()
  {
    {
      auto&& a = make_gc_ptr(new B());
      ptr = make_gc_ptr(new B());
      auto&& b = make_gc_ptr(new B());
    }
  }
};

TEST(BasicTest, BasicConcepts)
{
  {
    auto no = make_gc_ptr(new B());
    auto a = make_gc_ptr(new A());
    {
      auto a = make_gc_ptr(new A());
      auto p = make_gc_ptr(new A());
      p->f();
      p->ptr2 = no; 
    }
  }
  //show_obj();
  gc();
  //show_obj();

  {
      auto a = make_gc_ptr(new A());
      auto b = make_gc_ptr(new B());
      a->ptr = b;
      b->ptr = a;
  }
  gc();
  cout << "vector test\n";
  {
    auto a = make_gc_ptr(new A());
    /*
    {
      vector<gc_ptr<A>> ha(10);
      ha.emplace_back(a);
      ha.emplace_back(a);
      ha.emplace_back(a);
    }
    */
  }
  gc();

  {
    auto a = make_gc_ptr(new A());
    auto b = make_gc_ptr(new A());
  }
  gc();
  cout << "-- The End -- \n";
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
