#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record) {
    if (index_.count(record.id) == 0) {
      index_[record.id] = records_.size();
      auto timestamp_iterator =
          range_by_timestamp_.insert({record.timestamp, records_.size()});
      auto karma_iterator =
          range_by_karma_.insert({record.karma, records_.size()});
      auto user_iterator = users_.insert({record.user, records_.size()});
      records_.push_back(
          {record, timestamp_iterator, karma_iterator, user_iterator});
      return true;
    }
    return false;
  }

  const Record* GetById(const string& id) const {
    if (index_.count(id) == 0) {
      return nullptr;
    }
    return &records_[index_.at(id)].record_;
  }

  bool Erase(const string& id) {
    if (index_.count(id) == 0) {
      return false;
    }
    auto index = index_[id];
    range_by_timestamp_.erase(records_[index].iterators_.timestamp_iterator_);
    range_by_karma_.erase(records_[index].iterators_.karma_iterator_);
    users_.erase(records_[index].iterators_.user_iterator_);
    index_.erase(id);
    return true;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    for (auto it = range_by_timestamp_.lower_bound(low);
         it != range_by_timestamp_.upper_bound(high); ++it) {
      if (callback(records_[it->second].record_) == false) {
        return;
      }
    }
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    for (auto it = range_by_karma_.lower_bound(low);
         it != range_by_karma_.upper_bound(high); ++it) {
      if (callback(records_[it->second].record_) == false) {
        return;
      }
    }
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    for (auto it = users_.find(user);
         it != users_.end() && it->first == user; ++it) {
      if (callback(records_[it->second].record_) == false) {
        return;
      }
    }
  }

private:
  std::unordered_map<std::string, size_t> index_;
  std::multimap<int, size_t> range_by_timestamp_;
  std::multimap<int, size_t> range_by_karma_;
  std::unordered_multimap<std::string, size_t> users_;

  struct Iterators {
    std::multimap<int, size_t>::iterator timestamp_iterator_;
    std::multimap<int, size_t>::iterator karma_iterator_;
    std::unordered_multimap<std::string, size_t>::iterator user_iterator_;
  };

  struct RecordInfo {
    Record record_;
    Iterators iterators_;
  };

  std::vector<RecordInfo> records_;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}