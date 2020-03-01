//
// Created by mariakolyachko on 3.01.20.
//

#include "database.h"

std::ostream &operator<<(std::ostream &out, const Event &event) {
  out << event._date << ' ' << event._event;
  return out;
}

bool operator!=(const Event &it1, const Event &it2) {
  return !(it1._date == it2._date && it1._event == it2._event);
}


void Database::Add(const Date &date, const std::string &event) {
  if (_events_unique[date].insert(event).second) {
    _events[date].push_back(event);
  }
}

void Database::Print(std::ostream &out) const {
  for (const auto &it : _events) {
    for (const auto &event : it.second) {
      out << it.first << ' ' << event << std::endl;
    }
  }
}

Event Database::Last(const Date &date) const {
  if (date < _events.begin()->first) {
    throw std::invalid_argument("date");
  }
  auto it = _events.upper_bound(date);
  --it;
  return Event(it->first, (*it->second.rbegin()));
}