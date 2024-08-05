#pragma once
#include <atomic>

extern std::atomic<float> inverterPowerPercent;
extern std::atomic<float> fanPower;
extern std::atomic<float> pumpPowerPercent;
extern std::atomic<float> inverterTemp;
extern std::atomic<float> coolantFlowPercent;
extern std::atomic<int> state;
extern std::atomic<float> tempSetpoint;
extern std::atomic<bool> updateFlag;
extern std::atomic<float> pidControlSignal;
extern std::atomic<float> inverterControlSignal;
extern std::atomic<float> coolantFlow;

// Constants
extern const float maxInverterTemp;
extern const float emergencyInverterTemp;
extern const float maxInverterPower;
extern const float ambientTemp;
extern const float thermalResistance;
extern const float thermalCapacitance;
extern const float coolantInductance;
extern const float maxCoolantFlow;
extern const float coolingEfficiency;