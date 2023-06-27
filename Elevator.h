#ifndef ELEVATOR_H_
#define ELEVATOR_H_

#include <string>
#include <vector>
#include <exception>
#include "Config.h"
#include "QueuesAtFloors.h"

class Elevator {
public:
    const static int ELEVATORSTATEMAXFIELDWIDTH = 7;

    enum ElevatorState {
        OPEN,
        CLOSED,
        OPENING,
        CLOSING,
        BROKEN
    };
private:
    ElevatorState state{ ElevatorState::OPEN };
    std::vector<Person> capacity;

public:

    void setState(ElevatorState state) { this->state = state; }
    ElevatorState getState() { return state; }

    static std::string elevatorStateToString(ElevatorState state) {
        switch (state) {
        case(ElevatorState::OPEN): { return "OPEN"; }
        case(ElevatorState::CLOSED): { return "CLOSED"; }
        case(ElevatorState::OPENING): { return "OPENING"; }
        case(ElevatorState::CLOSING): { return "CLOSING"; }
        case(ElevatorState::BROKEN): { return "BROKEN"; }
        default:
            throw std::exception("Elevator instance in invalid state");
        }
    }

    int currentFloorVar{ 0 };
    const int CAPACITY{ 8 };
    
    bool isOpen() { return state == ElevatorState::OPEN; }
    void open() { setState(ElevatorState::OPENING); std::this_thread::sleep_for(LIFTTIMEOPENING); setState(ElevatorState::OPEN); }
    void close() { setState(ElevatorState::CLOSING); std::this_thread::sleep_for(LIFTTIMECLOSING); setState(ElevatorState::CLOSED); std::this_thread::sleep_for(LIFTTIMECLOSEDBEFOREMOVE); }
    void movedown() { if (isOpen()) { close(); } --currentFloorVar; std::this_thread::sleep_for(LIFTTIMEBETWEENFLOORS); }
    void moveup() { if (isOpen()) { close(); } ++currentFloorVar; std::this_thread::sleep_for(LIFTTIMEBETWEENFLOORS); }

    const int currentFloor() { return currentFloorVar; }

    int occupants() { return capacity.size(); }

    bool full() { return occupants() == CAPACITY; }
    
    void addPerson(Person p) {
        if (full()) { throw std::exception("addPerson called on lift at capacity"); }
        std::this_thread::sleep_for(LIFTTIMEACCEPTPERSON);
        capacity.push_back(p);
    }

    std::vector<Person> deloadPerson(){
        if (!isOpen()) { open(); }

        std::vector<Person> ret;
        capacity.erase(std::remove_if(capacity.begin(), capacity.end(), [&](Person pers) {
            if (pers.desiredFloor == currentFloorVar) {
                ret.push_back(pers);
                return true;
            }
            return false; }), capacity.end());
            
        return ret;
    }

};

#endif