//
// Created by Hamdan Nawaz on 16/04/2025.
//

#ifndef AIRCRAFT_H
#define AIRCRAFT_H
#include <string>

using std::string;


/// Class representing Aircraft
class Aircraft {
    // TODO: Add speed variables and such later
    int ID;
    string airline;
    string type;
    string status;

public:
    int arrivaltime;
    int scheduletime;
    int waittime;
    string phase;
    int priority;
    int speed;
    bool AVN;
    string direction;
    /// True if aircraft is taking off, false if landing
    bool takeoffFlag;
    /// Get identifier of Aircraft
    /// @return Alphanumeric Identifier
    string get_id() const;

    /// Get Airline Name
    /// @return Airline name
    string get_airline() const;

    /// Get Aircraft Type
    /// @return Aircraft Type
    string get_type() const;

    /// Get Aircraft Status
    /// @return Aircraft Status
    string get_status() const;

    Aircraft(int id, string airline, string type, string status, string direction, bool takeoff,int schedTime);
    void SetSpeed();
    void SetPhase();
};



#endif //AIRCRAFT_H
