//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H
#include <vector>
//#include <SFML/System/Clock.hpp>

#include "Airline.h"
#include "FlightSchedule.h"
#include "Runways.h"

using std::vector;

class Orchestrator {
    Runway* runways[3];
    vector<Aircraft *> aircrafts;
    Airline * airlines[6];
    FlightSchedule schedule;
    // sf::Clock clock;
    void fineAirline(string airline);
    void scheduleRunways();
    void simulateEmergency();
    void simulateGroundFault();
    void* findAvailableRunway(void* arg);
    void RemoveAircraft(Aircraft* aircraft);


public:
    int fd;
    Orchestrator();
    void AddFlights();
    void checkFines(Aircraft* aircraft);
    static void * proceedSimulation(void* arg);
    static void * loadGUI(void *arg);
    ~Orchestrator();
};



#endif //ORCHESTRATOR_H
