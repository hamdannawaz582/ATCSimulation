//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef RUNWAYS_H
#define RUNWAYS_H
#include "Aircraft.h"

struct Runway {
    char ID;
    bool status;
    Aircraft * aircraftUsing;
};

#endif //RUNWAYS_H
