//
// Created by areeb on 4/29/25.
//

#include "FlightSchedule.h"

void FlightSchedule::addFlight(Aircraft* flight) {
    if (!flight->takeoffFlag) arrivals.push(flight);
    else departures.push(flight);
}

Aircraft* FlightSchedule::getNextDeparture() {
    if (departures.empty()) return nullptr;
    Aircraft* next = departures.top();
    departures.pop();
    return next;
}

Aircraft* FlightSchedule::getNextArrival() {
    if (arrivals.empty()) return nullptr;
    Aircraft* next = arrivals.top();
    arrivals.pop();
    return next;
}

bool FlightSchedule::isEmpty() const {
    return departures.empty() && arrivals.empty();
}

Aircraft* FlightSchedule::getNextFlight() {
    if (arrivals.empty()) return getNextDeparture();
    if (departures.empty()) return getNextArrival();

    if (arrivals.top()->priority >= departures.top()->priority)
        return getNextArrival();
    else
        return getNextDeparture();
}