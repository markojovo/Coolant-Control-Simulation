#include <gtest/gtest.h>
#include "shared_data.h"
#include <thread>
#include <chrono>

// Function declarations
void plantSimulation();
void mainController();

class CoolingSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset all variables before each test
        inverterPowerPercent.store(100.0);
        fanPower.store(100.0);
        pumpPowerPercent.store(0.0);
        inverterTemp.store(20.0);
        coolantFlowPercent.store(0.0);
        state.store(0);
        updateFlag.store(false);
        pidControlSignal.store(0.0);
        inverterControlSignal.store(100.0);
        coolantFlow.store(0.0);
    }

    void RunSimulation(float setpoint, std::chrono::seconds duration) {
        tempSetpoint.store(setpoint);
        std::thread plant_thread(plantSimulation);
        std::thread controller_thread(mainController);

        // Let the simulation run for the specified duration
        std::this_thread::sleep_for(duration);

        // Stop the threads 
        plant_thread.detach();
        controller_thread.detach();
    }
};

TEST_F(CoolingSystemTest, TemperatureStabilizesAt50) {
    RunSimulation(50.0f, std::chrono::seconds(20));

    // Check if the temperature is close to 50°C
    EXPECT_NEAR(inverterTemp.load(), 50.0, 2.0);
    // Check if the system is in normal running state
    EXPECT_EQ(state.load(), 0);
}

TEST_F(CoolingSystemTest, EmergencyStopAt150) {
    RunSimulation(150.0f, std::chrono::seconds(20));

    // Check if inverter power is zero
    EXPECT_NEAR(inverterPowerPercent.load(), 0.0, 0.1);
    // Check if the system entered emergency stop
    EXPECT_EQ(state.load(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}