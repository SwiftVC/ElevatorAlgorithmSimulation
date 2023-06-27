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

};

#endif