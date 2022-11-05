#include "Simulation.h"

#include <iostream>
#include <thread>

#include "../phantom/aoa/AOASensor.h"
#include "../phantom/aoa/AOAIndicator.h"
#include "../phantom/aoa/AOAIndexer.h"
#include "../phantom/aoa/StallWarningVibrator.h"
#include "../phantom/aoa/AuralToneSystem.h"

Simulation::Simulation() {
    auto engine = std::make_shared<Engine>();
    entities.push_back(engine);

    addPhantom(engine);
}

void Simulation::addPhantom(const std::weak_ptr<const Engine>& engine) {
    auto sensor = std::make_shared<phantom::AOASensor>(engine);

    entities.push_back(sensor);
    entities.push_back(std::make_shared<phantom::AOAIndicator>(sensor));
    entities.push_back(std::make_shared<phantom::AOAIndexer>(sensor));
    entities.push_back(std::make_shared<phantom::StallWarningVibrator>(sensor));
    entities.push_back(std::make_shared<phantom::AuralToneSystem>(sensor));
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

const std::string ANSI_CLEAR_SCREEN = "\033[2J";

void Simulation::renderAll() const {
    std::cout << ANSI_CLEAR_SCREEN
              << "Best viewed on a ANSI-compatible console. Press CTRL+C to end the simulation.\n"
              << "Press the arrow keys to change the aircraft orientation.\n\n";

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
