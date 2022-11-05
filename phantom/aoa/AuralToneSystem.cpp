#include "AuralToneSystem.h"

#include <sstream>

namespace phantom {
AuralToneSystem::AuralToneSystem(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)} {}

void AuralToneSystem::initialize() {}

std::string AuralToneSystem::render() const {
    if (!tone) {
        return "aural tone system (_)";
    }

    std::stringstream ss;
    ss << "aural tone system (BEEP[freq="
       << tone->frequency << ", vol=" << tone->volume
       << "])";

    return ss.str();
}

void AuralToneSystem::update(double deltaTimeSeconds) {
    // TODO icing, volume knob, actual tone profile (also for slats/gear up/down)
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasSecondarySystemPower()) {
        tone = std::nullopt;
        return;
    }

    tone = aoaDeg > 20.3 ? std::make_optional<Tone>({1, 1}) : std::nullopt;
}

std::optional<Tone> AuralToneSystem::getTone() const {
    return tone;
}
} // phantom
