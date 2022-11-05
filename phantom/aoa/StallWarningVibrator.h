#ifndef PHANTOM_AOA_STALLWARNINGVIBRATOR_H
#define PHANTOM_AOA_STALLWARNINGVIBRATOR_H

#include "../../engine/Entity.h"
#include "AOASensor.h"
#include "AOAPowerSystem.h"

namespace phantom {
class StallWarningVibrator final : public Entity {
public:
    explicit StallWarningVibrator(std::weak_ptr<const AOASensor> sensor,
                                  std::weak_ptr<const AOAPowerSystem> powerSystem);

    ~StallWarningVibrator() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] bool isVibrating() const;

private:
    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;

    bool vibrating = false;
};
} // phantom

#endif //PHANTOM_AOA_STALLWARNINGVIBRATOR_H
