#ifndef PHANTOM_AOA_AURALTONESYSTEM_H
#define PHANTOM_AOA_AURALTONESYSTEM_H

#include <optional>
#include <vector>

#include "../../engine/Entity.h"
#include "../../engine/Engine.h"
#include "AOASensor.h"
#include "AOAPowerSystem.h"

namespace phantom {
struct Tone final {
    double frequency = 0;
    std::optional<double> pulsePerSecond;
    double volume = 0;
};

class AuralToneSystem final : public Entity {
public:
    explicit AuralToneSystem(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem,
                             std::weak_ptr<const Engine> engine);

    ~AuralToneSystem() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;
    const std::weak_ptr<const Engine> engine;

    std::vector<Tone> tones;

    [[nodiscard]] static std::vector<Tone> getTonesForLandingProfile(double aoaDeg, double volume);

    [[nodiscard]] static std::vector<Tone> getTonesForFlightProfile(double aoaDeg, double volume);

    [[nodiscard]] double getVolume() const;
};
} // phantom

#endif //PHANTOM_AOA_AURALTONESYSTEM_H
