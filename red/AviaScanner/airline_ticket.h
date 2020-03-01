//
// Created by mariakolyachko on 12.01.20.
//

#ifndef AVIASCANNER_AIRLINE_TICKET_H
#define AVIASCANNER_AIRLINE_TICKET_H

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Date {
  int _year, _month, _day;

  Date () {
    _year = 0;
    _month = _day = 1;
  }

  Date(int year, int month, int day) : _year(year), _month(month), _day(day) {}
};

bool operator<(const Date &first, const Date &second);
bool operator==(const Date &first, const Date &second);
bool operator!=(const Date &first, const Date &second);
std::ostream &operator<<(std::ostream &os, const Date &date);
std::istream &operator>>(std::istream &in, Date &date);

struct Time {
  int _hours, _minutes;

  Time() {
    _hours = _minutes = 0;
  }

  Time(int hours, int minutes) : _hours(hours), _minutes(minutes) {}
};

bool operator<(const Time &first, const Time &second);
bool operator==(const Time &first, const Time &second);
bool operator!=(const Time &first, const Time &second);
std::ostream &operator<<(std::ostream &out, const Time &time);
std::istream &operator>>(std::istream &in, Time &time);

struct AirlineTicket {
  string from;
  string to;
  string airline;
  Date departure_date;
  Time departure_time;
  Date arrival_date;
  Time arrival_time;
  int price;
};

#endif // AVIASCANNER_AIRLINE_TICKET_H
