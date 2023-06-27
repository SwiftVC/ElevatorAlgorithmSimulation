#ifndef BUILDING_H_
#define BUILDING_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include "Lift.h"
#include <mutex>

class Building {
private:
	int floors;
	QueuesAtFloors queuesAtFloors;
	QueuesAtFloors output;
	std::vector<Lift> lifts; // must be destructed before queuesAtFloors

public:
	Building(const Building& b) : queuesAtFloors(b.floors), floors(b.floors) {}

	Building(int totalFloors, int totalLifts) : floors(totalFloors), queuesAtFloors(totalFloors), output(totalFloors) {
		lifts.reserve(floors);
		for (int i = 0; i < totalLifts; ++i) {lifts.emplace_back(floors, &queuesAtFloors, &output);}
	}

	void addPersonToFloor(Person p, int floor) {
		queuesAtFloors.addPersonToFloor(p, floor);
	}

	void getBuildingRepresentation() {
		for (int floorIndex = floors - 1; floorIndex > -1; floorIndex--) {
			std::cout << "Floor: " << std::setw(4) << floorIndex + 1 << "|";

			// for each floor output queue remaining (Assumption: lifts are next to oneanother and service same queue)
			std::cout << "Queue: ";
			std::cout << std::setw(3) << queuesAtFloors.peopleAtFloor(floorIndex) << " | ";

			// for each floor output lift status OR placeholder image if lift elsewhere
			for (int liftIndex = 0; liftIndex < lifts.size(); liftIndex++) {
				std::cout << std::setw(Elevator::ELEVATORSTATEMAXFIELDWIDTH);
				if (lifts[liftIndex].atFloor(floorIndex) != true){std::cout << ""<< " : " << std::setw(6) << " | ";}
				else { std::cout << Elevator::elevatorStateToString(lifts[liftIndex].getState()) << " : " << std::setw(3) << lifts[liftIndex].occupants() << " | ";}
			}

			std::cout << "Output: ";
			std::cout << std::setw(3) << output.peopleAtFloor(floorIndex) << " | ";
			std::cout << "\n";
		}
	}

	void interruptSimulation() {
		lifts.clear();
	}
};

#endif