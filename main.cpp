#include "shared_data.h"
#include <iostream>
#include <thread>

void plantSimulation();
void mainController();


int main() {
    float initialTempSetpoint;
    std::cout << "Enter the temperature setpoint: ";
    std::cin >> initialTempSetpoint;
    tempSetpoint.store(initialTempSetpoint);

    std::thread plantThread(plantSimulation);
    std::thread controllerThread(mainController);
    
    plantThread.join();
    controllerThread.join();
    
    return 0;
}