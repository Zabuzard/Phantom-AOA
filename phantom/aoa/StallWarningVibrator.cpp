#include "StallWarningVibrator.h"

namespace phantom {
StallWarningVibrator::StallWarningVibrator(std::weak_ptr<const AOASensor> sensor) : sensor{std::move(sensor)} {}

void StallWarningVibrator::initialize() {}

std::string StallWarningVibrator::render() const {
    std::string description = vibrating ? "BRRRRRRRRT" : "_";

    return "stall warning vibrator (" + description + ")";
}

void StallWarningVibrator::update(double deltaTimeSeconds) {
    // TODO Does this get the data from the sensor or the laggy/dampened indicator?
    // TODO Power, circuit breakers, icing, weight on wheel, slats & gear up
    double aoaDeg = sensor.lock()->getAOADeg();

    vibrating = aoaDeg >= 22.3;
}

bool StallWarningVibrator::isVibrating() const {
    return vibrating;
}
} // phantom
