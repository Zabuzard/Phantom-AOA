#include "Simulation.h"

#include <iostream>
#include <thread>

#include "../phantom/Phantom.h"
#include "../util/Ansi.h"

Simulation::Simulation() {
    auto engine = std::make_shared<Engine>();
    entities.push_back(engine);

    entities.push_back(std::make_shared<phantom::Phantom>(engine));
}

void Simulation::run() {
    initializeAll();

    // Basic simulation loop with decoupled TPS and FPS
    // TPS is strict and will always be met
    // FPS is variable and takes the remaining time, but is capped
    // NOTE Since this is not really scope of the task, no need to get more fancy than this.
    //  It is enough for manual testing.
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

void Simulation::initializeAll() const {
    for (const auto& entity: entities) {
        entity->initialize();
    }
}

void Simulation::renderAll() const {
    std::cout << ansi::CLEAR_SCREEN
              << "Best viewed on a ANSI-compatible console. Interaction only works on Windows. Press CTRL+C to end the simulation.\n"
              << "Press the arrow keys to change the aircraft orientation.\n"
              << "Press QWER to toggle buses, ASDFGHJ for circuit breakers.\n"
              << "Rotate knobs: Instr lights (TZ), Emr flood (UI), Indx light (OP)\n"
              << "Further: Nose wheel (Y), Weight on Wheel (X), Gear (C), Slats (V)\n\n";

    for (const auto& entity: entities) {
        auto renderResult = entity->render();
        if (!renderResult.empty()) {
            std::cout << renderResult << '\n';
        }
    }
}

void Simulation::updateAll(double deltaTimeSeconds) const {
    for (const auto& entity: entities) {
        entity->update(deltaTimeSeconds);
    }
}
