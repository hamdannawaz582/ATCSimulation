cmake_minimum_required(VERSION 3.30)
project(ATCSimulation)

set(CMAKE_CXX_STANDARD 20)

add_executable(ATCSimulation main.cpp
        Airline.cpp
        Airline.h
        Aircraft.cpp
        Aircraft.h
        AircraftIDList.h
        Runways.h
        Orchestrator.cpp
        Orchestrator.h)
