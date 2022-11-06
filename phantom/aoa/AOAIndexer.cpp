#include "AOAIndexer.h"

#include "../../util/Ansi.h"

#include <sstream>

namespace phantom {
AOAIndexer::AOAIndexer(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem,
                       std::weak_ptr<const Engine> engine)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)}, engine{std::move(engine)} {}

void AOAIndexer::initialize() {}

std::string AOAIndexer::render() const {
    if (illuminatedLamps.empty() || lightIntensity == 0) {
        return "indexer (off)";
    }

    std::stringstream ss;
    ss << "indexer ("
       << ansi::BLACK_BACKGROUND
       << ansi::GREEN << (illuminatedLamps.contains(indexer::Lamp::LOW_SPEED) ? ">" : " ")
       << ansi::YELLOW << (illuminatedLamps.contains(indexer::Lamp::ON_SPEED) ? "o" : " ")
       << ansi::RED << (illuminatedLamps.contains(indexer::Lamp::HIGH_SPEED) ? "<" : " ")
       << ansi::COLOR_RESET
       << ", light: " << lightIntensity
       << ")";
    return ss.str();
}

void AOAIndexer::update(double deltaTimeSeconds) {
    // TODO AGM-45 mode, AN/ASQ-91 self-test mode, light control knob (intensity)
    illuminatedLamps.clear();

    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasSecondarySystemPower()) {
        return;
    }

    updateLamps(*aoaDeg);
    updateLightIntensity();
}

void AOAIndexer::updateLamps(double aoaDeg) {
    if (aoaDeg > 19.6) {
        illuminatedLamps.emplace(indexer::Lamp::LOW_SPEED);
    }
    if (aoaDeg > 18 && aoaDeg < 20.3) {
        illuminatedLamps.emplace(indexer::Lamp::ON_SPEED);
    }
    if (aoaDeg < 18.7) {
        illuminatedLamps.emplace(indexer::Lamp::HIGH_SPEED);
    }
}

void AOAIndexer::updateLightIntensity() {
    // TODO Probably has some circuit breakers as well
    if (!engine.lock()->isBusPowered(Bus::INSTRUMENT_AC)) {
        lightIntensity = 0;
        return;
    }

    lightIntensity = engine.lock()->getKnobValue(Knob::INDEXER_LIGHT_INTENSITY);
    // Knob can not turn the light completely off
    lightIntensity = std::max(0.1, lightIntensity);
}

std::unordered_set<indexer::Lamp> AOAIndexer::getIlluminatedLamps() const {
    return illuminatedLamps;
}
} // phantom
