#ifndef LIFT_H_
#define LIFT_H_

#include "Elevator.h"
#include "Config.h"
#include "LiftEnvironmentalData.h"
#include <thread>

class Lift;
void liftLogic(Lift& lift);

class Lift {
private:
    int totalFloors;
    Elevator elev;
    std::thread elevThread;
    std::vector<bool> floorButtonPanel;
    LiftEnvironmentData liftEnvData;

    bool canElevMoveUp() { return currentFloor() + 1 < totalFloors; }
    bool canElevMoveDown() { return currentFloor() != 0; }

public:
    bool stopThread{ false };

    explicit Lift(int floors, QueuesAtFloors* queuesRef, QueuesAtFloors* outputRef) : totalFloors(floors), liftEnvData(queuesRef, outputRef)  {
        elevThread = std::thread(liftLogic, std::ref(*this));
        floorButtonPanel.resize(floors, false);
    }
    Lift(const Lift& li) { throw std::exception(); }

    void moveUp() { if (canElevMoveUp()) { elev.moveup(); } }
    void moveDown() { if (canElevMoveDown()) { elev.movedown(); } }

    const int currentFloor() { return elev.currentFloor(); }
    bool atFloor(int i) { return i == currentFloor(); }


    Elevator::ElevatorState getState() { return elev.getState(); }

    ~Lift() {
        stopThread = true;
        if (elevThread.joinable()) {elevThread.join();}
    }

    const int occupants() {
        return elev.occupants();
    }

    void serveCurrentFloor() {
        int currentFloor = this->currentFloor();

        std::vector<Person> peopleGettingOff = elev.deloadPerson();
        for (Person& pers : peopleGettingOff) { liftEnvData.dropoffPers(currentFloor, pers); }


        while (liftEnvData.peopleAtFloor(currentFloor) > 0 && !elev.full())
            {elev.addPerson(liftEnvData.pickupPersIndiscrimintately(currentFloor));}
    }

    bool externalRequestAtFloor(int floor) {
        return liftEnvData.externalRequestAtFloor(floor);
    }
};

void liftLogic(Lift& lift) {
    while (!lift.stopThread) {

        
        lift.serveCurrentFloor();

        int currFloor = lift.currentFloor();
        while(lift.externalRequestAtFloor(currFloor) == 0) {
            lift.moveUp();
            currFloor = lift.currentFloor();
        }
        
        

    }

}

#endif