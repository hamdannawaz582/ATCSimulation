#include <iostream>
#include "Orchestrator.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Orchestrator orchestrator;
    orchestrator.AddFlights();
    orchestrator.proceedSimulation();
    return 0;
}