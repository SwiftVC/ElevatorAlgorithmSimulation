#ifndef QUEUESATFLOORS_H_
#define QUEUESATFLOORS_H_

#include <mutex>
#include <vector>
#include <list>
#include "Person.h"

class QueuesAtFloors {
	std::mutex mu;
	std::vector<std::list<Person>> queuesAtFloors;

public:
	QueuesAtFloors() {}
	QueuesAtFloors(int floors) {
		std::lock_guard<std::mutex> lock(mu);
		queuesAtFloors.resize(floors);
	}

	void addPersonToFloor(Person p, int floor) {
		std::lock_guard<std::mutex> lock(mu);
		queuesAtFloors[floor - 1].push_back(p);
	}

	const int peopleAtFloor(int floor) {
		std::lock_guard<std::mutex> lock(mu);
		return queuesAtFloors[floor].size();
	}

	Person removeFirstPersonIndiscriminately(int floor) {
		std::lock_guard<std::mutex> lock(mu);
		Person pers = queuesAtFloors[floor].front();
		queuesAtFloors[floor].pop_front();
		return pers;
	}

	const int floorCount() {
		std::lock_guard<std::mutex> lock(mu);
		return queuesAtFloors.size();
	}

	const std::vector<float> serviceTimeSeconds() {
		/* Returns a vector<float> of seconds for each person served */
		std::vector<float> ret;

		std::lock_guard<std::mutex> lock(mu);
		for (int floorIndex = 0; floorIndex < queuesAtFloors.size(); floorIndex++) {
			for (std::list<Person>::iterator persIt = queuesAtFloors[floorIndex].begin(); persIt != queuesAtFloors[floorIndex].end(); persIt++) {
				Person p = *persIt;
				ret.push_back(p.secondsForElevatorTravel());
			}
		}
		return ret;
	}

};

#endif