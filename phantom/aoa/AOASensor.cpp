#include "AOASensor.h"

#include <sstream>
#include <cmath>
#include <random>

#include "../../math/Math.h"

namespace phantom {
AOASensor::AOASensor(std::weak_ptr<const Engine> engine, std::weak_ptr<const AOAPowerSystem> powerSystem)
        : engine{std::move(engine)}, powerSystem{std::move(powerSystem)} {}

void AOASensor::initialize() {}

std::optional<double> AOASensor::getAOADeg() const {
    return measuredAOADeg;
}

std::string AOASensor::render() const {
    std::stringstream ss;
    ss << "sensor (";

    if (!isTurnedOn) {
        ss << "no power";
    } else if (warmedUpAt) {
        auto duration = *warmedUpAt - std::chrono::high_resolution_clock::now();
        auto secondsUntilWarmedUp = std::chrono::duration_cast<std::chrono::seconds>(duration);

        ss << "warming up... " << secondsUntilWarmedUp.count() << "s";
    } else {
        ss << "measured: " << *measuredAOADeg << " deg";
    }

    ss << ", physical: " << physicalAOADeg << " deg"
       << ", [side-slip: " << physicalSideSlipDeg << " deg])";

    return ss.str();
}

void AOASensor::update(double deltaTimeSeconds) {
    updatePhysicalAOA();
    updatePhysicalSideSlip();
    simulateSensorReading();
}

void AOASensor::updatePhysicalAOA() {
    const Vector3 flightPath = engine.lock()->getPlayerFlightPath();

    const Vector3 rollAxis = engine.lock()->getPlayerRollAxis();
    const Vector3 yawAxis = engine.lock()->getPlayerYawAxis();

    const Vector3 flightPathOnPitchPlane = flightPath.projectedOntoPlane(rollAxis, yawAxis);

    physicalAOADeg = math::radToDeg(
            flightPathOnPitchPlane.signedAngleRad(rollAxis, engine.lock()->getPlayerPitchAxis()));
}

void AOASensor::updatePhysicalSideSlip() {
    const Vector3 flightPath = engine.lock()->getPlayerFlightPath();

    const Vector3 rollAxis = engine.lock()->getPlayerRollAxis();
    const Vector3 pitchAxis = engine.lock()->getPlayerPitchAxis();

    const Vector3 flightPathOnYawPlane = flightPath.projectedOntoPlane(rollAxis, pitchAxis);

    physicalSideSlipDeg = math::radToDeg(
            flightPathOnYawPlane.signedAngleRad(rollAxis, engine.lock()->getPlayerYawAxis()));
}

void AOASensor::simulateSensorReading() {
    // TODO spin/stall error
    bool wasTurnedOff = !isTurnedOn;

    if (!powerSystem.lock()->hasPrimarySystemPower()) {
        isTurnedOn = false;
        measuredAOADeg = std::nullopt;
        return;
    }
    isTurnedOn = true;

    if (wasTurnedOff) {
        startWarmup();
    }
    if (warmedUpAt) {
        auto now = std::chrono::high_resolution_clock::now();
        if (now < warmedUpAt) {
            return;
        }

        warmedUpAt = std::nullopt;
    }

    measuredAOADeg = physicalAOADeg;

    simulateNoseWheelError();
    simulateSideSlipError();
    simulateFrozenSensor();

    // Patch computation edge cases
    if (measuredAOADeg) {
        measuredAOADeg = std::make_optional(patchNaN(*measuredAOADeg));
    }
    physicalAOADeg = patchNaN(physicalAOADeg);
    physicalSideSlipDeg = patchNaN(physicalSideSlipDeg);
}

void AOASensor::startWarmup() {
    // TODO Technically a full warmup would not be needed if just
    //  switching off and on, it should have some sort of cooldown
    using namespace std::chrono_literals;

    double temperature = engine.lock()->getOutsideTemperatureCelsius();
    double relevantTemperature = math::clamp(temperature, -54, 30);

    // MIL-I-18856B(AS): 8
    double warmUpDurationSeconds = math::rangeLerp(relevantTemperature, -54, 30, 120, 2);
    warmedUpAt = std::chrono::high_resolution_clock::now() +
                 std::chrono::seconds(static_cast<int32_t>(warmUpDurationSeconds));
}

void AOASensor::simulateNoseWheelError() {
    if (!engine.lock()->isFlagActive(Flag::NOSE_WHEEL_EXTENDED)) {
        // TO 1F-4E-1: 1-30
        measuredAOADeg = *measuredAOADeg - 1;
    }
}

void AOASensor::simulateSideSlipError() {
    // TODO The manual does not specify actual angles, only says "full rudder at low speed",
    //  30° sounds reasonable, needs further testing
    double relevantSideSlipDeg = math::clamp(physicalSideSlipDeg, -30, 30);
    // TO 1F-4E-1: 6-6
    double aoaDegError = math::rangeLerp(relevantSideSlipDeg, -30, 30, 2, -2);

    measuredAOADeg = *measuredAOADeg + aoaDegError;
}

void AOASensor::simulateFrozenSensor() {
    // TODO Should actually simulate icing based on heater and ambient temperature
    if (!engine.lock()->isFlagActive(Flag::FROZEN_AOA_SENSOR)) {
        return;
    }

    std::random_device device;
    std::mt19937 rng(device());

    // Jitters around in the high angles, see TO 1F-4E-1: 1-30
    std::uniform_real_distribution<double> dist(20, 50);

    measuredAOADeg = dist(rng);
}

double AOASensor::patchNaN(double value) {
    // TODO This method should not be needed, check the math to find why it is sometimes NaN around 0
    return std::isnan(value) ? 0 : value;
}
} // phantom
