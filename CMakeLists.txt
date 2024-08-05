cmake_minimum_required(VERSION 3.10)
project(CoolingSystemSimulation)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
find_package(GTest REQUIRED)

set(SOURCES
    main.cpp
    plant_simulation.cpp
    main_controller.cpp
    shared_data.cpp
)

add_executable(run ${SOURCES})
set(TEST_SOURCES
    cooling_system_test.cpp
    plant_simulation.cpp
    main_controller.cpp
    shared_data.cpp
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests GTest::GTest GTest::Main)

if(UNIX)
    target_link_libraries(run_tests pthread)
endif()