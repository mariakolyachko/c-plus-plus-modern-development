//
// Created by mariakolyachko on 2.02.20.
//

#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <cmath>
#include <future>
#include <numeric>
#include <random>
#include <string>
#include <vector>
using namespace std;

//template <typename T>
//class Synchronized {
//public:
//  explicit Synchronized(T initial = T()) : value(move(initial)) {}
//
//  struct Access {
//    lock_guard<mutex> guard;
//    T& ref_to_value;
//  };
//
//  Access GetAccess() {
//    return {lock_guard<mutex>(m), value};
//  }
//private:
//  T value;
//  mutex m;
//};

template <typename K, typename V> class ConcurrentMap {
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
    lock_guard<mutex> guard;
    V &ref_to_value;
  };

  explicit ConcurrentMap(size_t bucket_count)
      : segments_(bucket_count) {};

  Access operator[](const K &key) {
    size_t index = getIndex(key);
  //  lock_guard<mutex> guard(segments_[index].segment_mutex);
    return {lock_guard(segments_[index].segment_mutex),
            segments_[index].segment[key]};
  }

  map<K, V> BuildOrdinaryMap() {
    lock_guard<mutex> start_guard(mut);

    map<K, V> result;
    for (size_t i = 0; i < segments_.size(); ++i) {
      lock_guard<mutex> current_guard(segments_[i].segment_mutex);
      auto item = segments_[i].segment;
      for (auto &[k, v] : item) {
        result[k] = v;
      }
    }
    return result;
  }

private:
  struct Segment {
    mutex segment_mutex;
    map<K, V> segment;
  };
  vector<Segment> segments_;
  mutex mut;

  size_t getIndex(const K &key) {
    lock_guard<mutex> guard(mut);
    int index = key;
    return abs(index) % segments_.size();
  }
};

void RunConcurrentUpdates(ConcurrentMap<int, int> &cm, size_t thread_count,
                          int key_count) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
    cm.BuildOrdinaryMap();
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto &[k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string &s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
  return 0;
}