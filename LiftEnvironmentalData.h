#ifndef LIFTENVIRONMENTALDATA_H_
#define LIFTENVIRONMENTALDATA_H_

#include <thread>
#include <exception>

class LiftEnvironmentData {
    std::mutex mu;
    QueuesAtFloors* queuesRef;
    QueuesAtFloors* outputsRef;

    //std::vector<bool> floorBeingServiced; // resolving multiple elevators

public:
    LiftEnvironmentData(QueuesAtFloors* queueRef, QueuesAtFloors* outputRef) : queuesRef(queueRef), outputsRef{ outputRef } {
        //floorBeingServiced.resize(queueRef->floorCount(), false);  // resolving multiple elevators
    }

    LiftEnvironmentData() : queuesRef(nullptr), outputsRef(nullptr) {}

    bool externalRequestAtFloor(int currFloor) {
        std::lock_guard<std::mutex> lock(mu);
        return queuesRef->peopleAtFloor(currFloor) != 0;
    }

    //const std::vector<bool>& getLiftCalls() {  // resolving multiple elevators
    //    return floorBeingServiced;
    //}

    //void answerFloorCall(int floor) {  // resolving multiple elevators
    //    std::lock_guard<std::mutex> lock(mu);
    //    floorBeingServiced[floor] = true;
    //}

    //void finishedFloorCall(int floor) {  // resolving multiple elevators
    //    std::lock_guard<std::mutex> lock(mu);
    //    floorBeingServiced[floor] = false;
    //}

    void dropoffPers(int floor, Person& pers) {
        std::lock_guard<std::mutex> lock(mu);
        if (pers.desiredFloor != floor) {
            throw std::exception("Person served to wrong floor");
        }

        pers.reachedDestination();
        outputsRef->addPersonToFloor(pers, floor);
    }

    const int peopleAtFloor(int floor) {
        std::lock_guard<std::mutex> lock(mu);
        return queuesRef->peopleAtFloor(floor);
    }

    Person pickupPersIndiscriminately(int floor) {
        std::lock_guard<std::mutex> lock(mu);
        return queuesRef->removeFirstPersonIndiscriminately(floor);
    }

    const std::vector<bool> getExternalButtonPanel() { return queuesRef->getExternalButtonPanel(); }

};

#endif