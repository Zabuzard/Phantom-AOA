#include "AOAIndicator.h"

#include <sstream>

namespace phantom {
AOAIndicator::AOAIndicator(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)} {}

void AOAIndicator::initialize() {}

std::string AOAIndicator::render() const {
    std::stringstream ss;
    ss << "indicator (";

    if (showOffFlag) {
        ss << "OFF - ";
    }

    ss << "needle: " << indicatedAOADeg << " deg)";

    return ss.str();
}

void AOAIndicator::update(double deltaTimeSeconds) {
    // TODO backlight (white, red, which lamps, intensity), needle damping, needle slew rate (lag)
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasPrimarySystemPower()) {
        // Needle is stuck in previous indication
        showOffFlag = true;
        return;
    }

    indicatedAOADeg = *aoaDeg;
    showOffFlag = false;
}
} // phantom
