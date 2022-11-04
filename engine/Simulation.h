#ifndef PHANTOM_AOA_SIMULATION_H
#define PHANTOM_AOA_SIMULATION_H

#include <atomic>
#include <vector>
#include <memory>
#include <chrono>

#include "Entity.h"

class Simulation final {
public:
    Simulation();

    void run();

    void stop();

private:
    static const int TPS = 60;
    static const int FPS = 15;

    std::atomic<bool> shouldStop = false;
    std::vector<std::shared_ptr<Entity>> entities;

    void initializeAll() const;

    void renderAll() const;

    void updateAll(double deltaTimeSeconds) const;
};

#endif //PHANTOM_AOA_SIMULATION_H
