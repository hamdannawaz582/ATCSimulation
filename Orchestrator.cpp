//
// Created by Hamdan Nawaz on 16/04/2025.
//

#include "Orchestrator.h"
#include "AircraftIDList.h"
#include <unordered_map>

#define FINE 1000

Orchestrator::Orchestrator() {
    for (int i = 0; i < 3; i++) {
        runways[i] = Runway{char('A' + i), false, nullptr};
    }
    int index = 0;
    for (auto const& airline : AirlineIDList) {
        auto [identifier, type, aircraft, flights] = airline.second;
        airlines[index++] = new Airline (airline.first, type, aircraft, flights, 0);
    }
}

void Orchestrator::fineAirline(string airline) {
    for (auto & i : airlines) {
        if (airline == i->name) {
            i->addFine(FINE);
            return;
        }
    }
}

void Orchestrator::checkFines() {
    for (auto const& aircraft : aircrafts) {
        string status = aircraft->get_status();
        if (status == "At Gate") {
            if (aircraft->speed <= 10) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());
        } else if (status == "Taxi") {
            if (aircraft->speed <= 30) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Takeoff Roll") {
            if (aircraft->speed <= 290) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Climb") {
            if (aircraft->speed <= 463) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Departure") {
            if (aircraft->speed <= 900 && aircraft->speed >= 800) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Holding") {
            if (aircraft->speed < 600) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());
        } else if (status == "Approach") {
            if (aircraft->speed <= 290 && aircraft->speed >= 240) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Landing") {
            if (aircraft->speed <= 240 && aircraft->speed >= 30) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());
        }
        endfine:
    }
}

void Orchestrator::scheduleRunways() {
    for (auto const& aircraft : aircrafts) {
        // Assign runways based on direction of travel and type
        if (aircraft->takeoffFlag && aircraft->get_status() == "At Gate" || !(aircraft->takeoffFlag) && aircraft->get_status() == "Holding") {
            if (aircraft->get_type() == "Military" || aircraft->get_type() == "Cargo") {
                runways[2].aircraftUsing = aircraft;
            } else if (aircraft->direction == "North" || aircraft->direction == "South") {
                runways[0].aircraftUsing = aircraft;
            } else if (aircraft->direction == "East" || aircraft->direction == "West") {
                runways[1].aircraftUsing = aircraft;
            }
        }
    }
}

void Orchestrator::simulateEmergency() {
}

void Orchestrator::simulateGroundFault() {
}

void Orchestrator::proceedSimulation() {
    // TODO: Add timer code to implement delta time after adding SFML
    checkFines();
    scheduleRunways();
    simulateEmergency();

    for (auto const& aircraft : aircrafts) {
        if (aircraft->get_status() != "Holding" && aircraft->get_status() != "Approach" && aircraft->get_status() != "Departure") {
            simulateGroundFault();
        }
    }

}

Orchestrator::~Orchestrator() {
}
