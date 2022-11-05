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
       << (illuminatedLamps.contains(Lamp::LOW_SPEED) ? ">" : " ")
       << (illuminatedLamps.contains(Lamp::ON_SPEED) ? "o" : " ")
       << (illuminatedLamps.contains(Lamp::HIGH_SPEED) ? "<" : " ")
       << ")";
    return ss.str();
}

void AOAIndexer::update(double deltaTimeSeconds) {
    // TODO AGM-45 mode, AN/ASQ-91 self-test mode, light control know (intensity)
    illuminatedLamps.clear();

    std::optional<double> aoaDeg = sensor.lock()->getAOADeg();
    if (!aoaDeg || !powerSystem.lock()->hasSecondarySystemPower()) {
        return;
    }

    if (aoaDeg > 19.6) {
        illuminatedLamps.emplace(Lamp::LOW_SPEED);
    }
    if (aoaDeg > 18 && aoaDeg < 20.3) {
        illuminatedLamps.emplace(Lamp::ON_SPEED);
    }
    if (aoaDeg < 18.7) {
        illuminatedLamps.emplace(Lamp::HIGH_SPEED);
    }
}

std::unordered_set<Lamp> AOAIndexer::getIlluminatedLamps() const {
    return illuminatedLamps;
}
} // phantom
