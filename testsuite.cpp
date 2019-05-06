#include <iostream>
#include <vector>
#include <gc.h>

#include <gtest/gtest.h>
using namespace std;

/*
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
  A(A& rhs)  { ptr.context=this; ptr2.context=this; cout<<"A&\n";}
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
*/

struct A {
  bool& isDestroyd;
  A(bool& x) : isDestroyd(x) { isDestroyd=false; }
  ~A() { isDestroyd=true;}
};

using ObjectsList = unordered_map<void*,Node>;

TEST(BasicTest, BasicConcepts)
{
  bool bIsDel, cIsDel;
  void *bPtr, *cPtr;
  {
    gc_ptr<A> b = make_gc_ptr(new A(bIsDel));
    gc_ptr<A> c = make_gc_ptr(new A(cIsDel));
    bPtr = b.ptr; cPtr = c.ptr;
    ASSERT_FALSE (bIsDel);
    ASSERT_FALSE (cIsDel);
    //show_obj();

    ASSERT_EQ ((ObjectsList{{b.ptr,Node()}, {c.ptr,Node()}}), objects);
    ASSERT_EQ ((vector<void*>{b.ptr, c.ptr}), root);
  }
  ASSERT_EQ ((ObjectsList{{bPtr,Node()}, {cPtr,Node()}}), objects);
  ASSERT_EQ ((vector<void*>{}), root);
  ASSERT_FALSE (bIsDel);
  ASSERT_FALSE (cIsDel);
  //show_obj();
  gc();
  ASSERT_EQ ((ObjectsList{}), objects);
  ASSERT_EQ ((vector<void*>{}), root);
  //show_obj();
  ASSERT_TRUE (bIsDel);
  ASSERT_TRUE (cIsDel);

  gc();
  ASSERT_EQ ((ObjectsList{}), objects);
  ASSERT_EQ ((vector<void*>{}), root);
}


TEST(BasicTest, EnteringExitingScope)
{
  struct B {};
  void *aPtr, *bPtr, *cPtr, *dPtr, *ePtr, *fPtr;
  {
    gc_ptr<B> a = make_gc_ptr(new B());
    gc_ptr<B> b = make_gc_ptr(new B());
    aPtr = a.ptr; bPtr = b.ptr;

    ASSERT_EQ ((ObjectsList{{aPtr,Node()}, {bPtr,Node()}}), objects);
    ASSERT_EQ ((vector<void*>{aPtr, bPtr}), root);
    {
      gc_ptr<B> c = make_gc_ptr(new B());
      cPtr = c.ptr;
      ASSERT_EQ ((ObjectsList{{aPtr,Node()}, {bPtr,Node()}, {cPtr,Node()}}), objects);
      ASSERT_EQ ((vector<void*>{aPtr, bPtr, cPtr}), root);

      {
        gc_ptr<B> d = make_gc_ptr(new B());
        dPtr = d.ptr;
        ASSERT_EQ ((ObjectsList{{dPtr, Node()}, {aPtr,Node()}, {bPtr,Node()}, {cPtr,Node()}}), objects);
        ASSERT_EQ ((vector<void*>{aPtr, bPtr, cPtr, dPtr}), root);
      }

      ASSERT_EQ ((ObjectsList{{dPtr, Node()}, {aPtr,Node()}, {bPtr,Node()}, {cPtr,Node()}}), objects);
      ASSERT_EQ ((vector<void*>{aPtr, bPtr, cPtr}), root);
      gc();
      ASSERT_EQ ((ObjectsList{{aPtr,Node()}, {bPtr,Node()}, {cPtr,Node()}}), objects);
      ASSERT_EQ ((vector<void*>{aPtr, bPtr, cPtr}), root);
    }
    ASSERT_EQ ((ObjectsList{{aPtr,Node()}, {bPtr,Node()}, {cPtr,Node()}}), objects);
    ASSERT_EQ ((vector<void*>{aPtr, bPtr}), root);
  }
  ASSERT_EQ ((ObjectsList{{aPtr,Node()}, {bPtr,Node()}, {cPtr,Node()}}), objects);
  ASSERT_EQ ((vector<void*>{}), root);
  gc();
  ASSERT_EQ ((ObjectsList{}), objects);
  ASSERT_EQ ((vector<void*>{}), root);
}

/*
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
    {
      vector<gc_ptr<A>> ha;
      ha.emplace_back(a);
      ha.emplace_back(a);
      ha.emplace_back(a);
      ha.emplace_back(a);
      ha.emplace_back(a);
      ha.emplace_back(a);
    }
  }
  gc();

  {
    auto a = make_gc_ptr(new A());
    auto b = make_gc_ptr(new A());
  }
  gc();
  cout << "-- The End -- \n";
}
*/

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
