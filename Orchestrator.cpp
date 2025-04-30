//
// Created by Hamdan Nawaz on 16/04/2025.
//

#include "Orchestrator.h"

#include <cstring>
#include <iostream>

#include "AircraftIDList.h"
#include <unordered_map>
#include "pthread.h"
#include <string>

#define FINE 1000

Orchestrator::Orchestrator() {
    for (int i = 0; i < 3; i++) {
        runways[i] = new Runway{char('A' + i), false, nullptr};
    }
    int index = 0;
    bool flag = false;
    for (auto const& airline : AirlineIDList) {
        auto [identifier, type, aircraft, flights] = airline.second;
        airlines[index++] = new Airline (airline.first, type, aircraft, 0, 2);
        if (!flag && type == "Cargo") {
            airlines[index-1]->aircraftGen("At Gate", "West", true);
            flag = true;
        }
    }
    // Ensure all airlines are initialized
    for (int i = index; i < 6; i++) {
        airlines[i] = nullptr;
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
        endfine:;
    }
}

void* Orchestrator::findAvailableRunway(void* arg) {
    Aircraft* aircraft = (Aircraft*)arg;

    if (aircraft->takeoffFlag && aircraft->get_status() == "At Gate" || !(aircraft->takeoffFlag) && aircraft->get_status() == "Holding") {
        if (aircraft->get_type() == "Military" || aircraft->get_type() == "Cargo") {
            //  Auto unlocks the mutex when the scope ends
            std::cout << aircraft->get_id() << " Waiting to use Runway " << runways[2]->ID << "\n";
            // std::lock_guard<std::mutex> lock(runways[2]->mtx);
            runways[2]->mtx.lock();
            if (!runways[2]->status) {
                std::cout << aircraft->get_id() << " Started using Runway " << runways[2]->ID << "\n";
                runways[2]->status = true;
                runways[2]->aircraftUsing = aircraft;
            }
            runways[2]->status = false;
            runways[2]->mtx.unlock();
            pthread_exit(NULL);
        } else if (aircraft->direction == "North" || aircraft->direction == "South" || !aircraft->takeoffFlag) {
            std::cout << aircraft->get_id() << " Waiting to use Runway " << runways[0]->ID << "\n";
            // std::lock_guard<std::mutex> lock(runways[0]->mtx);
            runways[0]->mtx.lock();
            if (!runways[0]->status) {
                std::cout << aircraft->get_id() << " Started using Runway " << runways[0]->ID << "\n";
                runways[0]->status = true;
                runways[0]->aircraftUsing = aircraft;
                string gate = "Gate";
                while (aircraft->phase != gate) {
                    std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                    aircraft->SetPhase();
                }
                std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
            }
            runways[0]->status = false;
            runways[0]->mtx.unlock();
            pthread_exit(NULL);

        } else if (aircraft->direction == "East" || aircraft->direction == "West" || aircraft->takeoffFlag) {
            std::cout << aircraft->get_id() << " Waiting to use Runway " << runways[1]->ID << "\n";
            // std::lock_guard<std::mutex> lock(runways[1]->mtx);
            runways[1]->mtx.lock();
            if (!runways[1]->status) {
                std::cout << aircraft->get_id() << " Started using Runway " << runways[1]->ID << "\n";

                runways[1]->status = true;
                runways[1]->aircraftUsing = aircraft;
                string gate = "Departure";
                while (aircraft->phase != gate) {
                    std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                    aircraft->SetPhase();
                }
                std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
            }
            runways[1]->status = false;
            runways[1]->mtx.unlock();
            pthread_exit(NULL);
        }
        // std::lock_guard<std::mutex> lock(runways[2]->mtx);
        runways[2]->mtx.lock();
        if (aircraft->priority == 4) {  //  Priority 3 means Emergency
            std::cout << aircraft->get_id() << " Waiting to use Runway " << runways[2]->ID << "\n";
            //  Pre-empting the aircraft from runway[2]
            std::cout << aircraft->get_id() << " Started using Runway " << runways[2]->ID << "\n";
            if (runways[2]->status) {
                std::cout << runways[2]->aircraftUsing->get_id() << "Preempted.\n";
                schedule.addFlight(runways[2]->aircraftUsing);
            }
            runways[2]->status = true;
            runways[2]->aircraftUsing = aircraft;

            string gate;
            if (aircraft->takeoffFlag) gate = "Departure";
            else gate = "Gate";

            while (aircraft->phase != gate) {
                std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                aircraft->SetPhase();
            }
            std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
        }
        runways[2]->status = false;
        runways[2]->mtx.unlock();
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

void Orchestrator::scheduleRunways() {
    // for (auto const& aircraft : aircrafts) {
    //     // Assign runways based on direction of travel and type
    //     if (aircraft->takeoffFlag && aircraft->get_status() == "At Gate" || !(aircraft->takeoffFlag) && aircraft->get_status() == "Holding") {
    //         if (aircraft->get_type() == "Military" || aircraft->get_type() == "Cargo") {
    //             runways[2].aircraftUsing = aircraft;
    //         } else if (aircraft->direction == "North" || aircraft->direction == "South") {
    //             runways[0].aircraftUsing = aircraft;
    //         } else if (aircraft->direction == "East" || aircraft->direction == "West") {
    //             runways[1].aircraftUsing = aircraft;
    //         }
    //     }
    // }
    while (!schedule.isEmpty()) {
        Aircraft* nextFlight = schedule.getNextFlight();
        pthread_t thread;
        // pthread_create(&thread, NULL, findAvailableRunway, ((void*)nextFlight));
        pthread_create(&thread, nullptr, [](void* arg) -> void* {
        auto* params = static_cast<std::pair<Orchestrator*, Aircraft*>*>(arg);
        return params->first->findAvailableRunway(params->second);
        }, new std::pair<Orchestrator*, Aircraft*>(this, nextFlight));

        // pthread_join(thread, nullptr);
    }

}

void Orchestrator::AddFlights() {
    // Create and add a mix of aircraft types with different priorities and directions

    // Commercial flights (lower priority)
    Aircraft* flight1 = airlines[0]->aircraftGen("At Gate", "East", true);  // PIA departure
    if (flight1) {
        flight1->priority = 1;  // Low priority
        aircrafts.push_back(flight1);
        schedule.addFlight(flight1);
        std::cout << "Added departure: " << flight1->get_id() << " (Priority: " << flight1->priority << ")\n";
    }

    Aircraft* flight2 = airlines[1]->aircraftGen("Holding", "North", false);  // Airblue arrival
    if (flight2) {
        flight2->priority = 2;  // Medium priority
        aircrafts.push_back(flight2);
        schedule.addFlight(flight2);
        std::cout << "Added arrival: " << flight2->get_id() << " (Priority: " << flight2->priority << ")\n";
    }

    // Cargo flights (medium-high priority)
    Aircraft* flight3 = airlines[2]->aircraftGen("Holding", "South", false);  // FedEx arrival
    if (flight3) {
        flight3->priority = 2;  // Medium priority
        aircrafts.push_back(flight3);
        schedule.addFlight(flight3);
        std::cout << "Added cargo arrival: " << flight3->get_id() << " (Priority: " << flight3->priority << ")\n";
    }

    // Military flights (high priority)
    Aircraft* flight4 = airlines[3]->aircraftGen("At Gate", "West", true);  // Military departure
    if (flight4) {
        flight4->priority = 3;  // High priority
        aircrafts.push_back(flight4);
        schedule.addFlight(flight4);
        std::cout << "Added military departure: " << flight4->get_id() << " (Priority: " << flight4->priority << ")\n";
    }

    // Emergency flight (highest priority)
    Aircraft* flight5 = airlines[5]->aircraftGen("Holding", "East", false);  // Medical arrival
    if (flight5) {
        flight5->priority = 4;  // Emergency priority
        aircrafts.push_back(flight5);
        schedule.addFlight(flight5);
        std::cout << "Added emergency arrival: " << flight5->get_id() << " (Priority: " << flight5->priority << ")\n";
    }

    // Additional flights to test capacity
    Aircraft* flight6 = airlines[0]->aircraftGen("At Gate", "South", true);  // PIA departure
    if (flight6) {
        flight6->priority = 1;
        aircrafts.push_back(flight6);
        schedule.addFlight(flight6);
        std::cout << "Added departure: " << flight6->get_id() << " (Priority: " << flight6->priority << ")\n";
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
