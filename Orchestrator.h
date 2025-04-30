//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H
#include <vector>

#include "Airline.h"
#include "FlightSchedule.h"
#include "Runways.h"

using std::vector;

class Orchestrator {
    Runway* runways[3];
    vector<Aircraft *> aircrafts;
    Airline * airlines[6];
    FlightSchedule schedule;
    void fineAirline(string airline);
    void scheduleRunways();
    void simulateEmergency();
    void simulateGroundFault();
    void* findAvailableRunway(void* arg);

public:
    Orchestrator();
    void AddFlights();
    void checkFines();
    void proceedSimulation();
    ~Orchestrator();
};



#endif //ORCHESTRATOR_H
