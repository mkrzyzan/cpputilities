#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

#include <gc.h>
using namespace std;

vector<void*> root;
unordered_map<void*, Node> objects;

void show_obj() {
  cout << "---objects---\n";
  for (auto& x : objects) {
    cout << "ptr=" << x.first << ", visited=" << x.second.visited;
    cout << " {";
    for (auto& ptr : objects[x.first].children) cout << ptr << ", "; 
    cout << "}\n";
  }
}

void gc() {
  for (auto& x : objects) { x.second.visited = false; }
  
  queue<void*> objqueue;
  for (auto& p : root) {  objqueue.emplace(p); }

  while (objqueue.size()) {
    objects[objqueue.front()].visited = true;
    for (auto& x : objects[objqueue.front()].children) {
      if (false == objects[x].visited) {
        objqueue.emplace(x);
      }
    }
    objqueue.pop();
  }

  for (auto it = objects.begin(); it != objects.end();)
    if (false == it->second.visited) { it = objects.erase(it); }
    else ++it;
}

