#include "shared_data.h"

// Define global variables
std::atomic<float> inverterPowerPercent{100.0};
std::atomic<float> fanPower{100.0};
std::atomic<float> pumpPowerPercent{0.0};
std::atomic<float> inverterTemp{20.0};
std::atomic<float> coolantFlowPercent{0.0};
std::atomic<int> state{0};
std::atomic<float> tempSetpoint;
std::atomic<bool> updateFlag{false};
std::atomic<float> pidControlSignal{0.0};
std::atomic<float> inverterControlSignal{100.0};
std::atomic<float> coolantFlow{0.0};

// Define constants
const float maxInverterTemp = 80.0;
const float emergencyInverterTemp = 120.0;
const float maxInverterPower = 1000.0;
const float ambientTemp = 20.0;
const float thermalResistance = 5.0;
const float thermalCapacitance = 50.0;
const float coolantInductance = 10.0;
const float maxCoolantFlow = 50.0;
const float coolingEfficiency = 1.5;