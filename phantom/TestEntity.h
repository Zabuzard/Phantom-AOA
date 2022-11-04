#ifndef PHANTOM_AOA_TESTENTITY_H
#define PHANTOM_AOA_TESTENTITY_H

#include "../engine/Entity.h"

class TestEntity final : public Entity {
public:
    ~TestEntity() override = default;

    void initialize() override {

    }

    void update(double deltaTime) override {
        ticks++;
    }

    [[nodiscard]] std::string render() const override {
        return "this is a test, ticks: " + std::to_string(ticks);
    }

private:
    int32_t ticks;
};

#endif //PHANTOM_AOA_TESTENTITY_H
