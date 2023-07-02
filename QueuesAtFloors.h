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
	class ServeEmptyFloor : public std::exception {
		using std::exception::exception;
		// acceptable exception when "popping front" of empty floor queue
	};

	QueuesAtFloors() {}
	QueuesAtFloors(int floors) {
		std::lock_guard<std::mutex> lock(mu);
		queuesAtFloors.resize(floors);
	}

	void addPersonToFloor(Person p, int floor) {
		std::lock_guard<std::mutex> lock(mu);
		int floorIndex = floor - 1;
		queuesAtFloors[floorIndex].push_back(p);
	}

	const int peopleAtFloor(int floor) {
		std::lock_guard<std::mutex> lock(mu);
		int floorIndex = floor - 1;
		return queuesAtFloors[floorIndex].size();
	}

	Person removeFirstPersonIndiscriminately(int floor) {
		std::lock_guard<std::mutex> lock(mu);
		int floorIndex = floor - 1;
		if (queuesAtFloors[floorIndex].empty()) { throw ServeEmptyFloor("removeFirstPersonIndiscriminately called with noone at floor"); }
		Person pers = queuesAtFloors[floorIndex].front();
		queuesAtFloors[floorIndex].pop_front();
		return pers;
	}

	const int floorCount() {
		std::lock_guard<std::mutex> lock(mu);
		return queuesAtFloors.size();
	}

	const std::vector<bool> getExternalButtonPanel() {
		// assumes that queue at floor results in button pressed
		std::lock_guard<std::mutex> lock(mu);
		std::vector<bool> vect;
		for (auto queue : queuesAtFloors) { if (queue.size() > 0) { vect.push_back(true); } else { vect.push_back(false); } }
		return vect;
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