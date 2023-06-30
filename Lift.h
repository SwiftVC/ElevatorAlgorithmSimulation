#ifndef LIFT_H_
#define LIFT_H_

#include "Elevator.h"
#include "Config.h"
#include "LiftEnvironmentalData.h"
#include <thread>

class Lift;
void liftLogicLoop(Lift& lift);

class Lift {
private:
    int endDestination{ -1 };
    int totalFloors;
    Elevator elev;
    std::thread elevThread;
    LiftEnvironmentData liftEnvData;
    int defaultFloor{ 1 };

    bool canElevMoveUp() { return currentFloor() < totalFloors; }
    bool canElevMoveDown() { return currentFloor() > 1; }

    void deactivateFloorButton(int floor) {
        elev.deactivateFloorButton(floor);
    }
    void activateFloorButton(int floor) {
        elev.activateFloorButton(floor);
    }


    const int getCallsAbove(std::vector<bool> panelState) {
        int currFloor = currentFloor();
        int internalCallsAbove = 0;
        for (int floor = currFloor + 1; floor != panelState.size() + 1; floor++) {
            int floorIndex = floor - 1;
            if (panelState[floorIndex] == true) { ++internalCallsAbove; }
        }
        return internalCallsAbove;
    }

    const int getCallsBelow(std::vector<bool> panelState) {
        int currFloor = currentFloor();
        int internalCallsBelow = 0;
        for (int floor = currFloor - 1; floor != 0; floor--) {
            int floorIndex = floor - 1;
            if (panelState[floorIndex] == true) { ++internalCallsBelow; }
        }
        return internalCallsBelow;
    }

    const int getFurthestCallAbove(std::vector<bool> panelState) {
        for (int floorIndex = panelState.size() - 1; floorIndex != currentFloor() - 1; --floorIndex) {
            if (panelState[floorIndex]) { return floorIndex + 1; }
        }
        throw std::exception("getFurthestCallAbove called with no calls above");
    }

    const int getFurthestCallBelow(std::vector<bool> panelState) {
        for (int floorIndex = 0; floorIndex != currentFloor() - 1; ++floorIndex) {
            if (panelState[floorIndex]) { return floorIndex + 1; }
        }
        throw std::exception("getFurthestCallBelow called with no calls below");
    }

    int pollFloorCalls(std::vector<bool> panelState) {
        // check for furthest internal request in direction of most serves (minimises average time)
        int currFloor = currentFloor();
        int internalCallsAbove = getCallsAbove(panelState);
        int internalCallsBelow = getCallsBelow(panelState);

        if (internalCallsAbove == internalCallsBelow && internalCallsAbove > 0) { return getFurthestCallAbove(panelState); }
        if (internalCallsAbove > internalCallsBelow) { return getFurthestCallAbove(panelState); }

        if (internalCallsAbove < internalCallsBelow) { return getFurthestCallBelow(panelState); }

        return -1;
    }

    int pollExternalFloorCalls() {
        return pollFloorCalls(liftEnvData.getExternalButtonPanel());
    }

    int pollInternalFloorCalls() {
        return pollFloorCalls(elev.getFloorButtonPanel());
    }

public:
    bool stopThread{ false };

    explicit Lift(int floors, QueuesAtFloors* queuesRef, QueuesAtFloors* outputRef) : totalFloors(floors), liftEnvData(queuesRef, outputRef), elev(floors){
        elevThread = std::thread(liftLogicLoop, std::ref(*this));
    }
    Lift(const Lift& li) { throw std::exception(); }

    void moveUp() {
        if (canElevMoveUp()) { elev.moveup(); }
    }
    void moveDown() {
        if (canElevMoveDown()) { elev.movedown(); }
    }

    const int currentFloor() { return elev.currentFloor(); }
    bool atFloor(int i) { return i == currentFloor(); }

    Elevator::ElevatorState getState() { return elev.getState(); }

    ~Lift() {
        stopThread = true;
        if (elevThread.joinable()) { elevThread.join(); }
    }

    const int occupants() {
        return elev.occupants();
    }

    void serveCurrentFloor() {
        int currentFloor = this->currentFloor();
        if (currentFloor == getEndDestination()) { endDestinationServiced(); }

        std::vector<Person> peopleGettingOff = elev.deloadPerson();
        for (Person& pers : peopleGettingOff) {
            liftEnvData.dropoffPers(currentFloor, pers);
        }

        while (liftEnvData.peopleAtFloor(currentFloor) > 0 && !elev.full())
        {
            Person p = liftEnvData.pickupPersIndiscriminately(currentFloor);
            activateFloorButton(p.desiredFloor);
            elev.addPerson(p);
        }

        deactivateFloorButton(currentFloor);
    }

    bool externalRequestAtFloor(int floor) {
        return liftEnvData.externalRequestAtFloor(floor);
    }

    bool internalRequestAtFloor(int floor) {
        return elev.internalRequestAtFloor(floor);
    }

    bool atCapacity() { return occupants() == elev.getCapacity(); }

    int getEndDestination() { return endDestination; }
    void endDestinationServiced() { updateEndDestination(-1); }
    void updateEndDestination(int newDestination) { endDestination = newDestination; }

    int setEndDestination(int destination) { return endDestination = destination; }

    int PollForFloorCalls() {
        /*
        return -1 if no calls
        prioritise internal over external
        */
        int internalPoll = pollInternalFloorCalls();
        if (internalPoll != -1) { return internalPoll; }

        int externalPoll = pollExternalFloorCalls();
        if (externalPoll != -1) { return externalPoll; }

        return -1;
    }

    const int getDefaultFloor() { return defaultFloor; }


};

void towardDestination(Lift& lift) {
    int currFloor = lift.currentFloor();
    if (currFloor < lift.getEndDestination()) {
        lift.moveUp();
    }
    else {
        lift.moveDown();
    }
}

void towardDefaultfloor(Lift& lift) {
    int currFloor = lift.currentFloor();
    if (currFloor < lift.getDefaultFloor()) {
        lift.moveUp();
    }
    else {
        lift.moveDown();
    }
}


void liftLogicLoop(Lift& lift) {
    while (!lift.stopThread) {
        // as the lift travels up each floor, check for updates on both the internal button panel, and the external requests at sed floor

        int currFloor = lift.currentFloor();
        if (lift.externalRequestAtFloor(currFloor) &&
            !lift.atCapacity() ||
            lift.internalRequestAtFloor(currFloor) ||
            currFloor == lift.getEndDestination()){
                lift.serveCurrentFloor();
        }

        if (lift.getEndDestination() == -1) {
            lift.updateEndDestination(lift.PollForFloorCalls());
        }

        if (lift.getEndDestination() == -1){
            towardDefaultfloor(lift);
        }
        else {
            towardDestination(lift);
        }
    }
}

#endif