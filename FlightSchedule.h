//
// Created by areeb on 4/29/25.
//

#ifndef FLIGHTSCHEDULE_H
#define FLIGHTSCHEDULE_H

#include "Aircraft.h"
#include <queue>
#include <vector>

class FlightSchedule {
    //  Priority is (1-low and 4-high)
    struct ComparePriority {
        bool operator()(Aircraft* a, Aircraft* b) {
            // return a->priority < b->priority;
            if (a->priority != b->priority)
                return a->priority < b->priority; // Higher priority first
            else
                return a->arrivaltime > b->arrivaltime;
        }
    };

    //  Incoming and Outgoing Flights
    std::priority_queue<Aircraft*, std::vector<Aircraft*>, ComparePriority> departures;
    std::priority_queue<Aircraft*, std::vector<Aircraft*>, ComparePriority> arrivals;

public:
    /// Adds Flights to Arrival and Departure Queues
    /// @param flight Flight to be added into priority queue
    void addFlight(Aircraft* flight);

    /// Pops the Departure Queue
    /// @return Returns the next Departure Flight
    Aircraft* getNextDeparture();

    /// Pops the Arrival Queue
    /// @return Returns the next Arrival Flight
    Aircraft* getNextArrival();

    /// Checks if Both Queues are empty
    /// @return Checks if Both Queues are empty
    bool isEmpty() const;

    /// Compares Departure and Arrival Queue Top and returns higher priority Flight
    /// @return Returns the next Flight to be Processed
    Aircraft* getNextFlight();
};



#endif //FLIGHTSCHEDULE_H
