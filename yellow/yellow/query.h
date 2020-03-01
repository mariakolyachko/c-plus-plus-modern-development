//
// Created by mariakolyachko on 20.12.19.
//

#ifndef YELLOW_QUERY_H
#define YELLOW_QUERY_H

#include <vector>
#include <string>
#include <sstream>

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    std::string bus;
    std::string stop;
    std::vector<std::string> stops;
};

std::istream& operator >> (std::istream& is, Query& q);

#endif //YELLOW_QUERY_H
