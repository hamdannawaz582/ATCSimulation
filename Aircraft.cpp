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

Aircraft::Aircraft(int id, string airline, string type, string status, string direction, bool takeoff, int schedTime) {
    this->airline = airline;
    this->type = type;
    this->status = status;
    this->ID = id;

    this->speed = 0;
    this->AVN = false;
    this->direction = direction;
    this->takeoffFlag = takeoff;
    this->priority = -1;
    this->phase = status;
    this->arrivaltime = schedTime;
    this->waittime = 0;
    SetSpeed();
    del_phasetime = 2;
}

void Aircraft::SetSpeed() {
    int min = 0, max = 0;
    if (!takeoffFlag)
    {
        if (phase == "Holding") {
            min = 400; max = 600;
        } else if (phase == "Approach") {
            min = 240; max = 290;
        } else if (phase == "Land") {
            min = 30; max = 240;
        } else if (phase == "Taxi") {
            min = 15; max = 30;
        } else if (phase == "At Gate") {
            min = 0; max = 5;
        }

        int option = rand() % 3;

        if (option == 0) {
            speed = (rand() % 25) + (min - 25); // [min-20, min-1]
        } else if (option == 1) {
            speed = (rand() % (max - min + 1)) + min; // [min, max]
        } else {
            speed = (rand() % 25) + (max + 1); // [max+1, max+20]
        }
    }
    else
    {
        if (phase == "At Gate") {
            min = 0; max = 5;
        } else if (phase == "Taxi") {
            min = 15; max = 30;
        } else if (phase == "Takeoff Roll") {
            min = 0; max = 290;
        } else if (phase == "Climb") {
            min = 250; max = 463;
        } else if (phase == "Departure") {
            min = 800; max = 900;
        }

        int option = rand() % 3;

        if (option == 0) {
            speed = (rand() % 5) + (min - 5); // [min-50, min-1]
        } else if (option == 1) {
            speed = (rand() % (max - min + 1)) + min; // [min, max]
        } else {
            speed = (rand() % 5) + (max + 1); // [max+1, max+50]
        }
    }
    speed = abs(speed);
}

void Aircraft::SetPhase() {
    if (takeoffFlag) {
        if (phase == "At Gate") {
            phase = "Taxi";
        }
        else if (phase == "Taxi") {
            phase = "Takeoff Roll";
        }
        else if (phase == "Takeoff Roll") {
            phase = "Climb";
        }
        else if (phase == "Climb") {
            phase = "Departure";
        }
    }
    else {
        if (phase == "Holding") {
            phase = "Approach";
        }
        else if (phase == "Approach") {
            phase = "Land";
        }
        else if (phase == "Land") {
            phase = "Taxi";
        }
        else if (phase == "Taxi") {
            phase = "At Gate";
        }
    }
    status = phase;
    SetSpeed();
}
float Aircraft::PhaseProgress() {
    return (phasetime/del_phasetime)*100;
}

