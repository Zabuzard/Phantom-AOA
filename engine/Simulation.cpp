#include "Simulation.h"

#include <iostream>
#include <thread>

#include "../phantom/TestEntity.h"

Simulation::Simulation() {
    entities.push_back(std::make_shared<TestEntity>());
}

void Simulation::run() {
    // Basic simulation loop with decoupled TPS and FPS
    // TPS is strict and will always be met
    // FPS is variable and takes the remaining time, but is capped
    double deltaTimeSeconds = 1.0 / TPS;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    double secondsToConsume = 0.0;

    while (!shouldStop) {
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        double secondsLastFrameNeeded = std::chrono::duration<double>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;

        secondsToConsume += secondsLastFrameNeeded;
        while (secondsToConsume >= deltaTimeSeconds) {
            updateAll(deltaTimeSeconds);
            secondsToConsume -= deltaTimeSeconds;
        }

        renderAll();

        double currentFps = 1.0 / secondsLastFrameNeeded;
        if (currentFps > FPS) {
            // Cap FPS by wasting time if it was too fast
            double targetSecondsPerFrame = 1.0 / FPS;
            double secondsToWaste = std::abs(targetSecondsPerFrame - secondsLastFrameNeeded);
            std::this_thread::sleep_for(std::chrono::duration<double>(secondsToWaste));
        }
    }
}

void Simulation::stop() {
    shouldStop = true;
}

void Simulation::initializeAll() {
    for (const auto &entity: entities) {
        entity->initialize();
    }
}

const std::string ANSI_CLEAR_SCREEN = "\033[2J";

void Simulation::renderAll() {
    std::cout << ANSI_CLEAR_SCREEN;
    for (const auto &entity: entities) {
        std::cout << entity->render() << '\n';
    }
}

void Simulation::updateAll(double deltaTimeSeconds) {
    for (const auto &entity: entities) {
        entity->update(deltaTimeSeconds);
    }
}
