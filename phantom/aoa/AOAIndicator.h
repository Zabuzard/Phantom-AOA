#ifndef PHANTOM_AOA_AOAINDICATOR_H
#define PHANTOM_AOA_AOAINDICATOR_H

#include "../../engine/Entity.h"
#include "AOASensor.h"
#include "AOAPowerSystem.h"

namespace phantom {
class AOAIndicator final : public Entity {
public:
    explicit AOAIndicator(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem);

    ~AOAIndicator() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    static constexpr double SMALL_DIFF_NEEDLE_SPEED_DEG_PER_SECOND = 4.5 / 1.0;
    static constexpr double GREAT_DIFF_NEEDLE_SPEED_DEG_PER_SECOND = 20.0 / 3.0;

    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;

    double indicatedAOADeg = 0.0;
    boolean showOffFlag = true;
    boolean hasNeedleCaughtUp = true;

    void simulateNeedleLag(double aoaDeg, double deltaTimeSeconds);
};
} // phantom

#endif //PHANTOM_AOA_AOAINDICATOR_H
