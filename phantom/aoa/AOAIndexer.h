#ifndef PHANTOM_AOA_AOAINDEXER_H
#define PHANTOM_AOA_AOAINDEXER_H

#include <unordered_set>

#include "../../engine/Entity.h"
#include "AOASensor.h"
#include "AOAPowerSystem.h"

namespace phantom {

namespace indexer {
enum class Lamp {
    LOW_SPEED,
    ON_SPEED,
    HIGH_SPEED
};
} // indexer

class AOAIndexer final : public Entity {
public:
    explicit AOAIndexer(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem,
                        std::weak_ptr<const Engine> engine);

    ~AOAIndexer() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;
    const std::weak_ptr<const Engine> engine;

    std::unordered_set<indexer::Lamp> illuminatedLamps;
    double lightIntensity = 0.1;

    void updateLamps(double aoaDeg);

    void updateLampsAgmCue();

    void updateLightIntensity();
};
} // phantom

#endif //PHANTOM_AOA_AOAINDEXER_H
