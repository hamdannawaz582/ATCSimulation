//
// Created by Hamdan Nawaz on 07/05/2025.
//
#include <fcntl.h>
#include <sstream>
#include <string>
#ifndef AVNCHECKER_H
#define AVNCHECKER_H

void fineAirline() {
    std::cout << "Fine" << std::endl;
}

void checkAVN(int recievefd) {
    char buffer[1024];
    int flags = fcntl(recievefd, F_GETFL, 0);
    fcntl(recievefd, F_SETFL, flags | O_NONBLOCK);

    while (true) {
        int bytesRead = read(recievefd, buffer, 1024);
        if (bytesRead == -1) {
            continue;
        }
        buffer[bytesRead] = '\0';

        std::stringstream ss;
        if (bytesRead > 0) {
            ss.write(buffer, bytesRead);
        } else continue;
        std::string ID, phase;
        int speed;
        ss >> ID >> phase >> speed;

        if (phase == "AtGate") {
            if (speed <= 10) goto endfine;
            fineAirline();
        } else if (phase == "Taxi") {
            if (speed <= 30) goto endfine;
            fineAirline();

        } else if (phase == "TakeoffRoll") {
            if (speed <= 290) goto endfine;
            fineAirline();

        } else if (phase == "Climb") {
            if (speed <= 463) goto endfine;
            fineAirline();

        } else if (phase == "Departure") {
            if (speed <= 900 && speed >= 800) goto endfine;
            fineAirline();

        } else if (phase == "Holding") {
            if (speed < 600) goto endfine;
            fineAirline();
        } else if (phase == "Approach") {
            if (speed <= 290 && speed >= 240) goto endfine;
            fineAirline();

        } else if (phase == "Landing") {
            if (speed <= 240 && speed >= 30) goto endfine;
            fineAirline();
        }
        endfine:;
    }

    exit(0);
}



#endif //AVNCHECKER_H
