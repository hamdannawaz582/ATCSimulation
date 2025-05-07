//
// Created by Hamdan Nawaz on 07/05/2025.
//
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#ifndef AVNCHECKER_H
#define AVNCHECKER_H

void fineAirline() {
    std::cout << "Fine" << std::endl;
}
int AVNID = 0;

void AddNotice(std::vector<std::string> result) {
    std::string airline, ID, type, speed,phase;
    airline = result[0];


    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    ID = result[1];
    type = result[2];
    speed = result[3];

    std::ofstream file(airline + ".txt", std::ios::app);
    file << AVNID++ << std::endl;
    file << airline << std::endl;
    file << ID << std::endl;
    file << type << std::endl;
    file << speed << std::endl;
    file << result[5] << std::endl;
    file << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << std::endl;
    local_time->tm_mday += 3;
    std::mktime(local_time);
    file << std::put_time(local_time, "%Y-%m-%d") << std::endl;
    long double Fine = 0;
    if (type == "Commercial")
        Fine = 500000;
    else if (type == "Cargo")
        Fine = 700000;
    Fine *= 1.15;
    file << Fine << std::endl;
    file << "Unpaid" << std::endl << std::endl;
}

void UpdateStatus(int fineFD) {


}

void checkAVN(int recievefd)
{
    int AVNID = 0;
    char buffer[1024];
    int flags = fcntl(recievefd, F_GETFL, 0);
    fcntl(recievefd, F_SETFL, flags | O_NONBLOCK);

    // if (fork() == 0) {
    //     UpdateStatus(fineFD);
    // }

    while (true) {
        int bytesRead = read(recievefd, buffer, 1024);
        if (bytesRead == -1) {
            continue;
        }
        buffer[bytesRead] = '\0';

        std::string input = buffer;
        std::stringstream ss(input);
        std::string segment;
        std::vector<std::string> result;

        while (std::getline(ss, segment, '/')) {
            result.push_back(segment);
        }

        std::string  airline, ID, type, speed,phase;
        airline = result[0];
        ID = result[1];
        type = result[2];
        speed = result[3];
        phase = result[4];

        int ispeed = std::stoi(speed);

        std::string status = phase;
        if (status == "At Gate\n" || status == "At Gate") {
            if (ispeed <= 10) goto endfine;
            result.push_back(std::to_string(10));
            AddNotice(result);
        } else if (status == "Taxi\n" || status == "Taxi") {
            if (ispeed <= 30) goto endfine;
            result.push_back(std::to_string(30));
            AddNotice(result);
        } else if (status == "Takeoff Roll\n" || status == "Takeoff Roll") {
            if (ispeed <= 290) goto endfine;
            result.push_back(std::to_string(290));
            AddNotice(result);
        } else if (status == "Climb\n" || status == "Climb") {
            if (ispeed <= 463) goto endfine;
            result.push_back(std::to_string(463));
            AddNotice(result);
        } else if (status == "Departure\n" || status == "Departure") {
            if (ispeed <= 900 && ispeed >= 800) goto endfine;
            result.push_back(std::to_string(800) + '-' + std::to_string(900));
            AddNotice(result);
        } else if (status == "Holding\n" || status == "Holding") {
            if (ispeed < 600) goto endfine;
            result.push_back(std::to_string(600));
            AddNotice(result);
        } else if (status == "Approach\n") {
            if (ispeed <= 290 && ispeed >= 240) goto endfine;
            result.push_back(std::to_string(240) + '-' + std::to_string(290));
            AddNotice(result);
        } else if (status == "Landing\n") {
            if (ispeed <= 240 && ispeed >= 30) goto endfine;
            result.push_back(std::to_string(30) + '-' + std::to_string(240));
            AddNotice(result);
        }
        endfine:;
    }
    exit(0);
}


#endif //AVNCHECKER_H
