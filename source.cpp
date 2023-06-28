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

int main() {
   
    simulations.reserve(1);
    {
        int floors = 10;
        int lifts = 4;
        simulations.emplace_back(floors, lifts); // MedicineChest, emplace_back is used as the threads never get joined, so the ~Building causes a freeze
    }
    
    std::thread tGUI(updateTerminal, std::ref(simulations[0]), std::ref(interruptGUI));
    std::thread tKeyboardinput(fToStop, interruptSimulation);

    for(int i = 0; i < 10; i++)
    {
        int personDesiredFloor = 10;
        int personCurrentFloor = 5;
        simulations[0].addPersonToFloor(Person{ personDesiredFloor }, personCurrentFloor);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5000) / SIMULATIONSPEEDFACTOR);
    for (int i = 0; i < 10; i++)
    {
        int personDesiredFloor = 5;
        int personCurrentFloor = 9;
        simulations[0].addPersonToFloor(Person{ personDesiredFloor }, personCurrentFloor);
    }

    tGUI.join();
    tKeyboardinput.join();

    std::cout << std::setw(15) << "Longest service time: " << std::setw(5) << simulations[0].longestServiceTime() << " seconds." << std::endl;
    std::cout << std::setw(15) << "Shortest service time: " << std::setw(5) << simulations[0].shortestServiceTime() << " seconds." << std::endl;
    std::cout << std::setw(15) << "Average service time: " << std::setw(5) << simulations[0].averageServiceTime() << " seconds." << std::endl;
    return 0;
}