#include "StallWarningVibrator.h"

namespace phantom {
StallWarningVibrator::StallWarningVibrator(std::weak_ptr<const AOASensor> sensor,
                                           std::weak_ptr<const AOAPowerSystem> powerSystem)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)} {}

void StallWarningVibrator::initialize() {}

std::string StallWarningVibrator::render() const {
    std::string description = vibrating ? "BRRRRRRRRT" : "_";

    return "stall warning vibrator (" + description + ")";
}

void StallWarningVibrator::update(double deltaTimeSeconds) {
    // TODO icing, weight on wheel, slats & gear up
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasSecondarySystemPower()) {
        vibrating = false;
        return;
    }

    vibrating = aoaDeg >= 22.3;
}

bool StallWarningVibrator::isVibrating() const {
    return vibrating;
}
} // phantom
