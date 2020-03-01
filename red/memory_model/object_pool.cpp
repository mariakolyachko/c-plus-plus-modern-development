#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
    T* pointer;
    if (!deallocated_.empty()) {
      pointer = deallocated_.front();
      deallocated_.pop();
    } else {
      pointer = new T;
    }
    allocated_.insert(pointer);
    return pointer;
  }

  T* TryAllocate() {
    T* pointer;
    if (!deallocated_.empty()) {
      pointer = deallocated_.front();
      deallocated_.pop();
    } else {
      return nullptr;
    }
    allocated_.insert(pointer);
    return pointer;
  }

  void Deallocate(T* object) {
    auto it = allocated_.find(object);
    if (it != allocated_.end()) {
      deallocated_.push(*it);
      allocated_.erase(it);
    } else {
      throw std::invalid_argument("invalid_argument");
    }
  }

  ~ObjectPool() {
    while (!deallocated_.empty()) {
      delete deallocated_.front();
      deallocated_.pop();
    }
    while (!allocated_.empty()) {
      delete *allocated_.begin();
      allocated_.erase(allocated_.begin());
    }
  }

private:
  std::set<T*> allocated_;
  std::queue<T*> deallocated_;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}
