cmake_minimum_required(VERSION 3.30)
project(ATCSimulation)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(ATCSimulation main.cpp
        Airline.cpp
        Airline.h
        Aircraft.cpp
        Aircraft.h
        AircraftIDList.h
        Runways.h
        Orchestrator.cpp
        Orchestrator.h
        FlightSchedule.cpp
        FlightSchedule.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
