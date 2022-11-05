#ifndef PHANTOM_AOA_ENTITY_H
#define PHANTOM_AOA_ENTITY_H

#include <concepts>
#include <string>

class Entity {
public:
    virtual ~Entity() = default;

    virtual void initialize() = 0;

    [[nodiscard]] virtual std::string render() const = 0;

    virtual void update(double deltaTimeSeconds) = 0;
};

#endif //PHANTOM_AOA_ENTITY_H
