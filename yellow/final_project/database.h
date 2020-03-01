//
// Created by mariakolyachko on 3.01.20.
//

#ifndef FINAL_PROJECT_DATABASE_H
#define FINAL_PROJECT_DATABASE_H

#include "condition_parser.h"
#include "date.h"
#include "node.h"
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>

struct Event {
  Date _date;
  std::string _event;

  Event(const Date &date, const std::string &event)
      : _date(date), _event(event) {}
};

std::ostream &operator<<(std::ostream &out, const Event &event);
bool operator!=(const Event &it1, const Event &it2);

class Database {
public:
  void Add(const Date &date, const std::string &event);

  template <class Func> int RemoveIf(Func predicate) {
    int count = 0;
    std::vector<Date> toDelete;
    for (auto &it : _events) {
      auto date = &it.first;
      auto events = &it.second;
      auto border =
          std::stable_partition(events->begin(), events->end(),
                                [date, predicate](const std::string event) {
                                  return !predicate(*date, event);
                                });
      count += events->end() - border;
      events->erase(border, events->end());
      _events_unique[*date].clear();
      for (const auto &event : *events) {
        _events_unique[*date].insert(event);
      }
      if (events->empty()) {
        toDelete.push_back(*date);
      }
    }

    for (const auto &date : toDelete) {
      _events_unique.erase(date);
      _events.erase(date);
    }
    return count;
  }

  template <class Func> std::vector<Event> FindIf(Func predicate) const {
    std::vector<Event> events;
    for (auto it = _events.begin(); it != _events.end(); ++it) {
      auto pred = [it, predicate](const std::string event) {
        return predicate(it->first, event);
      };
      auto start = std::find_if(it->second.begin(), it->second.end(), pred);
      while (start != it->second.end()) {
        events.push_back(Event(it->first, (*start)));
        start = std::find_if(start + 1, it->second.end(), pred);
      }
    }
    return events;
  }

  Event Last(const Date &date) const;
  void Print(std::ostream &out) const;

private:
  std::map<Date, std::vector<std::string>> _events;
  std::map<Date, std::set<std::string>> _events_unique;
};

#endif // FINAL_PROJECT_DATABASE_H
