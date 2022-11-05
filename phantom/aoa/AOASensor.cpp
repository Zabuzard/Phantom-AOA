#include "AOASensor.h"

#include <sstream>
#include "../../math/Math.h"

AOASensor::AOASensor(std::weak_ptr<const Engine> engine) : engine(std::move(engine)) {}

void AOASensor::initialize() {}

double AOASensor::getAOADeg() const {
    return measuredAOADeg;
}

std::string AOASensor::render() const {
    std::stringstream ss;
    ss << "sensor (measured: " << measuredAOADeg << " deg, physical: " << physicalAOADeg << " deg)";
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
    measuredAOADeg = physicalAOADeg;
}
