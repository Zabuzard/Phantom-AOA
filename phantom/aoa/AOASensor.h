#ifndef PHANTOM_AOA_AOASENSOR_H
#define PHANTOM_AOA_AOASENSOR_H

#include "../../engine/Entity.h"
#include "../../engine/Engine.h"

#include <memory>

namespace phantom {
class AOASensor final : public Entity {
public:
    explicit AOASensor(std::weak_ptr<const Engine> engine);

    ~AOASensor() override = default;

    void initialize() override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::string render() const override;

    /**
     * Gets the AOA measured by this sensor.
     * <p>
     * The sensor does not differentiate between reflex angles and others, as soon as the AOA would go
     * beyond 180° (flying backwards), the angle is measured from other direction and decreases back to 0°.
     *
     * The AOA is also never negative. Rapidly pitching down hard is treated equally to pitching up,
     * as positive angle (this can occur during extreme conditions, such as stalls or spins).
     *
     * @return the AOA in degrees between 0 (inclusive) and 180 (inclusive)
     */
    [[nodiscard]] double getAOADeg() const;

private:
    const std::weak_ptr<const Engine> engine;

    double physicalAOADeg = 0.0;
    double measuredAOADeg = 0.0;

    void updatePhysicalAOA();

    void simulateSensorReading();
};
} // phantom

#endif //PHANTOM_AOA_AOASENSOR_H
