#include <iostream>
#include <vector>
#include <map>

using namespace std;

void addNewBus(map<string, vector<string>> &buses,
               map<string, vector<string>> &stops) {
    string bus;
    int stop_count;
    cin >> bus >> stop_count;
    for (int i = 0; i < stop_count; i++) {
        string stop;
        cin >> stop;
        buses[bus].push_back(stop);
        stops[stop].push_back(bus);
    }
}

void printBusesForStop(const map<string, vector<string>> &stops,
                       const string &stop) {
    if (stops.find(stop) == stops.end()) {
        std::cout << "No stop" << std::endl;
    } else {
        vector<string> buses = stops.find(stop)->second;
        for (auto &bus : buses) {
            std::cout << bus << ' ';
        }
        std::cout << std::endl;
    }
}

void printStopsForBus(const map<string, vector<string>> &buses,
                      const map<string, vector<string>> &stops,
                      const string &bus
) {
    if (buses.find(bus) == buses.end()) {
        std::cout << "No bus" << std::endl;
    } else {
        vector<string> busStops = buses.find(bus)->second;
        for (const auto &busStop : busStops) {
            auto busesStop = stops.find(busStop)->second;
            std::cout << "Stop " << busStop << ':';
            if (busesStop.size() == 1) {
                std::cout << " no interchange" << std::endl;
            } else {
                for (const auto &item : stops.find(busStop)->second) {
                    if (item != bus) {
                        std::cout << ' ' << item;
                    }
                }
                std::cout << std::endl;
            }
        }
    }
}

void printAllBuses(const map<string, vector<string>> &buses,
                   const map<string, vector<string>> &stops) {
    if (buses.empty()) {
        std::cout << "No buses" << std::endl;
    } else {
        for (const auto &bus : buses) {
            std::cout << "Bus " << bus.first << ':';
            for (const auto &stop : bus.second) {
                std::cout << ' ' << stop;
            }
            std::cout << std::endl;
        }
    }
}

int main() {
    map<string, vector<string>> buses;
    map<string, vector<string>> stops;

    int qr;
    cin >> qr;
    for (int q = 0; q < qr; q++) {
        string query;
        std::cin >> query;
        if (query == "ALL_BUSES") {
            printAllBuses(buses, stops);
        } else {
            if (query == "BUSES_FOR_STOP") {
                string stop;
                std::cin >> stop;
                printBusesForStop(stops, stop);
            } else if (query == "STOPS_FOR_BUS") {
                string bus;
                std::cin >> bus;
                printStopsForBus(buses, stops, bus);
            } else {
                addNewBus(buses, stops);
            }
        }
    }
}