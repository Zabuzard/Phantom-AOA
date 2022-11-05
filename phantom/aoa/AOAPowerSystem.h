#ifndef PHANTOM_AOA_AOAPOWERSYSTEM_H
#define PHANTOM_AOA_AOAPOWERSYSTEM_H

#include "../../engine/Engine.h"

#include <memory>

namespace phantom {
class AOAPowerSystem final {
public:
    explicit AOAPowerSystem(std::weak_ptr<const Engine> engine);

    /**
     * Whether the primary AOA system has power.
     * This includes the sensor itself and the indicator.
     *
     * @return Whether the primary AOA system has power
     */
    [[nodiscard]] boolean hasPrimarySystemPower() const;

    /**
     * Whether the secondary AOA system has power. This includes all other components,
     * such as the indexer, stall warning vibrator or the aural tone system.
     *
     * @return Whether the secondary AOA system has power
     */
    [[nodiscard]] boolean hasSecondarySystemPower() const;

private:
    const std::weak_ptr<const Engine> engine;
};
} // phantom

#endif //PHANTOM_AOA_AOAPOWERSYSTEM_H
