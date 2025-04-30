//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef AIRLINE_H
#define AIRLINE_H
#include <string>

#include "Aircraft.h"

using std::string;

/// Factory Class for Generating Airplanes
class Airline {
    string type;
    int aircraftNumber;
    int aircraftDeployed;
    int flightsInOperation;
    int finescollected;

public:
    string name;

    Airline(string name, string type, int aircraftNumber, int aircraftDeployed, int flightsInOperation);
    Aircraft * aircraftGen(string status, string direction, bool takeoff, int schedTime);
    bool aircraftAvailable();
    bool aircraftReturn(Aircraft * aircraft);
    void addFine(int fine);
};



#endif //AIRLINE_H
