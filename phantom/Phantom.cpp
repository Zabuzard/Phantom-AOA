#include "Phantom.h"

#include "../phantom/aoa/AOASensor.h"
#include "../phantom/aoa/AOAIndicator.h"
#include "../phantom/aoa/AOAIndexer.h"
#include "../phantom/aoa/StallWarningVibrator.h"
#include "../phantom/aoa/AuralToneSystem.h"

#include <sstream>

namespace phantom {
Phantom::Phantom(const std::weak_ptr<const Engine>& engine) {
    aoaPowerSystem = std::make_shared<phantom::AOAPowerSystem>(engine);

    auto sensor = std::make_shared<phantom::AOASensor>(engine, aoaPowerSystem);
    entities.push_back(sensor);

    entities.push_back(std::make_shared<phantom::AOAIndicator>(sensor, aoaPowerSystem));
    entities.push_back(std::make_shared<phantom::AOAIndexer>(sensor, aoaPowerSystem));
    entities.push_back(std::make_shared<phantom::StallWarningVibrator>(sensor, aoaPowerSystem, engine));
    entities.push_back(std::make_shared<phantom::AuralToneSystem>(sensor, aoaPowerSystem, engine));
}

void Phantom::initialize() {
    for (const auto& entity: entities) {
        entity->initialize();
    }
}

std::string Phantom::render() const {
    std::stringstream ss;

    for (const auto& entity: entities) {
        auto renderResult = entity->render();
        if (!renderResult.empty()) {
            ss << renderResult << '\n';
        }
    }
    return ss.str();
}

void Phantom::update(double deltaTimeSeconds) {
    for (const auto& entity: entities) {
        entity->update(deltaTimeSeconds);
    }
}
} // phantom
