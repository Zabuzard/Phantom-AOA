#include "AOAIndexer.h"

#include <sstream>

namespace phantom {
AOAIndexer::AOAIndexer(std::weak_ptr<const AOASensor> sensor) : sensor{std::move(sensor)} {}

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
    // TODO Does this get the data from the sensor or the laggy/dampened indicator?
    // TODO Power, circuit breakers, AGM-45 mode, AN/ASQ-91 self-test mode, light control know (intensity)
    double aoaDeg = sensor.lock()->getAOADeg();

    // NOTE This is not atomic, but we do not plan to run update and render in parallel
    illuminatedLamps.clear();
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
