#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
using namespace std;

struct Node;
extern vector<void*> root;
extern unordered_map<void*, Node> objects;
void show_obj();
void gc();

struct Node 
{
  unordered_set<void*> children; 
  bool visited;
  function<void()> dest;
  Node(const unordered_set<void*>& ch) : dest(nullptr), visited(false), children(ch) {}
  Node(function<void()> p = nullptr) : dest(p), visited(false) {}
  ~Node() { if (dest) dest(); }
  
  bool operator==(const Node& rhs) const {
    return children == rhs.children;
  }
};

ostream& operator<<(ostream& ostr, const Node& obj);

template <typename T>
struct gc_ptr 
{
  gc_ptr() : ptr(nullptr), context(nullptr) {}

  gc_ptr(T* p); 

  //gc_ptr(void* cont) : context(cont), ptr(nullptr) {}

  gc_ptr(const gc_ptr<T>& rhs);
  
  ~gc_ptr();

  gc_ptr<T>& operator=(const gc_ptr<T>& p);

  T* operator->();

  T* ptr;
  void* context;
};

template <typename T>
gc_ptr<T>::gc_ptr(T* p) : ptr(p), context(nullptr) 
{ 
  if (ptr) 
  {
    function<void()> bla = [p](){p->~T(); free(p);};
    objects.emplace(static_cast<void*>(p), bla); 
    root.emplace_back(p);
  }
}

template <typename T>
gc_ptr<T>::gc_ptr(const gc_ptr<T>& rhs)
{ 
  ptr = rhs.ptr; 
  context = rhs.context; 
  if(ptr) root.emplace_back(ptr); 
}
  
template <typename T>
gc_ptr<T>::~gc_ptr() 
{ 
  //cout << "kasujemy pointer\n";
  if (ptr) root.pop_back(); 
}

template <typename T>
gc_ptr<T>& gc_ptr<T>::operator=(const gc_ptr<T>& p) 
{
  objects[context].children.emplace(p.ptr); 
}

template <typename T>
T* gc_ptr<T>::operator->() { return ptr; }



// ============== helper functions ===============
template <typename T>
gc_ptr<T> make_gc_ptr(T* t) 
{
  return gc_ptr<T>(t);
}



