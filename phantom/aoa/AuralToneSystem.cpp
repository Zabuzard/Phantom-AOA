#include "AuralToneSystem.h"

#include <sstream>

#include "../../math/Math.h"
#include "../../util/Ansi.h"

namespace phantom {
AuralToneSystem::AuralToneSystem(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem,
                                 std::weak_ptr<const Engine> engine)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)}, engine{std::move(engine)} {}

void AuralToneSystem::initialize() {}

std::string AuralToneSystem::render() const {
    std::stringstream ss;
    ss << "aural tone system (";

    if (tones.empty()) {
        ss << "_)";
        return ss.str();
    }

    for (const auto& tone: tones) {
        std::string noise = ansi::WHITE;
        if (tone.frequency < 600) {
            noise += ansi::CYAN_BACKGROUND + "mmmm";
        } else if (tone.frequency < 1200) {
            noise += ansi::YELLOW_BACKGROUND + "beep";
        } else {
            noise += ansi::RED_BACKGROUND + "BEEP";
        }
        noise += ansi::COLOR_RESET;

        ss << noise << "[";

        if (tone.pulsePerSecond) {
            ss << "pps=" << *tone.pulsePerSecond;
        } else {
            ss << "steady";
        }

        ss << ",vol=" << tone.volume << "], ";
    }

    ss << ")";
    return ss.str();
}

void AuralToneSystem::update(double deltaTimeSeconds) {
    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasSecondarySystemPower()) {
        tones.clear();
        return;
    }

    bool useLandingProfile =
            engine.lock()->isFlagActive(Flag::SLATS_IN) || engine.lock()->isFlagActive(Flag::GEAR_EXTENDED);

    double volume = getVolume();
    tones = useLandingProfile ? getTonesForLandingProfile(*aoaDeg, volume)
                              : getTonesForFlightProfile(*aoaDeg, volume);
}

std::vector<Tone> AuralToneSystem::getTones() const {
    return tones;
}

std::vector<Tone> AuralToneSystem::getTonesForLandingProfile(double aoaDeg, double volume) {
    if (aoaDeg < 15) {
        return {};
    }

    std::vector<Tone> tones;
    if (15 <= aoaDeg && aoaDeg <= 18.7) {
        double pulsePerSecond = math::rangeLerp(aoaDeg, 15, 18.7, 1.5, 8.2);
        tones.push_back({400, pulsePerSecond, volume});
    }
    if (18.1 <= aoaDeg && aoaDeg <= 20.3) {
        tones.push_back({900, std::nullopt, volume});
    }
    if (19.7 <= aoaDeg && aoaDeg <= 22.3) {
        double pulsePerSecond = math::rangeLerp(aoaDeg, 19.7, 22.3, 1.5, 6.2);
        tones.push_back({1600, pulsePerSecond, volume});
    }
    if (aoaDeg > 22.3) {
        tones.push_back({1600, 20, volume});
    }

    return tones;
}

std::vector<Tone> AuralToneSystem::getTonesForFlightProfile(double aoaDeg, double volume) {
    if (aoaDeg < 21) {
        return {};
    }

    if (aoaDeg >= 25) {
        // Ensure volume knob can not mute the sound anymore
        volume = std::max(volume, 0.3);
    }

    std::vector<Tone> tones;
    if (21 <= aoaDeg && aoaDeg <= 23.5) {
        double pulsePerSecond = math::rangeLerp(aoaDeg, 21, 23.5, 1.5, 6);
        tones.push_back({400, pulsePerSecond, volume});
    }
    if (23 <= aoaDeg && aoaDeg <= 25) {
        tones.push_back({900, std::nullopt, volume});
    }
    if (24.5 <= aoaDeg && aoaDeg <= 29) {
        double pulsePerSecond = math::rangeLerp(aoaDeg, 24.5, 29, 1.5, 9.6);
        tones.push_back({1600, pulsePerSecond, volume});
    }
    if (aoaDeg > 29) {
        tones.push_back({1600, 20, volume});
    }

    return tones;
}

double AuralToneSystem::getVolume() const {
    return engine.lock()->getKnobValue(Knob::AURAL_TONE_VOLUME);
}
} // phantom
