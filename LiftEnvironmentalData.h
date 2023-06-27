#ifndef LIFTENVIRONMENTALDATA_H_
#define LIFTENVIRONMENTALDATA_H_

#include "Elevator.h"
#include <thread>

class LiftEnvironmentData {
    std::mutex mu;

    QueuesAtFloors* queuesRef;
    QueuesAtFloors* outputsRef;
    std::vector<bool> floorBeingServiced;

public:
    LiftEnvironmentData(QueuesAtFloors* queueRef, QueuesAtFloors* outputRef) : queuesRef(queueRef), outputsRef{ outputRef } { floorBeingServiced.resize(queueRef->floorCount(), false); }

    LiftEnvironmentData() : queuesRef(nullptr), outputsRef(nullptr) {}

    bool externalRequestAtFloor(int currFloor) {
        return queuesRef->peopleAtFloor(currFloor) != 0;
    }

    const std::vector<bool>& getLiftCalls() {
        return floorBeingServiced;
    }

    void answerFloorCall(int floor) {
        std::lock_guard<std::mutex> lock(mu);
        floorBeingServiced[floor] = true;
    }

    void finishedFloorCall(int floor) {
        std::lock_guard<std::mutex> lock(mu);
        floorBeingServiced[floor] = false;
    }

    void dropoffPers(int floor, Person& pers) {
        if (pers.desiredFloor != floor) {
            throw std::exception("Person served to wrong floor");
        }

        pers.reachedDestination();
        outputsRef->addPersonToFloor(pers, floor);
    }

    const int peopleAtFloor(int floor) {
        return queuesRef->peopleAtFloor(floor);
    }

    Person pickupPersIndiscrimintately(int floor) {
        return queuesRef->removeFirstPersonIndiscriminately(floor);
    }

};

#endif