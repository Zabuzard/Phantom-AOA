#ifndef PHANTOM_AOA_AOAINDICATOR_H
#define PHANTOM_AOA_AOAINDICATOR_H

#include "../../engine/Entity.h"
#include "../../engine/Engine.h"
#include "../../util/Ansi.h"

#include "AOASensor.h"
#include "AOAPowerSystem.h"

#include <optional>
#include <map>

namespace phantom {

namespace indicator {
enum class LampColor {
    WHITE,
    RED
};

// MIL-I-18856B(AS): 7
enum class LampType {
    LOW,
    MEDIUM,
    HIGH
};

struct Lamp final {
    LampType type;
    double intensity;
    LampColor color;
};
} // indicator

class AOAIndicator final : public Entity {
public:
    explicit AOAIndicator(std::weak_ptr<const AOASensor> sensor, std::weak_ptr<const AOAPowerSystem> powerSystem,
                          std::weak_ptr<const Engine> engine);

    ~AOAIndicator() override = default;

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    // MIL-I-18856B(AS): 8
    static constexpr double SMALL_DIFF_NEEDLE_SPEED_DEG_PER_SECOND = 4.5 / 1.0;
    // MIL-I-18856B(AS): 8, 15
    static constexpr double GREAT_DIFF_NEEDLE_SPEED_DEG_PER_SECOND = 20.0 / 3.0;

    const std::weak_ptr<const AOASensor> sensor;
    const std::weak_ptr<const AOAPowerSystem> powerSystem;
    const std::weak_ptr<const Engine> engine;

    double indicatedAOADeg = 0.0;
    boolean showOffFlag = true;
    boolean hasNeedleCaughtUp = true;
    std::optional<indicator::Lamp> illuminatedLamp;

    std::map<indicator::LampType, std::string> lampTypeToName{
            {indicator::LampType::LOW,    "low"},
            {indicator::LampType::MEDIUM, "med"},
            {indicator::LampType::HIGH,   "high"}
    };

    std::map<indicator::LampColor, std::string> lampColorToName{
            {indicator::LampColor::WHITE, ansi::WHITE_BACKGROUND + ansi::BLACK + "white" + ansi::COLOR_RESET},
            {indicator::LampColor::RED,   ansi::RED_BACKGROUND + ansi::WHITE + "red" + ansi::COLOR_RESET}
    };

    void updateIndication(double deltaTimeSeconds);

    void simulateNeedleLag(double aoaDeg, double deltaTimeSeconds);

    void updateLamp();
};
} // phantom

#endif //PHANTOM_AOA_AOAINDICATOR_H
