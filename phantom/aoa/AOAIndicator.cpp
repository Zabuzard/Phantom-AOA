#include "AOAIndicator.h"

#include <sstream>

namespace phantom {
AOAIndicator::AOAIndicator(std::weak_ptr<const AOASensor> sensor) : sensor{std::move(sensor)} {}

void AOAIndicator::initialize() {}

std::string AOAIndicator::render() const {
    std::stringstream ss;
    ss << "indicator (indicated: " << indicatedAOADeg << " deg)";
    return ss.str();
}

void AOAIndicator::update(double deltaTimeSeconds) {
    // TODO Power, circuit breakers, OFF flag, backlight (white, red, which lamps, intensity), needle damping, needle slew rate (lag)
    indicatedAOADeg = sensor.lock()->getAOADeg();
}
} // phantom
