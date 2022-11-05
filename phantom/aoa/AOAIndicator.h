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
    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;

    double indicatedAOADeg = 0.0;
    boolean showOffFlag = true;
};
} // phantom

#endif //PHANTOM_AOA_AOAINDICATOR_H
