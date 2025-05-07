#include <iostream>
#include "Orchestrator.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Orchestrator orchestrator;
    // orchestrator.AddFlights();
    pthread_t FlightSim;
    pthread_create(&FlightSim, NULL, Orchestrator::proceedSimulation, &orchestrator);
    pthread_detach(FlightSim);
    // orchestrator.proceedSimulation();
    pthread_t GUIThread;
    pthread_create(&GUIThread, NULL, Orchestrator::loadGUI, &orchestrator);
    pthread_detach(GUIThread);
    pthread_exit(NULL);

    return 0;
}