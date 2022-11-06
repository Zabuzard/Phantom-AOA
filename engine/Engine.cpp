#include "Engine.h"

#include <windows.h>
#include <bitset>
#include <map>
#include <iostream>
#include <random>

#include "Controls.h"
#include "Flag.h"

#include "../math/Math.h"

void Engine::initialize() {}

std::string Engine::render() const {
    std::map<Bus, std::string> busToName{
            {Bus::ESSENTIAL_DC,  "Ess DC"},
            {Bus::RIGHT_MAIN_AC, "R Main AC"},
            {Bus::INSTRUMENT_AC, "Instr AC"}
    };

    std::map<CircuitBreaker, std::string> circuitBreakerToName{
            {CircuitBreaker::AOA_PROBE_HTR_PWR,    "HTR PWR"},
            {CircuitBreaker::AOA_PROBE_HTR_CONT,   "HTR CONT"},
            {CircuitBreaker::ESS_DC_CADC,          "ESS CADC"},
            {CircuitBreaker::INSTR_AC_CADC,        "INSTR CADC"},
            {CircuitBreaker::RM_VAC_AC_CADC_PWR_1, "CADC PWR 1"},
            {CircuitBreaker::RM_VAC_AC_CADC_PWR_2, "CADC PWR 2"},
            {CircuitBreaker::RM_VAC_AC_CADC_PWR_3, "CADC PWR 3"}
    };

    std::map<Flag, std::string> flagToName{
            {Flag::NOSE_WHEEL_EXTENDED, "Nose wheel out"},
            {Flag::WEIGHT_ON_WHEEL,     "Weight on Wheel"},
            {Flag::GEAR_EXTENDED,       "Gear out"},
            {Flag::SLATS_IN,            "Slats in"}
    };

    std::stringstream ss;

    ss << "playerFlightPath: " << playerFlightPath.toString() << '\n';
    ss << "forward: " << playerRollAxis.toString()
       << ", left: " << playerPitchAxis.toString()
       << ", up: " << playerYawAxis.toString() << '\n';

    ss << "powered buses: [";
    for (const auto& bus: poweredBuses) {
        ss << busToName[bus] << ", ";
    }
    ss << "]\n";

    ss << "pulled circuit breakers: [";
    for (const auto& circuitBreaker: pulledCircuitBreakers) {
        ss << circuitBreakerToName[circuitBreaker] << ", ";
    }
    ss << "]\n";

    ss << "flags: [";
    for (const auto& flag: activeFlags) {
        ss << flagToName[flag] << ", ";
    }
    ss << "]\n";

    return ss.str();
}

Vector3 Engine::getPlayerFlightPath() const {
    return playerFlightPath;
}

Vector3 Engine::getPlayerRollAxis() const {
    return playerRollAxis;
}

Vector3 Engine::getPlayerPitchAxis() const {
    return playerPitchAxis;
}

Vector3 Engine::getPlayerYawAxis() const {
    return playerYawAxis;
}

bool Engine::isBusPowered(Bus bus) const {
    return poweredBuses.contains(bus);
}

bool Engine::isCircuitBreakerOut(CircuitBreaker circuitBreaker) const {
    return pulledCircuitBreakers.contains(circuitBreaker);
}

bool Engine::isFlagActive(Flag flag) const {
    return activeFlags.contains(flag);
}

void Engine::update(double deltaTimeSeconds) {
    updateAircraftOrientation(deltaTimeSeconds);
    // TODO Think about merging the maps into just the flags
    updateToggleInputs(pulledCircuitBreakers, keyToCircuitBreaker);
    updateToggleInputs(poweredBuses, keyToBus);
    updateToggleInputs(activeFlags, keyToFlag);
}

void Engine::updateAircraftOrientation(double deltaTimeSeconds) {
    double changePitchByDeg = 0;
    if (isKeyPressed(controls::PITCH_UP)) {
        changePitchByDeg = -CHANGE_PITCH_DEG_PER_SECOND;
    } else if (isKeyPressed(controls::PITCH_DOWN)) {
        changePitchByDeg = CHANGE_PITCH_DEG_PER_SECOND;
    }
    changePitchByDeg *= deltaTimeSeconds;
    double changePitchByRad = math::degToRad(changePitchByDeg);

    playerRollAxis = playerRollAxis.rotatedAroundYAxis(changePitchByRad);
    playerYawAxis = playerYawAxis.rotatedAroundYAxis(changePitchByRad);

    double changeYawByDeg = 0;
    if (isKeyPressed(controls::RUDDER_RIGHT)) {
        changeYawByDeg = CHANGE_YAW_DEG_PER_SECOND;
    } else if (isKeyPressed(controls::RUDDER_LEFT)) {
        changeYawByDeg = -CHANGE_YAW_DEG_PER_SECOND;
    }
    changeYawByDeg *= deltaTimeSeconds;
    double changeYawByRad = math::degToRad(changeYawByDeg);

    playerRollAxis = playerRollAxis.rotatedAroundZAxis(changeYawByRad);
    playerPitchAxis = playerPitchAxis.rotatedAroundZAxis(changeYawByRad);
}

bool Engine::isKeyPressed(int keyCode) {
    auto keyState = GetKeyState(keyCode);

    // High order bit indicates pressed
    constexpr auto bits = CHAR_BIT * sizeof(SHORT);
    return std::bitset<bits>(keyState)[bits - 1];
}

bool Engine::isToggleInputOnCooldown() {
    using namespace std::chrono_literals;

    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - lastToggleInput;

    return diff < 200ms;
}

double Engine::getOutsideTemperatureCelsius() const {
    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_real_distribution<double> dist(-30, 30);

    // FIXME Re-enable before release, was just disabled for testing convenience
    // return dist(rng);
    return 30;
}
