# ElevatorAlgorithmSimulation
Simulates transfer of variable floors and variable elevators in a building:
-  non-directional elevator-call buttons
-  internal elevator button panels with all floors
  
In its current configuration, each elevator operates on its own thread.
When the simulation has finished, enter 'f' to get metrics:
- Longest service time
- Shortest service time
- Average service time

Simulation speed may be increased but starts to return inconsistent results at Config.h SIMULATIONSPEEDFACTOR > 5 (CPU: i5-12600K).
This is due to elevator threads waiting on shared resources and the calculations for transfer times based on system clock.

Configure:
  + source.cpp/int floors
  + source.cpp/int lifts
  + source.cpp/CONFIG: Add People at Simulation Start
  + source.cpp/CONFIG: Add People after delay
  
