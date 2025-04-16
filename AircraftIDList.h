//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef AIRCRAFTIDLIST_H
#define AIRCRAFTIDLIST_H
#include <unordered_map>
#include <string>
#include <tuple>
using std::string;
using std::tuple;

/// Mapping of Airline Names to triple digit ID Tags
inline std::unordered_map<string, tuple<string, string, int, int>> AirlineIDList = {
    {"PIA", {"PIA", "Commercial", 6, 4}},
    {"Airblue", {"ABL", "Commercial", 4, 4}},
    {"FedEx", {"FEX", "Cargo", 3, 2}},
    {"Pakistan Airforce", {"PAF", "Military", 2, 1}},
    {"BlueDart", {"BDA", "Cargo", 2, 2}},
    {"AghaKhan Air Ambulance", {"AAA", "Medical", 2, 1}}
};

#endif //AIRCRAFTIDLIST_H
