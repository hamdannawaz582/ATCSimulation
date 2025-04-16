//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H
#include <vector>

#include "Airline.h"
#include "Runways.h"

using std::vector;

class Orchestrator {
    Runway runways[3];
    vector<Aircraft *> aircrafts;
    Airline * airlines[6];
    void fineAirline(string airline);
    void scheduleRunways();
    void simulateEmergency();
    void simulateGroundFault();
public:
    Orchestrator();
    void checkFines();
    void proceedSimulation();
    ~Orchestrator();
};



#endif //ORCHESTRATOR_H
