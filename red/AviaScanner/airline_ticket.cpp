////
//// Created by mariakolyachko on 17.01.20.
////
//
//#include "airline_ticket.h"
//
//bool operator<(const Date &first, const Date &second) {
//  if (first._year == second._year) {
//    if (first._month == second._month) {
//      return first._day < second._day;
//    } else {
//      return first._month < second._month;
//    }
//  } else {
//    return first._year < second._year;
//  }
//}
//
//bool operator==(const Date &first, const Date &second) {
//  return first._year == second._year && first._month == second._month &&
//         first._day == second._day;
//}
//
//bool operator!=(const Date &first, const Date &second) {
//  return !(first == second);
//}
//
//std::ostream &operator<<(std::ostream &out, const Date &date) {
//  out << date._year << "-" << date._month << "-" << date._day;
//  return out;
//}
//
//bool operator<(const Time &first, const Time &second) {
//  if (first._hours == second._hours) {
//    return first._minutes < second._minutes;
//  } else {
//    return first._hours < second._hours;
//  }
//}
//
//bool operator==(const Time &first, const Time &second) {
//  return first._hours == second._hours && first._minutes == second._minutes;
//}
//
//bool operator!=(const Time &first, const Time &second) {
//  return !(first == second);
//}
//
//std::ostream &operator<<(std::ostream &out, const Time &time) {
//  out << time._hours << ":" << time._minutes;
//  return out;
//}
//std::istream &operator>>(std::istream &in, Date &date) {
//  in >> date._year;
//  in.ignore(1);
//  in >> date._month;
//  in.ignore(1);
//  in >> date._day;
//  return in;
//}
//std::istream &operator>>(std::istream &in, Time &time) {
//  in >> time._hours;
//  in.ignore(1);
//  in >> time._minutes;
//  return in;
//}
