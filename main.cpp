#include <iostream>
#include <unistd.h>
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
    pthread_create(&FlightSim, NULL, Orchestrator::proceedSimulation, &orchestrator);
    pthread_detach(FlightSim);
    // orchestrator.proceedSimulation();
    if (fork() == 0) {
        checkAVN(AVNFD[0]);
    }
    Orchestrator::loadGUI(&orchestrator);
    pthread_exit(NULL);

    return 0;
}