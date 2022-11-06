#include "StallWarningVibrator.h"

#include "../../util/Ansi.h"

namespace phantom {
StallWarningVibrator::StallWarningVibrator(std::weak_ptr<const AOASensor> sensor,
                                           std::weak_ptr<const AOAPowerSystem> powerSystem,
                                           std::weak_ptr<const Engine> engine)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)}, engine{std::move(engine)} {}

void StallWarningVibrator::initialize() {}

std::string StallWarningVibrator::render() const {
    std::string description = vibrating ? (ansi::WHITE_BACKGROUND + ansi::BLACK + "BRRRRRRRRT" + ansi::COLOR_RESET)
                                        : "_";

    return "stall warning vibrator (" + description + ")";
}

void StallWarningVibrator::update(double deltaTimeSeconds) {
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg
        || !powerSystem.lock()->hasSecondarySystemPower()
        || hasWeightOnWheel()) {
        vibrating = false;
        return;
    }

    vibrating = aoaDeg >= 22.3;
}

bool StallWarningVibrator::isVibrating() const {
    return vibrating;
}

bool StallWarningVibrator::hasWeightOnWheel() {
    return engine.lock()->isFlagActive(Flag::WEIGHT_ON_WHEEL);
}
} // phantom
