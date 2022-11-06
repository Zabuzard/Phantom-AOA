#ifndef PHANTOM_AOA_AOASENSOR_H
#define PHANTOM_AOA_AOASENSOR_H

#include "../../engine/Entity.h"
#include "../../engine/Engine.h"
#include "AOAPowerSystem.h"

#include <memory>
#include <optional>
#include <chrono>

namespace phantom {
class AOASensor final : public Entity {
public:
    explicit AOASensor(std::weak_ptr<const Engine> engine, std::weak_ptr<const AOAPowerSystem> powerSystem);

    ~AOASensor() override = default;

    void initialize() override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::string render() const override;

    /**
     * Gets the AOA measured by this sensor.
     * <p>
     * The sensor does not differentiate between reflex angles and others, as soon as the AOA would go
     * beyond 180° (flying backwards), the angle is measured from other direction.
     * Hence, it jumps to -180° and increases back to 0°.
     * <p>
     * A negative AOA indicates pitching down hard (this can occur during extreme conditions, such as stalls or spins).
     * <p>
     * The sensor has a warmup period before it activates and also can only return measurements if it is powered.
     *
     * @return the AOA in degrees between -180 (inclusive) and 180 (inclusive), if the sensor is ready to measure
     */
    [[nodiscard]] std::optional<double> getAOADeg() const;

private:
    const std::weak_ptr<const Engine> engine;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;

    double physicalAOADeg = 0.0;
    double physicalSideSlipDeg = 0.0;
    std::optional<double> measuredAOADeg;
    boolean isTurnedOn = false;
    std::optional<std::chrono::high_resolution_clock::time_point> warmedUpAt;

    void updatePhysicalAOA();

    void updatePhysicalSideSlip();

    void simulateSensorReading();

    void startWarmup();

    void simulateNoseWheelError();

    void simulateSideSlipError();

    static double patchNaN(double value);
};
} // phantom

#endif //PHANTOM_AOA_AOASENSOR_H
