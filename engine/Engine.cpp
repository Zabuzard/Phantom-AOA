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
            {Bus::INSTRUMENT_AC, "Instr AC"},
            {Bus::LEFT_MAIN_AC,  "L Main AC"}
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
            {Flag::SLATS_IN,            "Slats in"},
            {Flag::AGM_45_SELECTED,     "AGM-45 selected"},
            {Flag::FROZEN_AOA_SENSOR,   "AOA sensor frozen"}
    };

    std::map<Knob, std::string> knobToName{
            {Knob::INSTR_PANEL_LIGHT_INTENSITY,    "INSTR Light"},
            {Knob::EMERGENCY_FLOODLIGHT_INTENSITY, "EMR Flood"},
            {Knob::INDEXER_LIGHT_INTENSITY,        "INDX Light"},
            {Knob::AURAL_TONE_VOLUME,              "Aural Vol"}
    };

    std::stringstream ss;
    ss << "## Engine\n";

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

    ss << "knob values: [";
    for (const auto& [knob, value]: knobToValue) {
        ss << knobToName[knob] << "=" << value << ", ";
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
    updateToggleInputs(pulledCircuitBreakers, keyToCircuitBreaker);
    updateToggleInputs(poweredBuses, keyToBus);
    updateToggleInputs(activeFlags, keyToFlag);
    updateKnobValues(deltaTimeSeconds);
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
    // TODO The only reason why the simulation only works on Windows right now,
    //  try to get a platform-independent solution.
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

    return dist(rng);
}

double Engine::getKnobValue(Knob knob) const {
    const auto it = knobToValue.find(knob);
    return it == knobToValue.cend() ? 0 : it->second;
}

void Engine::updateKnobValues(double deltaTimeSeconds) {
    double changeValueBy = CHANGE_KNOB_VALUE_PER_SECOND * deltaTimeSeconds;

    for (const auto& [key, knob]: keyToKnobDecrease) {
        if (!isKeyPressed(key)) {
            continue;
        }

        double value = getKnobValue(knob);
        value = math::clamp(value - changeValueBy, 0, 1);
        knobToValue[knob] = value;
    }

    for (const auto& [key, knob]: keyToKnobIncrease) {
        if (!isKeyPressed(key)) {
            continue;
        }

        double value = getKnobValue(knob);
        value = math::clamp(value + changeValueBy, 0, 1);
        knobToValue[knob] = value;
    }
}

Agm45Cue Engine::getAgm45Cue() const {
    std::vector<Agm45Cue> allCues{Agm45Cue::DIVE, Agm45Cue::LEVEL, Agm45Cue::PULL_UP};

    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_int_distribution<size_t> dist(0, allCues.size() - 1);

    return allCues[dist(rng)];
}
