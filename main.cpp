#include <iostream>
#include <unistd.h>

#include "AirlineFine.h"
#include "AVNChecker.h"
#include "Orchestrator.h"



int AVNFD[2];
int FineFD[2];
int StripePayFD[2];

int main() {
    std::cout << "Hello, World!" << std::endl;
    pipe(AVNFD);
    pipe(FineFD);
    pipe(StripePayFD);
    Orchestrator orchestrator;
    // orchestrator.AddFlights();
    pthread_t FlightSim;
    if (fork() == 0) {
        checkAVN(AVNFD[0]);
    }
    if (fork() == 0) {
        AirlineFine();
    }
    pthread_create(&FlightSim, NULL, Orchestrator::proceedSimulation, &orchestrator);
    pthread_detach(FlightSim);
    // orchestrator.proceedSimulation();

    Orchestrator::loadGUI(&orchestrator);
    pthread_exit(NULL);

    return 0;
}