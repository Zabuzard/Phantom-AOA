#include "AuralToneSystem.h"

#include <sstream>

namespace phantom {
AuralToneSystem::AuralToneSystem(std::weak_ptr<const AOASensor> sensor) : sensor{std::move(sensor)} {}

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
    // TODO Does this get the data from the sensor or the laggy/dampened indicator?
    // TODO Power, circuit breakers, icing, volume knob, actual tone profile (also for slats/gear up/down)
    double aoaDeg = sensor.lock()->getAOADeg();

    tone = aoaDeg > 20.3 ? std::make_optional<Tone>({1, 1}) : std::nullopt;
}

std::optional<Tone> AuralToneSystem::getTone() const {
    return tone;
}
} // phantom
