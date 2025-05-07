//
// Created by Hamdan Nawaz on 16/04/2025.
//

#include "Orchestrator.h"
// #include <SFML/System.hpp>
#include <cstring>
#include <iostream>
#include "AircraftIDList.h"
#include <unordered_map>
#include "pthread.h"
#include <string>
#include <unistd.h>
#include <raylib.h>
#include <chrono>

#define FINE 1000

Orchestrator::Orchestrator() {
    for (int i = 0; i < 3; i++) {
        runways[i] = new Runway{char('A' + i), false, nullptr};
        pthread_mutex_init(&runways[i]->mtx, nullptr);
        pthread_mutex_init(&runways[i]->mtx2, nullptr);
    }
    int index = 0;
    bool flag = false;
    for (auto const& airline : AirlineIDList) {
        auto [identifier, type, aircraft, flights] = airline.second;
        airlines[index++] = new Airline (airline.first, type, aircraft, 0, 8);
        if (!flag && type == "Cargo") {
            airlines[index-1]->aircraftGen("At Gate", "West", true, 0);
            flag = true;
        }
    }
}

void Orchestrator::fineAirline(string airline) {
    for (auto & i : airlines) {
        if (airline == i->name) {
            i->addFine(FINE);
            return;
        }
    }
}

void Orchestrator::checkFines() {
    for (auto const& aircraft : aircrafts) {
        string status = aircraft->get_status();
        if (status == "At Gate") {
            if (aircraft->speed <= 10) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());
        } else if (status == "Taxi") {
            if (aircraft->speed <= 30) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Takeoff Roll") {
            if (aircraft->speed <= 290) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Climb") {
            if (aircraft->speed <= 463) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Departure") {
            if (aircraft->speed <= 900 && aircraft->speed >= 800) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Holding") {
            if (aircraft->speed < 600) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());
        } else if (status == "Approach") {
            if (aircraft->speed <= 290 && aircraft->speed >= 240) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());

        } else if (status == "Landing") {
            if (aircraft->speed <= 240 && aircraft->speed >= 30) goto endfine;
            aircraft->AVN = true;
            fineAirline(aircraft->get_airline());
        }
        endfine:;
    }
}

void* Orchestrator::findAvailableRunway(void* arg) {
    sleep(1);
    Aircraft* aircraft = (Aircraft*)arg;

    if (aircraft->takeoffFlag && aircraft->get_status() == "At Gate" || !(aircraft->takeoffFlag) && aircraft->get_status() == "Holding") {
        if (aircraft->get_type() == "Military" || aircraft->get_type() == "Cargo") {
            printf(">\t%s Waiting to use Runway %c\n", aircraft->get_id().c_str(), runways[2]->ID);
            pthread_mutex_lock(&runways[2]->mtx);
            if (!runways[2]->status) {
                printf(">\t%s Started using Runway %c\n", aircraft->get_id().c_str(), runways[2]->ID);

                runways[2]->status = true;
                pthread_mutex_lock(&runways[2]->mtx);
                runways[2]->aircraftUsing = aircraft;
                pthread_mutex_unlock(&runways[2]->mtx);

                string gate;
                if (aircraft->takeoffFlag) gate = "Departure";
                else gate = "At Gate";

                while (aircraft->phase != gate) {
                    printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                    printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);
                    checkFines();
                    aircraft->SetPhase();
                    auto start = std::chrono::high_resolution_clock::now();
                    while (true) {
                        auto now = std::chrono::high_resolution_clock::now();
                        float elapsedTime = std::chrono::duration<float>(now - start).count();
                        if (elapsedTime >= aircraft->del_phasetime) {
                            break;
                        }
                        aircraft->phasetime = elapsedTime;
                    }
                    // sleep(1);
                }
                // std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                // std::cout << aircraft->get_id() << " Speed: " << aircraft->speed << " km/h.\n";
                printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);

            }
            runways[2]->status = false;
            // std::cout << std::endl;
            pthread_mutex_lock(&runways[2]->mtx2);
            runways[2]->aircraftUsing = nullptr;
            pthread_mutex_unlock(&runways[2]->mtx2);
            RemoveAircraft(aircraft);
            pthread_mutex_unlock(&runways[2]->mtx);
            pthread_exit(NULL);
        } else if (aircraft->direction == "North" || aircraft->direction == "South" || !aircraft->takeoffFlag) {
            // std::cout << std::endl << aircraft->get_id() << " Waiting to use Runway " << runways[0]->ID << "\n";
            printf(">\t%s Waiting to use Runway %c\n", aircraft->get_id().c_str(), runways[0]->ID);

            // std::lock_guard<std::mutex> lock(runways[0]->mtx);
            pthread_mutex_lock(&runways[0]->mtx);
            if (!runways[0]->status) {
                // std::cout << aircraft->get_id() << " Started using Runway " << runways[0]->ID << "\n";
                printf(">\t%s Started using Runway %c\n", aircraft->get_id().c_str(), runways[0]->ID);
                runways[0]->status = true;
                pthread_mutex_lock(&runways[0]->mtx2);
                runways[0]->aircraftUsing = aircraft;
                pthread_mutex_unlock(&runways[0]->mtx2);
                string gate = "At Gate";
                // auto start = std::chrono::high_resolution_clock::now();
                while (aircraft->phase != gate) {
                    // std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                    // std::cout << aircraft->get_id() << " Speed: " << aircraft->speed << " km/h.\n";
                    printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                    printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);
                    checkFines();
                    aircraft->SetPhase();
                    auto start = std::chrono::high_resolution_clock::now();
                    while (true) {
                        auto now = std::chrono::high_resolution_clock::now();
                        float elapsedTime = std::chrono::duration<float>(now - start).count();
                        if (elapsedTime >= aircraft->del_phasetime) {
                            break;
                        }
                        aircraft->phasetime = elapsedTime;
                    }
                    // sleep(1);
                }
                printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);

                // std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                // std::cout << aircraft->get_id() << " Speed: " << aircraft->speed << " km/h.\n";
            }
            runways[0]->status = false;
            // std::cout << std::endl;
            pthread_mutex_lock(&runways[0]->mtx2);
            runways[0]->aircraftUsing = nullptr;
            pthread_mutex_unlock(&runways[0]->mtx2);
            RemoveAircraft(aircraft);
            pthread_mutex_unlock(&runways[0]->mtx);
            pthread_exit(NULL);

        } else if (aircraft->direction == "East" || aircraft->direction == "West" || aircraft->takeoffFlag) {
            printf(">\t%s Waiting to use Runway %c\n", aircraft->get_id().c_str(), runways[1]->ID);
            pthread_mutex_lock(&runways[1]->mtx);
            if (!runways[1]->status) {
                printf(">\t%s Started using Runway %c\n", aircraft->get_id().c_str(), runways[1]->ID);

                runways[1]->status = true;
                pthread_mutex_lock(&runways[1]->mtx2);
                runways[1]->aircraftUsing = aircraft;
                pthread_mutex_unlock(&runways[1]->mtx2);
                string gate = "Departure";
                // auto start = std::chrono::high_resolution_clock::now();
                while (aircraft->phase != gate) {
                    printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                    printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);

                    checkFines();
                    aircraft->SetPhase();
                    auto start = std::chrono::high_resolution_clock::now();
                    while (true) {
                        auto now = std::chrono::high_resolution_clock::now();
                        float elapsedTime = std::chrono::duration<float>(now - start).count();
                        if (elapsedTime >= aircraft->del_phasetime) {
                            break;
                        }
                        aircraft->phasetime = elapsedTime;
                    }

                    // sleep(1);
                }
                printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);

            }
            runways[1]->status = false;
            // std::cout << std::endl;
            pthread_mutex_lock(&runways[1]->mtx2);
            runways[1]->aircraftUsing = nullptr;
            pthread_mutex_unlock(&runways[1]->mtx2);
            RemoveAircraft(aircraft);
            pthread_mutex_unlock(&runways[1]->mtx);
            pthread_exit(NULL);
        }
        // std::lock_guard<std::mutex> lock(runways[2]->mtx);
        pthread_mutex_lock(&runways[2]->mtx);
        if (aircraft->priority == 4) {  //  Priority 4 means Emergency
            // std::cout << std::endl << aircraft->get_id() << " Waiting to use Runway " << runways[2]->ID << "\n";
            printf(">\t%s Waiting to use Runway %c\n", aircraft->get_id().c_str(), runways[2]->ID);

            //  Pre-empting the aircraft from runway[2]
            // std::cout << aircraft->get_id() << " Started using Runway " << runways[2]->ID << "\n";
            printf(">\t%s Started using Runway %c\n", aircraft->get_id().c_str(), runways[2]->ID);

            if (runways[2]->status) {
                // std::cout << runways[2]->aircraftUsing->get_id() << "Preempted.\n";
                printf("%s Preempted.\n", runways[2]->aircraftUsing->get_id().c_str());
                schedule.addFlight(runways[2]->aircraftUsing);
            }
            runways[2]->status = true;
            pthread_mutex_lock(&runways[2]->mtx2);
            runways[2]->aircraftUsing = aircraft;
            pthread_mutex_unlock(&runways[2]->mtx2);

            string gate;
            if (aircraft->takeoffFlag) gate = "Departure";
            else gate = "At Gate";
            // auto start = std::chrono::high_resolution_clock::now();
            while (aircraft->phase != gate) {
                // std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
                // std::cout << aircraft->get_id() << " Speed: " << aircraft->speed << " km/h.\n";
                printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
                printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);


                checkFines();
                aircraft->SetPhase();
                auto start = std::chrono::high_resolution_clock::now();
                while (true) {
                    auto now = std::chrono::high_resolution_clock::now();
                    float elapsedTime = std::chrono::duration<float>(now - start).count();
                    if (elapsedTime >= aircraft->del_phasetime) {
                        break;
                    }
                    aircraft->phasetime = elapsedTime;
                }
                // sleep(1);
            }
            // std::cout << aircraft->get_id() << " Entering " << aircraft->phase << " Phase.\n";
            // std::cout << aircraft->get_id() << " Speed: " << aircraft->speed << " km/h.\n";
            printf("%s Entering %s Phase.\n", aircraft->get_id().c_str(), aircraft->phase.c_str());
            printf("%s Speed: %d km/h.\n", aircraft->get_id().c_str(), aircraft->speed);

        }
        runways[2]->status = false;
        // std::cout << std::endl;
        pthread_mutex_lock(&runways[2]->mtx2);
        runways[2]->aircraftUsing = nullptr;
        pthread_mutex_unlock(&runways[2]->mtx2);
        RemoveAircraft(aircraft);
        pthread_mutex_unlock(&runways[2]->mtx);
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

void Orchestrator::RemoveAircraft(Aircraft* aircraft) {
    for (int i=0 ; i<6 ; i++) {
        if (aircraft->get_airline() == airlines[i]->name) {
            airlines[i]->aircraftReturn(aircraft);
            break;
        }
    }
}

Vector2 getStartEnd(string direction, string phase, int runway) {
    Vector2 startEnd;
    if (runway == 2) {
        if (phase == "At Gate") {
            startEnd.x = 315;
            startEnd.y = 315;
        } else if (phase == "Takeoff Roll") {
            startEnd.x = 235;
            startEnd.y = 100;
        } else if (phase == "Climb") {
            startEnd.x = 100;
            startEnd.y = 0;
        } else if (phase == "Taxi") {
            startEnd.x = 315;
            startEnd.y = 235;
        } else if (phase == "Approach") {
            startEnd.x = 0;
            startEnd.y = 100;
        } else if (phase == "Landing") {
            startEnd.x = 100;
            startEnd.y = 235;
        }
    } else if (runway == 0) {
        if (direction == "North") {
            if (phase == "At Gate") {
                startEnd.x = 315;
                startEnd.y = 315;
            } else if (phase == "Takeoff Roll") {
                startEnd.x = 235;
                startEnd.y = 100;
            } else if (phase == "Climb") {
                startEnd.x = 100;
                startEnd.y = 0;
            } else if (phase == "Taxi") {
                startEnd.x = 315;
                startEnd.y = 235;
            } else if (phase == "Approach") {
                startEnd.x = 0;
                startEnd.y = 100;
            } else if (phase == "Landing") {
                startEnd.x = 100;
                startEnd.y = 235;
            }
        } else if (direction == "South") {
            if (phase == "At Gate") {
                startEnd.x = 315;
                startEnd.y = 315;
            } else if (phase == "Takeoff Roll") {
                startEnd.x = 235;
                startEnd.y = 350;
            } else if (phase == "Climb") {
                startEnd.x = 350;
                startEnd.y = 450;
            } else if (phase == "Taxi") {
                startEnd.x = 315;
                startEnd.y = 235;
            } else if (phase == "Approach") {
                startEnd.x = 450;
                startEnd.y = 350;
            } else if (phase == "Landing") {
                startEnd.x = 350;
                startEnd.y = 235;
            }
        }
    } else {
        if (direction == "West") {
            if (phase == "At Gate") {
                startEnd.x = 225;
                startEnd.y = 225;
            } else if (phase == "Takeoff Roll") {
                startEnd.x = 325;
                startEnd.y = 478;
            } else if (phase == "Climb") {
                startEnd.x = 478;
                startEnd.y = 800;
            } else if (phase == "Taxi") {
                startEnd.x = 225;
                startEnd.y = 325;
            } else if (phase == "Approach") {
                startEnd.x = 0;
                startEnd.y = 193;
            } else if (phase == "Landing") {
                startEnd.x = 193;
                startEnd.y = 325;
            }
        } else if (direction == "East") {
            if (phase == "At Gate") {
                startEnd.x = 225;
                startEnd.y = 225;
            } else if (phase == "Takeoff Roll") {
                startEnd.x = 325;
                startEnd.y = 192;
            } else if (phase == "Climb") {
                startEnd.x = 192;
                startEnd.y = 0;
            } else if (phase == "Taxi") {
                startEnd.x = 225;
                startEnd.y = 325;
            } else if (phase == "Approach") {
                startEnd.x = 800;
                startEnd.y = 475;
            } else if (phase == "Landing") {
                startEnd.x = 475;
                startEnd.y = 325;
            }
        }
    }
    return startEnd;
}

int getCurrent(int start, int end, float progress) {
    if (end > start) {
        return start + (int)(progress * (end - start));
    } else {
        return start - (int)(progress * (start - end));
    }
}

/// Function to create and run the GUI
/// @param arg Object itself
/// @return Nothing
void * Orchestrator::loadGUI(void *arg) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    std::cout << "IN GUI FUNC" << std::endl;
    auto * obj = static_cast<Orchestrator*>(arg);
    SetTraceLogLevel(LOG_ALL);
    InitWindow(screenWidth, screenHeight, "AirTrafficControlX");
    SetTargetFPS(60);
    Vector2 scale = GetWindowScaleDPI();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(5, 5);



        // Drawing the runways
        DrawRectangle(189, 97, 32/scale.x, 256/scale.y, GRAY); // Runway A
        DrawRectangle(221, 321, 256/scale.x, 32/scale.y, GRAY); // Runway B
        DrawRectangle(139, 97, 32/scale.x, 256/scale.y, GRAY); // Runway C

        // Drawing Taxiways
        DrawRectangle(221, 359, 192, 8, GRAY);
        DrawRectangle(176, 129, 8, 192, GRAY);
        DrawRectangle(126, 129, 8, 192, GRAY);

        // Drawing Labels
        DrawText("A", 195, 292, 20, BLACK);
        DrawText("B", 228, 321, 20, BLACK);
        DrawText("C", 145, 292, 20, BLACK);

        DrawCircleV(GetMousePosition(), 4, DARKGRAY);
        DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
            {GetMousePosition().x - 44, GetMousePosition().y - 24}, 20, 2, BLACK);

        for (int i = 0; i < 3; i++) {
            pthread_mutex_lock(&(obj->runways[i]->mtx2));
            if (obj->runways[i]->aircraftUsing == nullptr) {
                pthread_mutex_unlock(&(obj->runways[i]->mtx2));
                continue;
            }
            string phase = obj->runways[i]->aircraftUsing->phase;
            string direction = obj->runways[i]->aircraftUsing->direction;
            float progress = (obj->runways[i]->aircraftUsing->PhaseProgress())/100;
            string ID = (obj->runways[i]->aircraftUsing->get_id());
            // std::cout << ID << " " << phase << " " << direction << " " << progress << " " << i << std::endl;
            pthread_mutex_unlock(&(obj->runways[i]->mtx2));
            Vector2 startEnd = getStartEnd(direction, phase, i);
            if (i == 0) {
                if (phase == "At Gate" || phase == "Taxi") {
                    DrawCircle(202 - 20, getCurrent(startEnd.x, startEnd.y, progress), 4, RED);
                    DrawText(ID.c_str(), 202 - 20, getCurrent(startEnd.x, startEnd.y, progress) - 10, 20, BLACK);
                } else {
                    DrawCircle(202, getCurrent(startEnd.x, startEnd.y, progress), 4, RED);
                    DrawText(ID.c_str(), 202, getCurrent(startEnd.x, startEnd.y, progress) - 10, 20, BLACK);
                }

                // Drawing Text Info
                DrawText("Runway: ", 10, 10, 20, BLACK);
                DrawText("A", 100, 10, 20, BLACK);
                DrawText("Direction: ", 10, 40, 20, BLACK);
                DrawText(direction.c_str(), 100, 40, 20, BLACK);
                DrawText("Phase: ", 10, 70, 20, BLACK);
                DrawText(phase.c_str(), 100, 70, 20, BLACK);
                DrawText("Progress: ", 10, 100, 20, BLACK);
                DrawText((std::to_string(int(progress*100)) + "%").c_str(), 100, 100, 20, BLACK);

            } else if (i == 1) {
                if (phase == "At Gate" || phase == "Taxi") {
                    DrawCircle(getCurrent(startEnd.x, startEnd.y, progress), 340 + 20, 4, GREEN);
                    DrawText(ID.c_str(), getCurrent(startEnd.x, startEnd.y, progress) - 10, 340 + 40, 20, BLACK);
                } else {
                    DrawCircle(getCurrent(startEnd.x, startEnd.y, progress), 340, 4, GREEN);
                    DrawText(ID.c_str(), getCurrent(startEnd.x, startEnd.y, progress) - 10, 340 + 20, 20, BLACK);
                }

                // Drawing Text Info
                DrawText("Runway: ", 550, 10, 20, BLACK);
                DrawText("B", 650, 10, 20, BLACK);
                DrawText("Direction: ", 550, 40, 20, BLACK);
                DrawText(direction.c_str(), 650, 40, 20, BLACK);
                DrawText("Phase: ", 550, 70, 20, BLACK);
                DrawText(phase.c_str(), 650, 70, 20, BLACK);
                DrawText("Prog: ", 550, 100, 20, BLACK);
                DrawText((std::to_string(int(progress*100)) + "%").c_str(), 650, 100, 20, BLACK);

            } else {
                if (phase == "At Gate" || phase == "Taxi") {
                    DrawCircle(152 - 20, getCurrent(startEnd.x, startEnd.y, progress), 4, BLUE);
                    DrawText(ID.c_str(), 152 - 20, getCurrent(startEnd.x, startEnd.y, progress) - 10, 20, BLACK);
                } else {
                    DrawCircle(152, getCurrent(startEnd.x, startEnd.y, progress), 4, BLUE);
                    DrawText(ID.c_str(), 152, getCurrent(startEnd.x, startEnd.y, progress) - 10, 20, BLACK);
                }

                // Drawing Text Info
                DrawText("Runway: ", 550, 300, 20, BLACK);
                DrawText("B", 650, 300, 20, BLACK);
                DrawText("Direction: ", 550, 340, 20, BLACK);
                DrawText(direction.c_str(), 650, 340, 20, BLACK);
                DrawText("Phase: ", 550, 370, 20, BLACK);
                DrawText(phase.c_str(), 650, 370, 20, BLACK);
                DrawText("Prog: ", 550, 400, 20, BLACK);
                DrawText((std::to_string(int(progress*100)) + "%").c_str(), 650, 400, 20, BLACK);
            }


        }

        EndDrawing();
    }
    // pthread_exit(NULL);
}

void Orchestrator::scheduleRunways() {
    //sf::Clock clock;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float>(now - start).count();


        for (size_t i = 0; i < aircrafts.size(); ++i) {
            if (elapsedTime >= aircrafts[i]->arrivaltime) {
                schedule.addFlight(aircrafts[i]);
                Aircraft* tmp = std::move(aircrafts[i]);
                aircrafts.erase(aircrafts.begin() + i);
                printf("\t<<< Scheduled: %s at time %f seconds >>>\n", tmp->get_id().c_str(), elapsedTime);
                tmp->scheduletime = elapsedTime;
                tmp->waittime = elapsedTime - tmp->arrivaltime;
                Aircraft* nextFlight = schedule.getNextFlight();
                pthread_t thread;
                pthread_create(&thread, nullptr, [](void* arg) -> void* {
                auto* params = static_cast<std::pair<Orchestrator*, Aircraft*>*>(arg);
                return params->first->findAvailableRunway(params->second);
                }, new std::pair<Orchestrator*, Aircraft*>(this, nextFlight));

                // pthread_join(thread, NULL);
                pthread_detach(thread);
            }
        }

        if (aircrafts.empty()) {
            break;
        }
    }
}

void Orchestrator::AddFlights() {
   // int numFlights;
   //  std::cout << "Enter the number of flights to add: ";
   //  std::cin >> numFlights;
   //
   //  for (int i = 0; i < numFlights; ++i) {
   //      std::string status, direction;
   //      bool takeoff;
   //      int schedTime, priority, airlineIndex;
   //
   //      std::cout << "\nAdding flight " << (i + 1) << ":\n";
   //
   //      std::cout << "Select Airline:\n";
   //      for (int j = 0; j < 6; ++j) {
   //          std::cout << j + 1 << ". " << airlines[j]->name << "\n";
   //      }
   //      std::cout << "Enter choice (1-6): ";
   //      std::cin >> airlineIndex;
   //      --airlineIndex;
   //
   //      std::cout << "Is this a takeoff flight? (1 for Yes, 0 for No): ";
   //      std::cin >> takeoff;
   //
   //      if (takeoff) {
   //          std::cout << "Select Status:\n1. At Gate\n2. Taxi\n3. Takeoff Roll\n4. Climb\n5. Departure\n";
   //          std::cout << "Enter choice (1-5): ";
   //          int statusChoice;
   //          std::cin >> statusChoice;
   //          if (statusChoice == 1) status = "At Gate";
   //          else if (statusChoice == 2) status = "Taxi";
   //          else if (statusChoice == 3) status = "Takeoff Roll";
   //          else if (statusChoice == 4) status = "Climb";
   //          else if (statusChoice == 5) status = "Departure";
   //      } else {
   //          std::cout << "Select Status:\n1. Holding\n2. Approach\n3. Land\n4. Taxi\n5. At Gate\n";
   //          std::cout << "Enter choice (1-5): ";
   //          int statusChoice;
   //          std::cin >> statusChoice;
   //          if (statusChoice == 1) status = "Holding";
   //          else if (statusChoice == 2) status = "Approach";
   //          else if (statusChoice == 3) status = "Land";
   //          else if (statusChoice == 4) status = "Taxi";
   //          else if (statusChoice == 5) status = "At Gate";
   //      }
   //
   //      if (!takeoff) {
   //          std::cout << "Select Direction:\n1. North\n2. South\n";
   //          std::cout << "Enter choice (1-2): ";
   //          int directionChoice;
   //          std::cin >> directionChoice;
   //          if (directionChoice == 1) direction = "North";
   //          else if (directionChoice == 2) direction = "South";
   //      } else {
   //          std::cout << "Select Direction:\n1. East\n2. West\n";
   //          std::cout << "Enter choice (1-2): ";
   //          int directionChoice;
   //          std::cin >> directionChoice;
   //          if (directionChoice == 1) direction = "East";
   //          else if (directionChoice == 2) direction = "West";
   //      }
   //
   //     std::cout << "Enter schedule time (in seconds): ";
   //     std::cin >> schedTime;
   //
   //     std::cout << "Enter priority (1-4, 4 being highest): ";
   //     std::cin >> priority;
   //
   //     Aircraft* flight = airlines[airlineIndex]->aircraftGen(status, direction, takeoff, schedTime);
   //     if (flight) {
   //         flight->priority = priority;
   //         aircrafts.push_back(flight);
   //         std::cout << "Added flight: " << flight->get_id()
   //                   << " (Priority: " << flight->priority
   //                   << ", Schedule Time: " << flight->scheduletime << ")\n";
   //     } else {
   //         std::cout << "Failed to add flight. Check airline constraints.\n";
   //     }
   //
   //     std::cin.clear();
   //     std::cin.ignore();
   // }
    // // Create and add a mix of aircraft types with different priorities and directions
    // // Commercial flights (lower priority)
    // // Sample Aircraft Entries
    Aircraft* flight1 = airlines[0]->aircraftGen("At Gate", "East", true, 2);  // PIA departure at time 5
    if (flight1) {
        flight1->priority = 1;  // Low priority
        aircrafts.push_back(flight1);
        std::cout << "Added departure: " << flight1->get_id() << " (Priority: " << flight1->priority << ", Schedule Time: " << flight1->arrivaltime << ")\n";
    }

    Aircraft* flight2 = airlines[1]->aircraftGen("Holding", "North", false, 2);  // Airblue arrival at time 10
    if (flight2) {
        flight2->priority = 1;  // Medium priority
        aircrafts.push_back(flight2);
        std::cout << "Added arrival: " << flight2->get_id() << " (Priority: " << flight2->priority << ", Schedule Time: " << flight2->arrivaltime << ")\n";
    }

    Aircraft* flight3 = airlines[2]->aircraftGen("Holding", "South", false, 5);  // FedEx arrival at time 15
    if (flight3) {
        flight3->priority = 1;  // High priority
        aircrafts.push_back(flight3);
        std::cout << "Added cargo arrival: " << flight3->get_id() << " (Priority: " << flight3->priority << ", Schedule Time: " << flight3->arrivaltime << ")\n";
    }

    Aircraft* flight4 = airlines[3]->aircraftGen("At Gate", "West", true, 5);  // Military departure at time 20
    if (flight4) {
        flight4->priority = 1;  // Emergency priority
        aircrafts.push_back(flight4);
        std::cout << "Added military departure: " << flight4->get_id() << " (Priority: " << flight4->priority << ", Schedule Time: " << flight4->arrivaltime << ")\n";
    }

    Aircraft* flight5 = airlines[5]->aircraftGen("Holding", "East", true, 5);  // Medical arrival at time 25
    if (flight5) {
        flight5->priority = 1;  // Emergency priority
        aircrafts.push_back(flight5);
        std::cout << "Added emergency arrival: " << flight5->get_id() << " (Priority: " << flight5->priority << ", Schedule Time: " << flight5->arrivaltime << ")\n";
    }

    // Aircraft* flight1 = airlines[0]->aircraftGen("At Gate", "East", true, 2);  // PIA departure at time 5
    // if (flight1) {
    //     flight1->priority = 2;  // Low priority
    //     aircrafts.push_back(flight1);
    //     std::cout << "Added departure: " << flight1->get_id() << " (Priority: " << flight1->priority << ", Schedule Time: " << flight1->scheduletime << ")\n";
    // }
    //
    // Aircraft* flight2 = airlines[1]->aircraftGen("Holding", "North", false, 2);  // Airblue arrival at time 10
    // if (flight2) {
    //     flight2->priority = 2;  // Medium priority
    //     aircrafts.push_back(flight2);
    //     std::cout << "Added arrival: " << flight2->get_id() << " (Priority: " << flight2->priority << ", Schedule Time: " << flight2->scheduletime << ")\n";
    // }
    //
    // Aircraft* flight3 = airlines[2]->aircraftGen("At Gate", "West", true, 2);  // FedEx arrival at time 15
    // if (flight3) {
    //     flight3->priority = 2;  // High priority
    //     aircrafts.push_back(flight3);
    //     std::cout << "Added cargo arrival: " << flight3->get_id() << " (Priority: " << flight3->priority << ", Schedule Time: " << flight3->scheduletime << ")\n";
    // }
    //
    // Aircraft* flight4 = airlines[3]->aircraftGen("At Gate", "West", true, 2);  // Military departure at time 20
    // if (flight4) {
    //     flight4->priority = 2;  // Emergency priority
    //     aircrafts.push_back(flight4);
    //     std::cout << "Added military departure: " << flight4->get_id() << " (Priority: " << flight4->priority << ", Schedule Time: " << flight4->scheduletime << ")\n";
    // }
    //
    // Aircraft* flight5 = airlines[5]->aircraftGen("Holding", "South", false, 2);  // Medical arrival at time 25
    // if (flight5) {
    //     flight5->priority = 2;  // Emergency priority
    //     aircrafts.push_back(flight5);
    //     std::cout << "Added emergency arrival: " << flight5->get_id() << " (Priority: " << flight5->priority << ", Schedule Time: " << flight5->scheduletime << ")\n";
    // }
    // CloseWindow();
}

void Orchestrator::simulateEmergency() {
}

void Orchestrator::simulateGroundFault() {
}

void* Orchestrator::proceedSimulation(void* arg) {
    // TODO: Add timer code to implement delta time after adding SFML
    Orchestrator* obj = static_cast<Orchestrator*>(arg);
    obj->AddFlights();
    obj->scheduleRunways();
    obj->simulateEmergency();

    for (auto const& aircraft : obj->aircrafts) {
        if (aircraft->get_status() != "Holding" && aircraft->get_status() != "Approach" && aircraft->get_status() != "Departure") {
            obj->simulateGroundFault();
        }
    }
    // pthread_exit(NULL);
}

Orchestrator::~Orchestrator() {
}
