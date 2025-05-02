#include <iostream>
#include "Orchestrator.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Orchestrator orchestrator;
    orchestrator.AddFlights();
    orchestrator.proceedSimulation();
    pthread_exit(NULL);

    return 0;
}