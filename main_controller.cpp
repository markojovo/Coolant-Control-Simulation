#include "shared_data.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

void mainController() {
    float believedInverterPower = 100.0;
    float believedPumpPower = 0.0;
    float believedCoolantFlow = 0.0;
    float believedInverterTemp = 20.0;
    int believedState = 0;

    float integral = 0.0;
    float prevError = 0.0;

    // PID controller parameters
    const float K = 2.5;
    float kp = K * 1.0;
    float ki = K * 0.1;
    float kd = K * 0.01;

    auto lastPrintTime = std::chrono::steady_clock::now();
    auto lastControlTime = std::chrono::steady_clock::now();

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();

        // Check if update flag is set (This is our stand-in for receiving a CANBUS message from the cooler peripheral and having an IRQ load the data from the packet)
        if (updateFlag.load()) {
            believedInverterPower = inverterPowerPercent.load();
            believedPumpPower = pumpPowerPercent.load();
            believedCoolantFlow = coolantFlowPercent.load();
            believedInverterTemp = inverterTemp.load();
            believedState = state.load();
            updateFlag.store(false);
        }

        // State machine and PID control (every 50ms)
        if (std::chrono::duration<float>(currentTime - lastControlTime).count() >= 0.05f) { // 50ms
            float dt = std::chrono::duration<float>(currentTime - lastControlTime).count();
            lastControlTime = currentTime;

            // State machine
            switch (believedState) {
                case 0:  // Normal running
                    if (believedInverterTemp > maxInverterTemp) {
                        believedState = 1;
                    }
                    inverterControlSignal.store(100.0f);
                    break;
                case 1:  // Inverter derating
                    if (believedInverterTemp <= maxInverterTemp) {
                        believedState = 0;
                    } else if (believedInverterTemp > emergencyInverterTemp) {
                        believedState = 2;
                        tempSetpoint.store(25.0);
                        inverterControlSignal.store(0.0);
                    } else {
                        float powerReduction = (believedInverterTemp - maxInverterTemp) / (emergencyInverterTemp - maxInverterTemp) * 100.0 * 0.75;
                        inverterControlSignal.store(std::max(0.0f, 100.0f - powerReduction));
                    }
                    break;
                case 2:  // Emergency stop
                    inverterControlSignal.store(0.0);
                    break;
            }

            // PID controller
            float error = believedInverterTemp - tempSetpoint.load();
            integral += error * dt;
            float derivative = (error - prevError) / dt;
            prevError = error;
            float pidOutput = kp * error + ki * integral + kd * derivative;
            pidControlSignal.store(std::max(0.0f, std::min(pidOutput, 100.0f)));

            // Update state
            state.store(believedState);
        }

        // Print believed values (every 500ms)
        if (std::chrono::duration<float>(currentTime - lastPrintTime).count() >= 0.5f) {
            lastPrintTime = currentTime;
            std::cout << std::fixed << std::setprecision(0)
                      << "Set Temperature: " << tempSetpoint.load() << "°C, "
                      << "State: "<< believedState << ", "
                      << "Inverter Power: " << std::setprecision(2) << believedInverterPower << "%, "
                      << "Pump Power: " << believedPumpPower << "%, "
                      << "Coolant Flow Rate: " << believedCoolantFlow << "%, "
                      << "Inverter Temperature: " << believedInverterTemp << "°C\n";
        }

    }
}