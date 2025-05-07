//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef RUNWAYS_H
#define RUNWAYS_H
#include <mutex>
#include <atomic>
#include "Aircraft.h"

class Runway {
public:
    char ID;
    std::atomic<bool> status;
    Aircraft * aircraftUsing;
    pthread_mutex_t mtx;
    pthread_mutex_t mtx2;
};

#endif //RUNWAYS_H
