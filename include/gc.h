#pragma once

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

extern vector<void*> root;
extern unordered_map<void*, Node> objects;
void show_obj();
void gc();


template <typename T>
struct gc_ptr {
  T* ptr;
  void* context;

  gc_ptr() : ptr(nullptr), context(nullptr) {}

  gc_ptr(T* p) : ptr(p), context(nullptr) { 
    if(ptr) 
    {
      function<void()> bla = [p](){p->~T(); free(p);};
      objects.emplace(static_cast<void*>(p), bla); 
      root.emplace_back(p);
    }
  }

/*
  gc_ptr(void* cont) : context(cont), ptr(nullptr) {}

*/
  gc_ptr(const gc_ptr<T>& rhs)
  { 
    ptr = rhs.ptr; 
    context = rhs.context; 
    if(ptr) root.emplace_back(ptr); 
  }
  
  ~gc_ptr() 
  { 
    //cout << "kasujemy pointer\n";
    if (ptr) root.pop_back(); 
  }

  gc_ptr<T>& operator=(const gc_ptr<T>& p) 
  {
    objects[context].children.emplace(p.ptr); 
  }

  T* operator->() { return ptr; }
};


template <typename T>
gc_ptr<T> make_gc_ptr(T* t) {
  return gc_ptr<T>(t);
}



