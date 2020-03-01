//
// Created by mariakolyachko on 30.01.20.
//

#include "test_runner.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <random>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T> class PriorityCollection {
public:
  using Id = int;
  using Object = typename std::pair<int, Id>;
  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    data_.push_back(move(object));
    priorities_.emplace_back(0);
    Id id = data_.size() - 1;
    sorted_.insert({0, id});
    ids_.insert(id);
    return id;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (auto it = range_begin; it != range_end; ++it) {
      *(ids_begin++) = Add(move(*it));
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  [[nodiscard]] bool IsValid(Id id) const {
    return ids_.find(id) != ids_.end();
  }

  // Получить объект по идентификатору
  [[nodiscard]] const T &Get(Id id) const { return data_[id]; }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    sorted_.erase({priorities_[id], id});
    ++priorities_[id];
    sorted_.insert({priorities_[id], id});
  }

  // Получить объект с максимальным приоритетом и его приоритет
  [[nodiscard]] pair<const T &, int> GetMax() const {
    auto max = prev(sorted_.end());
    return std::pair<const T &, int>(data_[max->second], max->first);
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    auto max = prev(sorted_.end());
    sorted_.erase(prev(sorted_.end()));
    ids_.erase(max->second);
    return std::pair<T, int>(move(data_[max->second]), max->first);
  }

private:
  set<Object> sorted_;
  set<Id> ids_;
  vector<T> data_;
  vector<int> priorities_;
};

class StringNonCopyable : public string {
public:
  using string::string; // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable &) = delete;
  StringNonCopyable(StringNonCopyable &&) = default;
  StringNonCopyable &operator=(const StringNonCopyable &) = delete;
  StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.GetMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

void TestIsValidId() {
  vector<StringNonCopyable> const_strings;
  for (size_t i = 0; i < 10; ++i) {
    string it = to_string(i);
    const_strings.emplace_back(StringNonCopyable(it.begin(), it.end()));
  }
  PriorityCollection<StringNonCopyable> strings;
  std::vector<int> ids;
  strings.Add(const_strings.begin(), const_strings.end(),
              std::back_inserter(ids));
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(strings.IsValid(9), false);
  }
  {
    for (size_t i = 0; i < 9; ++i) {
      for (size_t j = 0; j < i; ++j) {
        strings.Promote(i);
      }
    }
    {
      const auto item = strings.PopMax();
      ASSERT_EQUAL(strings.IsValid(8), false);
    }
    {
      for (size_t i = 0; i < 10; ++i) {
        strings.Promote(5);
      }
      const auto item = strings.PopMax();
      ASSERT_EQUAL(strings.IsValid(5), false);
    }
    { ASSERT_EQUAL(strings.IsValid(12), false); }
  }
}
int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, TestIsValidId);
  return 0;
}