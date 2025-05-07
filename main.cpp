#include <iostream>
#include <unistd.h>
#include "AVNChecker.h"
#include "Orchestrator.h"
#include "AirlineFine.h""
//  Added all File Descriptors to Orchestrator.h
int AVNFD[2];
int FineFD[2];
int StripePayFD[2];

int main() {
    std::cout << "Hello, World!" << std::endl;

    pipe(AVNFD);
    pipe(FineFD);
    pipe(StripePayFD);
    Orchestrator orchestrator;
    orchestrator.fd = AVNFD[1];
    pthread_t FlightSim;
    if (fork() == 0) {
        checkAVN(AVNFD[0]);
    }
    if (fork() == 0) {
        AirlineFine();
    }
    pthread_create(&FlightSim, NULL, Orchestrator::proceedSimulation, &orchestrator);
    pthread_detach(FlightSim);

    Orchestrator::loadGUI(&orchestrator);
    pthread_exit(NULL);

    return 0;
}