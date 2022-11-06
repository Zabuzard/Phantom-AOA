cmake_minimum_required(VERSION 3.23)
project(Phantom_AOA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Phantom_AOA main.cpp engine/Simulation.cpp engine/Simulation.h engine/Entity.h engine/Engine.cpp engine/Engine.h math/Vector3.h phantom/aoa/AOASensor.cpp phantom/aoa/AOASensor.h math/Math.h phantom/aoa/AOAIndicator.cpp phantom/aoa/AOAIndicator.h phantom/aoa/AOAIndexer.cpp phantom/aoa/AOAIndexer.h phantom/aoa/StallWarningVibrator.cpp phantom/aoa/StallWarningVibrator.h phantom/aoa/AuralToneSystem.cpp phantom/aoa/AuralToneSystem.h engine/CircuitBreaker.h engine/Bus.h engine/Controls.h phantom/aoa/AOAPowerSystem.h phantom/aoa/AOAPowerSystem.cpp phantom/Phantom.cpp phantom/Phantom.h engine/Flag.h engine/Knob.h)
