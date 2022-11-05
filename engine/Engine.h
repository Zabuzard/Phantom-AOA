#ifndef PHANTOM_AOA_ENGINE_H
#define PHANTOM_AOA_ENGINE_H

#include "../math/Vector3.h"
#include "Entity.h"

class Engine final : public Entity {
public:
    ~Engine() override = default;

    void initialize() override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::string render() const override;

    [[nodiscard]] Vector3 getPlayerFlightPath() const;

    [[nodiscard]] Vector3 getPlayerChordLine() const;

private:
    static constexpr double CHANGE_PITCH_PER_SECOND = 0.2;
    static constexpr double CHANGE_RUDDER_PER_SECOND = 0.2;

    // NOTE Values can also be adjusted here manually to play with the system
    Vector3 playerFlightPath{1, 0, 0};
    Vector3 playerChordLine{1, 0, 0};

    static bool isKeyPressed(int keyCode);
};

#endif //PHANTOM_AOA_ENGINE_H
