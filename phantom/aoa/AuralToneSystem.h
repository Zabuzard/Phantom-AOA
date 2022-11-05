#ifndef PHANTOM_AOA_AURALTONESYSTEM_H
#define PHANTOM_AOA_AURALTONESYSTEM_H

#include <optional>

#include "../../engine/Entity.h"
#include "AOASensor.h"
#include "AOAPowerSystem.h"

namespace phantom {
struct Tone final {
    double frequency;
    double volume;
};

class AuralToneSystem final : public Entity {
public:
    explicit AuralToneSystem(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem);

    ~AuralToneSystem() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::optional<Tone> getTone() const;

private:
    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;

    std::optional<Tone> tone;
};
} // phantom

#endif //PHANTOM_AOA_AURALTONESYSTEM_H
