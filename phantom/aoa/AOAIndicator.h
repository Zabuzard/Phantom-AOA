#ifndef PHANTOM_AOA_AOAINDICATOR_H
#define PHANTOM_AOA_AOAINDICATOR_H

#include "../../engine/Entity.h"
#include "AOASensor.h"

namespace phantom {
class AOAIndicator final : public Entity {
public:
    explicit AOAIndicator(std::weak_ptr<const AOASensor> sensor);

    ~AOAIndicator() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    const std::weak_ptr<const AOASensor> sensor;

    double indicatedAOADeg = 0.0;
};
} // phantom

#endif //PHANTOM_AOA_AOAINDICATOR_H
