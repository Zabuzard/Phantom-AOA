#ifndef PHANTOM_AOA_AOASENSOR_H
#define PHANTOM_AOA_AOASENSOR_H

#include "../../engine/Entity.h"
#include "../../engine/Engine.h"

#include <memory>

class AOASensor final: public Entity {
public:
    explicit AOASensor(std::weak_ptr<const Engine> engine);

    void initialize() override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::string render() const override;

    [[nodiscard]] double getAOADeg() const;
private:
    const std::weak_ptr<const Engine> engine;

    double physicalAOADeg = 0.0;
    double measuredAOADeg = 0.0;

    void updatePhysicalAOA();
    void simulateSensorReading();
};

#endif //PHANTOM_AOA_AOASENSOR_H
