//
// Created by mariakolyachko on 3.01.20.
//

#ifndef FINAL_PROJECT_DATE_H
#define FINAL_PROJECT_DATE_H

#include <iomanip>
#include <iostream>
#include <string>

class Date {
public:
  Date(int year = 0, int month = 1, int day = 1)
      : _day(day), _month(month), _year(year) {
  }

  friend std::ostream &operator<<(std::ostream &stream, const Date &item);
  friend std::istream &operator>>(std::istream &stream, Date &item);
  friend bool operator<(const Date &lhs, const Date &rhs);
  friend bool operator==(const Date &lhs, const Date &rhs);

private:
  int _day;
  int _month;
  int _year;
};

Date ParseDate(std::istream &stream);

#endif // FINAL_PROJECT_DATE_H
