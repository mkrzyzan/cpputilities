#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
using namespace std;

struct Node {
  bool visited;
  function<void()> dest;
  unordered_set<void*> children; 
  Node(function<void()> p = nullptr) : dest(p), visited(false) {}
  ~Node() { dest(); }
};

vector<void*> root;
unordered_map<void*, Node> objects;

void show_obj() {
  for (auto& x : objects) {
    cout << "ptr=" << x.first << ", visited=" << x.second.visited;
    cout << " {";
    for (auto& ptr : objects[x.first].children) cout << ptr << ", "; 
    cout << "}\n";
  }
}

void gc() {
  //cout << "----garbrage collection----\n";
  //show_obj();
  for (auto& x : objects) { x.second.visited = false; }
  
  //cout << "raz, root.size =" << root.size() << "\n";
  queue<void*> objqueue;
  for (auto& p : root) {  objqueue.emplace(p); }

  //cout << "dwa\n";
  while (objqueue.size()) {
    //cout << "trzy, ptr=" << objqueue.front() << "\n";
    //show_obj();
    objects[objqueue.front()].visited = true;
    for (auto& x : objects[objqueue.front()].children) {
      //cout << "cztery, ptr=" << x << "\n";
      if (false == objects[x].visited) {
        //cout << "piec\n";
        objqueue.emplace(x);
      }
    }
    //cout << "szesc\n";
    objqueue.pop();
  }

  //cout << "siedem\n";
  for (auto it = objects.begin(); it != objects.end();)
    if (false == it->second.visited) { it = objects.erase(it); }
    else ++it;
  //cout << "----garbrage collection  End----\n";
}



template <typename T>
struct gc_ptr {
  T* ptr;
  void* context;

  gc_ptr() : ptr(nullptr), context(nullptr) {}
  gc_ptr(T* p) : ptr(p), context(nullptr) { if(ptr) {objects.emplace(p, [p]{delete p;}); root.emplace_back(p);} }
  gc_ptr(void* cont) : context(cont), ptr(nullptr) {}
  gc_ptr(const gc_ptr<T>& rhs) { ptr = rhs.ptr; context = rhs.context; if(ptr) root.emplace_back(ptr); }
  
  ~gc_ptr() { if (ptr) root.pop_back(); }

  gc_ptr<T>& operator=(const gc_ptr<T>& p) { objects[context].children.emplace(p.ptr); }

  T* operator->() { return ptr; }
};

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
