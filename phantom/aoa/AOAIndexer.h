#ifndef PHANTOM_AOA_AOAINDEXER_H
#define PHANTOM_AOA_AOAINDEXER_H

#include <unordered_set>

#include "../../engine/Entity.h"
#include "AOASensor.h"

namespace phantom {
enum class Lamp {
    LOW_SPEED,
    ON_SPEED,
    HIGH_SPEED
};

class AOAIndexer final : public Entity {
public:
    explicit AOAIndexer(std::weak_ptr<const AOASensor> sensor);

    ~AOAIndexer() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::unordered_set<Lamp> getIlluminatedLamps() const;

private:
    const std::weak_ptr<const AOASensor> sensor;

    std::unordered_set<Lamp> illuminatedLamps;
};
} // phantom

#endif //PHANTOM_AOA_AOAINDEXER_H
