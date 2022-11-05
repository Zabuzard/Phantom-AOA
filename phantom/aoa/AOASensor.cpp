#include "AOASensor.h"

#include <sstream>
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

    if (!measuredAOADeg) {
        ss << "no power";
    } else {
        ss << "measured: " << *measuredAOADeg << " deg";
    }

    ss << ", physical: " << physicalAOADeg << " deg)";

    return ss.str();
}

void AOASensor::update(double deltaTimeSeconds) {
    updatePhysicalAOA();
    simulateSensorReading();
}

void AOASensor::updatePhysicalAOA() {
    const Vector3 chordLine = engine.lock()->getPlayerChordLine();
    const Vector3 flightPath = engine.lock()->getPlayerFlightPath();

    double angleDeg = math::radToDeg(flightPath.angleRad(chordLine));

    physicalAOADeg = angleDeg;
}

void AOASensor::simulateSensorReading() {
    // TODO nose wheel changes, side slipping error, frozen error, warmup, ...
    if (!powerSystem.lock()->hasPrimarySystemPower()) {
        measuredAOADeg = std::nullopt;
        return;
    }

    measuredAOADeg = physicalAOADeg;
}
} // phantom
