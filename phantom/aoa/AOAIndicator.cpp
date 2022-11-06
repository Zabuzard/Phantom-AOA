#include "AOAIndicator.h"

#include <sstream>

#include "../../math/Math.h"

namespace phantom {
AOAIndicator::AOAIndicator(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem,
                           std::weak_ptr<const Engine> engine)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)}, engine{std::move(engine)} {}

void AOAIndicator::initialize() {}

std::string AOAIndicator::render() const {
    std::stringstream ss;
    ss << "indicator (";

    if (showOffFlag) {
        ss << "OFF - ";
    }

    ss << "needle: " << indicatedAOADeg << " deg";

    ss << ", lamp: ";
    if (illuminatedLamp) {
        ss << "[" << lampTypeToName.at(illuminatedLamp->type) << ", "
           << illuminatedLamp->intensity << ", "
           << lampColorToName.at(illuminatedLamp->color) << "]";
    } else {
        ss << "off";
    }

    ss << ")";
    return ss.str();
}

void AOAIndicator::update(double deltaTimeSeconds) {
    updateIndication(deltaTimeSeconds);
    updateLamp();
}

void AOAIndicator::updateIndication(double deltaTimeSeconds) {
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || powerSystem.lock()->hasPrimarySystemPower()) {
        // Needle is stuck in previous indication
        showOffFlag = true;
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

void AOAIndicator::updateLamp() {
    indicator::LampType type;
    // TODO These ranges are adjustable, figure out the commonly used settings
    if (indicatedAOADeg < 10) {
        type = indicator::LampType::LOW;
    } else if (indicatedAOADeg < 20) {
        type = indicator::LampType::MEDIUM;
    } else {
        type = indicator::LampType::HIGH;
    }

    // NOTE Technically this is a 3-way switch, not a rotary
    double emergencyFloodlightValue = engine.lock()->getKnobValue(Knob::EMERGENCY_FLOODLIGHT_INTENSITY);
    if (emergencyFloodlightValue < 0.3) {
        // TODO Probably also has some circuit breakers
        if (!engine.lock()->isBusPowered(Bus::RIGHT_MAIN_AC)) {
            illuminatedLamp = std::nullopt;
            return;
        }

        // Use white light
        double intensity = engine.lock()->getKnobValue(Knob::INSTR_PANEL_LIGHT_INTENSITY);
        illuminatedLamp = {type, intensity, indicator::LampColor::WHITE};
        return;
    }

    indicator::LampColor color = indicator::LampColor::RED;
    if (emergencyFloodlightValue < 0.6) {
        // Use dimmed red light
        // TODO Probably also has some circuit breakers
        if (!engine.lock()->isBusPowered(Bus::LEFT_MAIN_AC)) {
            illuminatedLamp = std::nullopt;
            return;
        }

        illuminatedLamp = {type, 0.5, color};
        return;
    }

    // Use bright red light
    // TODO Probably also has some circuit breakers
    if (!engine.lock()->isBusPowered(Bus::ESSENTIAL_DC)) {
        illuminatedLamp = std::nullopt;
        return;
    }
    illuminatedLamp = {type, 1, color};
}

} // phantom
