#include "Engine.h"

#include <windows.h>
#include <iostream>
#include <bitset>

void Engine::initialize() {}

std::string Engine::render() const {
    return "playerFlightPath: " + playerFlightPath.toString() + " (press arrow keys to move)";
}

Vector3 Engine::getPlayerFlightPath() const {
    return playerFlightPath;
}

Vector3 Engine::getPlayerChordLine() const {
    return playerChordLine;
}

void Engine::update(double deltaTimeSeconds) {
    double changePitchBy = 0;
    if (isKeyPressed(VK_UP)) {
        changePitchBy = CHANGE_PITCH_PER_SECOND;
    } else if (isKeyPressed(VK_DOWN)) {
        changePitchBy = -CHANGE_PITCH_PER_SECOND;
    }
    changePitchBy *= deltaTimeSeconds;

    playerFlightPath.z += changePitchBy;

    double changeRudderBy = 0;
    if (isKeyPressed(VK_RIGHT)) {
        changeRudderBy = CHANGE_RUDDER_PER_SECOND;
    } else if (isKeyPressed(VK_LEFT)) {
        changeRudderBy = -CHANGE_RUDDER_PER_SECOND;
    }
    changeRudderBy *= deltaTimeSeconds;

    playerFlightPath.y += changeRudderBy;
}

bool Engine::isKeyPressed(int keyCode) {
    // High order bit indicates pressed
    auto keyState = GetKeyState(keyCode);

    constexpr auto bits = CHAR_BIT * sizeof(SHORT);
    return std::bitset<bits>(keyState)[bits - 1];
}


