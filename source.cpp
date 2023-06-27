#include <iostream>
#include <thread>
#include <mutex>
#include "GUI.h"


int main() {
    std::vector<Building> simulations;

    simulations.reserve(1);
    {
        int floors = 10;
        int lifts = 1;
        simulations.emplace_back(floors, lifts); // MedicineChest, emplace_back is used as the threads never get joined, so the ~Building causes a freeze
    }
   
    for(int i = 0; i < 10; i++)
    {
        int personDesiredFloor = 0;
        int personCurrentFloor = 5;
        simulations[0].addPersonToFloor(Person{ personDesiredFloor }, personCurrentFloor);
    }

    std::thread t_GUI(updateTerminal, std::ref(simulations[0]));

    t_GUI.join();

    return 0;
}