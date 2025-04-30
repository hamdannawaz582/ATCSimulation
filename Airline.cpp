//
// Created by Hamdan Nawaz on 16/04/2025.
//

#include "Airline.h"

Airline::Airline(string name, string type, int aircraftNumber, int aircraftDeployed, int flightsInOperation) {
    this->name = name;
    this->type = type;
    this->aircraftNumber = aircraftNumber;
    this->aircraftDeployed = aircraftDeployed;
    this->flightsInOperation = flightsInOperation;
    this->finescollected = 0;
}

/// Generator for Aircraft Objects
/// @param status The state in which to generate the Aircraft
/// @return A new Aircraft Object initialized to the Airline's specifications
Aircraft * Airline::aircraftGen(string status, string direction, bool takeoff) {
    // Not enough planes allowed / Not enough exist
    // if ((flightsInOperation >= aircraftDeployed) || (aircraftNumber < aircraftDeployed + 1)) {
    //     return nullptr;
    // }
    return new Aircraft(aircraftDeployed++, name, type, status, direction, takeoff);

}

/// Checker to see if aircraft can be created
/// @return True if more Aircraft can be created, False otherwise
bool Airline::aircraftAvailable() {
    return !(flightsInOperation >= aircraftDeployed) || (aircraftNumber < aircraftDeployed + 1);
}

/// Destroys Aircraft Objects and allows more to be created
/// @param aircraft Aircraft to be destroyed
/// @return True if successful, False otherwise
bool Airline::aircraftReturn(Aircraft *aircraft) {
    // In case the aircraft was sent to the wrong airline
    if (!aircraft || aircraft->get_airline() != name) {
        return false;
    }
    aircraftDeployed--;
    delete aircraft;
    return true;
}

/// Fine Airline for AVN
/// @param fine fine amount
void Airline::addFine(int fine) {
    if (fine <= 0) return;
    finescollected += fine;
}