//
// Created by Hamdan Nawaz on 07/05/2025.
//
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <__ostream/basic_ostream.h>
#include <fstream>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#ifndef AIRLINEFINE_H
#define AIRLINEFINE_H

struct AVNEntry {
    std::string AVNID;
    std::string airline;
    std::string flightID;
    std::string type;
    std::string speed;
    std::string allowedSpeed;
    std::string date;
    std::string dueDate;
    std::string fine;
    std::string status;
};

void AirlineFine() {
    InitWindow(800, 600, "raygui Example");
    SetTargetFPS(60);

    bool showMessageBox = false;
    char filename[1024] = {0};
    bool active = false;
    Vector2 scroll = {0, 0};
    Rectangle panelBounds = {10, 10, 780, 580};
    Rectangle contentBounds = {10, 10, 780, 580};
    Rectangle panelView = {0};
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!active){
            if (GuiTextInputBox((Rectangle){300, 80, 200, 150}, "Airline Name", "Enter AirlineID", "Enter", filename, 1024,
                               NULL) != -1) {
                showMessageBox = true;
                               }


            if (showMessageBox) {
                int result = GuiMessageBox((Rectangle){300, 200, 200, 100},
                                           "Airline", "Logged In", "OK");

                if (result >= 0) {
                    showMessageBox = false;
                    active = true;
                }
            }
        } else {
            std::string fn(filename);
            std::ifstream file(fn + ".txt");
            if (!file.is_open()) {
                std::cerr << "Could not open the file!" << std::endl;
                exit(-1);
            }
            std::vector<AVNEntry> avnEntries;

            while (!file.eof()) {
                AVNEntry avnEntry;
                file >> avnEntry.AVNID >> avnEntry.airline >> avnEntry.flightID >> avnEntry.type >> avnEntry.speed >> avnEntry.allowedSpeed >> avnEntry.date >> avnEntry.dueDate >> avnEntry.fine >> avnEntry.status;
                avnEntries.push_back(avnEntry);
                //std::cout << "Added AVN Entry" << std::endl;
            }

            const char ** items = new const char*[avnEntries.size() * 9];
            for (int i = 0; i < avnEntries.size(); i++) {
                items[i*9 + 0] = avnEntries[i].AVNID.c_str();
                items[i*9 + 1] = avnEntries[i].airline.c_str();
                items[i*9 + 2] = avnEntries[i].flightID.c_str();
                items[i*9 + 3] = avnEntries[i].type.c_str();
                items[i*9 + 4] = avnEntries[i].speed.c_str();
                items[i*9 + 5] = avnEntries[i].allowedSpeed.c_str();
                items[i*9 + 6] = avnEntries[i].date.c_str();
                items[i*9 + 7] = avnEntries[i].dueDate.c_str();
                items[i*9 + 8] = avnEntries[i].fine.c_str();
            }

            // GuiListView(panelBounds, NULL, items, avnEntries.size() * 9);

            contentBounds.height = avnEntries.size() * 135;

            GuiScrollPanel(panelBounds, NULL, contentBounds, &scroll, &panelView);

            BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
            int y = 10;
            for (int i = 0; i < avnEntries.size(); i++) {
                for (int j = 0; j < 9; j++) {
                    DrawText(items[i*9 + j], 15, y + scroll.y, 10, BLACK);
                    y+=15;
                }
            }
            EndScissorMode();

        }

        EndDrawing();
    }

    CloseWindow();
    exit(0);
}

#endif //AIRLINEFINE_H
