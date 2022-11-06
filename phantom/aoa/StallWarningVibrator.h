#ifndef PHANTOM_AOA_STALLWARNINGVIBRATOR_H
#define PHANTOM_AOA_STALLWARNINGVIBRATOR_H

#include "../../engine/Engine.h"
#include "../../engine/Entity.h"
#include "AOASensor.h"
#include "AOAPowerSystem.h"

namespace phantom {
class StallWarningVibrator final : public Entity {
public:
    explicit StallWarningVibrator(std::weak_ptr<const AOASensor> sensor,
                                  std::weak_ptr<const AOAPowerSystem> powerSystem,
                                  std::weak_ptr<const Engine> engine);

    ~StallWarningVibrator() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;
    const std::weak_ptr<const Engine> engine;

    bool vibrating = false;

    [[nodiscard]] bool hasWeightOnWheel();
};
} // phantom

#endif //PHANTOM_AOA_STALLWARNINGVIBRATOR_H
