#ifndef PHANTOM_AOA_PHANTOM_H
#define PHANTOM_AOA_PHANTOM_H

#include "../engine/Entity.h"
#include "aoa/AOAPowerSystem.h"

#include <memory>
#include <vector>

namespace phantom {

class Phantom final : public Entity {
public:
    explicit Phantom(const std::weak_ptr<const Engine>& engine);

    void initialize() override;

    [[nodiscard]] std::string render() const override;

    void update(double deltaTimeSeconds) override;

private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::shared_ptr<AOAPowerSystem> aoaPowerSystem;
};

} // phantom

#endif //PHANTOM_AOA_PHANTOM_H
