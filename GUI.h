#ifndef GUI_H_
#define GUI_H_

#include <cstdlib>
#include <chrono>
#include "Building.h"
#include "Config.h"

void clearWindowsTerminal() {
    system("cls");
}

void printTerminalContent(Building& ref) {
    ref.getBuildingRepresentation();
}

void updateTerminal(Building& ref, bool& interrupt) {
    while (!interrupt) {
        clearWindowsTerminal();

        std::cout << "Type 'f' and Press [Enter] to see results.\n";
        printTerminalContent(ref);

        std::this_thread::sleep_for(UPDATEGUITIME);
    }
}

#endif