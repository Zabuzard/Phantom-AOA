#include "AOAIndicator.h"

#include <sstream>

#include "../../math/Math.h"

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
    // TODO backlight (white, red, which lamps, intensity)
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasPrimarySystemPower()) {
        // Needle is stuck in previous indication
        showOffFlag = true;
        return;
    }

    // Indicator limits
    double targetAoaDeg = math::clamp(*aoaDeg, 0, 30);

    simulateNeedleLag(targetAoaDeg, deltaTimeSeconds);
    showOffFlag = false;
}

void AOAIndicator::simulateNeedleLag(double aoaDeg, double deltaTimeSeconds) {
    double diffDeg = std::abs(aoaDeg - indicatedAOADeg);

    // Dampening to prevent jitter
    if (hasNeedleCaughtUp && diffDeg < 0.25) {
        return;
    }
    hasNeedleCaughtUp = false;

    double degPerSecond =
            diffDeg <= 10 ? SMALL_DIFF_NEEDLE_SPEED_DEG_PER_SECOND : GREAT_DIFF_NEEDLE_SPEED_DEG_PER_SECOND;
    double moveNeedleByDeg = degPerSecond * deltaTimeSeconds;

    indicatedAOADeg = math::moveTowards(indicatedAOADeg, moveNeedleByDeg, aoaDeg);

    if (indicatedAOADeg == aoaDeg) {
        hasNeedleCaughtUp = true;
    }
}
} // phantom
