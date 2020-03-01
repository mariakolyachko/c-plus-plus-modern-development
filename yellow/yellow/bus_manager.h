//
// Created by mariakolyachko on 19.12.19.
//

#ifndef YELLOW_BUS_MANAGER_H
#define YELLOW_BUS_MANAGER_H


#include <iostream>
#include <cassert>
#include <vector>
#include "responses.h"

class BusManager {
public:
    void AddBus(const std::string& bus, const std::vector<std::string>& stops);
    BusesForStopResponse GetBusesForStop(const std::string& stop) const;
    StopsForBusResponse GetStopsForBus(const std::string& bus) const;
    AllBusesResponse GetAllBuses() const;
private:
    std::map<std::string, std::vector<std::string>> buses_to_stops;
    std::map<std::string, std::vector<std::string>> stops_to_buses;
};


#endif //YELLOW_BUS_MANAGER_H
