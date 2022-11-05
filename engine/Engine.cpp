#include "Engine.h"

#include <windows.h>
#include <bitset>
#include <map>
#include <iostream>
#include <random>

#include "Controls.h"
#include "Flag.h"

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
            {Flag::WEIGHT_ON_WHEEL, "Weight on Wheel"}
    };

    std::stringstream ss;

    ss << "playerFlightPath: " << playerFlightPath.toString() << '\n';

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

Vector3 Engine::getPlayerChordLine() const {
    return playerChordLine;
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
    // TODO If theres time, change this to actually change by degrees of the angle
    // NOTE Technically this does not actually change the pitch and rudder (as in, degrees or similar),
    //  just the position in the respective axis. But since this is only needed for testing, no need to get fancy here.
    double changePitchBy = 0;
    if (isKeyPressed(controls::PITCH_UP)) {
        changePitchBy = CHANGE_PITCH_PER_SECOND;
    } else if (isKeyPressed(controls::PITCH_DOWN)) {
        changePitchBy = -CHANGE_PITCH_PER_SECOND;
    }
    changePitchBy *= deltaTimeSeconds;

    playerFlightPath.z += changePitchBy;

    double changeRudderBy = 0;
    if (isKeyPressed(controls::RUDDER_RIGHT)) {
        changeRudderBy = CHANGE_RUDDER_PER_SECOND;
    } else if (isKeyPressed(controls::RUDDER_LEFT)) {
        changeRudderBy = -CHANGE_RUDDER_PER_SECOND;
    }
    changeRudderBy *= deltaTimeSeconds;

    playerFlightPath.y += changeRudderBy;
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
