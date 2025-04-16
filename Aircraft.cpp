//
// Created by Hamdan Nawaz on 16/04/2025.
//
#include "AircraftIDList.h"
#include "Aircraft.h"

string Aircraft::get_id() const {
    return std::get<0>(AirlineIDList[airline]) + std::to_string(ID);
}

string Aircraft::get_airline() const {
    return airline;
}

string Aircraft::get_type() const {
    return type;
}

string Aircraft::get_status() const {
    return status;
}

Aircraft::Aircraft(int id, string airline, string type, string status, string direction, bool takeoff) {
    this->airline = airline;
    this->type = type;
    this->status = status;
    this->ID = id;

    this->speed = 0;
    this->AVN = false;
    this->direction = direction;
    this->takeoffFlag = takeoff;
}
