//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef RUNWAYS_H
#define RUNWAYS_H
#include <mutex>

#include "Aircraft.h"

class Runway {
public:
    char ID;
    bool status;
    Aircraft * aircraftUsing;
    std::mutex mtx;

};

#endif //RUNWAYS_H
