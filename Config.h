#ifndef CONFIG_H_
#define CONFIG_H_

#include <chrono>

float SIMULATIONSPEEDFACTOR = 5; // variability in results where SIMULATIONSPEEDFACTOR > 5

auto LIFTTIMEBETWEENFLOORS = std::chrono::milliseconds(2000) / SIMULATIONSPEEDFACTOR;
auto LIFTTIMECLOSING = std::chrono::milliseconds(2000) / SIMULATIONSPEEDFACTOR;
auto LIFTTIMEOPENING = std::chrono::milliseconds(1000) / SIMULATIONSPEEDFACTOR;
auto LIFTTIMECLOSEDBEFOREMOVE = std::chrono::milliseconds(200) / SIMULATIONSPEEDFACTOR;
auto LIFTTIMEACCEPTPERSON = std::chrono::milliseconds(1000) / SIMULATIONSPEEDFACTOR;

int GUI_UPDATES_PER_SECOND = 3;
auto UPDATEGUITIME = std::chrono::milliseconds(1000) / GUI_UPDATES_PER_SECOND;

#endif