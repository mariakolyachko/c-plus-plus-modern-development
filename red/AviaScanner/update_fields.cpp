//
// Created by mariakolyachko on 17.01.20.
//

#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values)    \
{                                              \
  auto it = values.find(#field);               \
  if (it != values.end()) {                    \
    std::stringstream ss(it->second);          \
    ss >> ticket.field;                        \
  }                                            \
}

bool operator<(const Date &first, const Date &second) {
  if (first._year == second._year) {
    if (first._month == second._month) {
      return first._day < second._day;
    } else {
      return first._month < second._month;
    }
  } else {
    return first._year < second._year;
  }
}

bool operator==(const Date &first, const Date &second) {
  return first._year == second._year && first._month == second._month &&
         first._day == second._day;
}

bool operator!=(const Date &first, const Date &second) {
  return !(first == second);
}

std::ostream &operator<<(std::ostream &out, const Date &date) {
  out << date._year << "-" << date._month << "-" << date._day;
  return out;
}

bool operator<(const Time &first, const Time &second) {
  if (first._hours == second._hours) {
    return first._minutes < second._minutes;
  } else {
    return first._hours < second._hours;
  }
}

bool operator==(const Time &first, const Time &second) {
  return first._hours == second._hours && first._minutes == second._minutes;
}

bool operator!=(const Time &first, const Time &second) {
  return !(first == second);
}

std::ostream &operator<<(std::ostream &out, const Time &time) {
  out << time._hours << ":" << time._minutes;
  return out;
}
std::istream &operator>>(std::istream &in, Date &date) {
  in >> date._year;
  in.ignore(1);
  in >> date._month;
  in.ignore(1);
  in >> date._day;
  return in;
}
std::istream &operator>>(std::istream &in, Time &time) {
  in >> time._hours;
  in.ignore(1);
  in >> time._minutes;
  return in;
}


void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
      {"departure_date", "2018-2-28"},
      {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
      {"price", "12550"},
      {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
  // значения этих полей не должны измениться
  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}