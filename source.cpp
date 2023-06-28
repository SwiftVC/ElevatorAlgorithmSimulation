#include <iostream>
#include <thread>
#include <mutex>
#include "GUI.h"
#include "Input.h"

std::vector<Building> simulations;
bool interruptGUI{ false };

void interruptSimulation() {
    simulations[0].interruptSimulation();
    interruptGUI = true;
}

void addPersonAtBeginning(int currentFloor, int desiredFloor) {
    simulations[0].addPersonToFloor(Person{ desiredFloor }, currentFloor);
}

void addPersonAfterDelay(int milliseconds, int currentFloor, int desiredFloor) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds) / SIMULATIONSPEEDFACTOR);
        simulations[0].addPersonToFloor(Person{ desiredFloor }, currentFloor);
}

int main() {
    /* CONFIG: floorCount & Liftcount */
    simulations.reserve(1);
    {
        int floors = 10;
        int lifts = 1;
        simulations.emplace_back(floors, lifts); // MedicineChest, emplace_back is used as the threads never get joined, so the ~Building causes a freeze
    }

    /* Boilerplate 1/2 */
    std::thread tGUI(updateTerminal, std::ref(simulations[0]), std::ref(interruptGUI));
    std::thread tKeyboardinput(fToStop, interruptSimulation);

    /* CONFIG: Add People at Simulation Start */
    for(int i = 0; i < 10; i++)
    {
        int personDesiredFloor = 10;
        int personCurrentFloor = 5;
        addPersonAtBeginning(personCurrentFloor, personDesiredFloor);
    }

    /* CONFIG: Add People after delay. (delays accumulate) */
    for (int i = 1; i < 11; i++)
    {
        int personDesiredFloor = 2;
        int personCurrentFloor = i;
        addPersonAfterDelay(1000, personCurrentFloor, personDesiredFloor);
    }

    /* Boilerplate 2/2 */
    tGUI.join();
    tKeyboardinput.join();

    std::cout << std::setw(15) << "Longest service time: " << std::setw(5) << simulations[0].longestServiceTime() << " seconds." << std::endl;
    std::cout << std::setw(15) << "Shortest service time: " << std::setw(5) << simulations[0].shortestServiceTime() << " seconds." << std::endl;
    std::cout << std::setw(15) << "Average service time: " << std::setw(5) << simulations[0].averageServiceTime() << " seconds." << std::endl;
    return 0;
}