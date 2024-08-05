#include "shared_data.h"
#include <chrono>
#include <thread>
#include <iomanip>

void plantSimulation() {
    auto lastTime = std::chrono::steady_clock::now();
    auto lastUpdateTime = lastTime;
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        float currentTemp = inverterTemp.load();
        float currentFlow = coolantFlow.load();

        // Update coolant flow (inductor-like behavior)
        float flowChange = (pumpPowerPercent.load() - currentFlow) * dt / coolantInductance;
        coolantFlow.store(std::max(0.0f, std::min(currentFlow + flowChange, 100.0f)));
        coolantFlowPercent.store(coolantFlow.load());

        // Update inverter power based on control signal
        inverterPowerPercent.store(inverterControlSignal.load());

        // Calculate heat generation and dissipation
        float heatGeneration = inverterPowerPercent.load() * maxInverterPower / 100.0;
        float cooling = coolingEfficiency * coolantFlowPercent.load() * maxCoolantFlow / 100.0 * (currentTemp - ambientTemp);
        float heatDissipation = (currentTemp - ambientTemp) / thermalResistance + cooling;
        float tempChange = (heatGeneration - heatDissipation) * dt / thermalCapacitance;
        inverterTemp.store(currentTemp + tempChange);

        if (std::chrono::duration<float>(currentTime - lastUpdateTime).count() >= 0.1f) {
            updateFlag.store(true);
            lastUpdateTime = currentTime;
        }

        // "IRQ" to update motor control value
        pumpPowerPercent.store(pidControlSignal.load());

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}