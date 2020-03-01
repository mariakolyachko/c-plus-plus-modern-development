//
// Created by mariakolyachko on 3.01.20.
//
#include "date.h"

std::ostream &operator<<(std::ostream &stream, const Date &item) {
  stream << std::setw(4) << std::setfill('0') << item._year;
  stream << '-' << std::setw(2) << std::setfill('0') << item._month;
  stream << '-' << std::setw(2) << std::setfill('0') << item._day;
  return stream;
}

std::istream &operator>>(std::istream &stream, Date &item) {
  item = ParseDate(stream);
  return stream;
}

bool operator<(const Date &lhs, const Date &rhs) {
  if (lhs._year == rhs._year) {
    if (lhs._month == rhs._month) {
      return lhs._day < rhs._day;
    }
    return lhs._month < rhs._month;
  }
  return lhs._year < rhs._year;
}

bool operator==(const Date &lhs, const Date &rhs) {
  return lhs._year == rhs._year && lhs._month == rhs._month &&
         lhs._day == rhs._day;
}

Date ParseDate(std::istream &stream) {
  int year, month, day;

  stream >> year;
  stream.ignore(1);
  stream >> month;
  stream.ignore(1);
  stream >> day;

  return Date(year, month, day);
}