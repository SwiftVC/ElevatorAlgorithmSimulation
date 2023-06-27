#ifndef PERSON_H_
#define PERSON_H_

#include <chrono>
#include "Config.h"

class Person {
public:
    int desiredFloor;
    std::chrono::steady_clock::time_point liftUseStarted;
    std::chrono::steady_clock::time_point reachedDestinationTime;

    Person(int desiredFloor) : desiredFloor(desiredFloor)  {
        liftUseStarted = std::chrono::steady_clock::now();
        reachedDestinationTime = std::chrono::steady_clock::time_point::max();
    }

    void reachedDestination(){ reachedDestinationTime = std::chrono::steady_clock::now(); }

    float SecondsForElevatorTravel() {
        /*
        Includes waiting in queue time, takes simulation speed factor into consideration.
        */
        if (reachedDestinationTime == std::chrono::steady_clock::time_point::max()) {
            throw std::exception("Person::getTimeForElevatorTravel called without Person::reachedDestination set");
        }
        
        std::chrono::seconds secs = std::chrono::duration_cast<std::chrono::seconds>(reachedDestinationTime - liftUseStarted);
        float diff = secs.count()*SIMULATIONSPEEDFACTOR;

        return diff;
    }
};

#endif