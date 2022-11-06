#include "AOAIndexer.h"

#include <sstream>

namespace phantom {
AOAIndexer::AOAIndexer(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem)
        : sensor{std::move(sensor)}, powerSystem{std::move(powerSystem)} {}

void AOAIndexer::initialize() {}

std::string AOAIndexer::render() const {
    if (illuminatedLamps.empty()) {
        return "indexer (off)";
    }

    std::stringstream ss;
    ss << "indexer ("
       << (illuminatedLamps.contains(indexer::Lamp::LOW_SPEED) ? ">" : " ")
       << (illuminatedLamps.contains(indexer::Lamp::ON_SPEED) ? "o" : " ")
       << (illuminatedLamps.contains(indexer::Lamp::HIGH_SPEED) ? "<" : " ")
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

std::unordered_set<indexer::Lamp> AOAIndexer::getIlluminatedLamps() const {
    return illuminatedLamps;
}
} // phantom
