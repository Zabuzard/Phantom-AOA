#include "Engine.h"

#include <windows.h>
#include <iostream>
#include <bitset>

void Engine::initialize() {}

std::string Engine::render() const {
    return "playerFlightPath: " + playerFlightPath.toString() + '\n';
}

Vector3 Engine::getPlayerFlightPath() const {
    return playerFlightPath;
}

Vector3 Engine::getPlayerChordLine() const {
    return playerChordLine;
}

void Engine::update(double deltaTimeSeconds) {
    // TODO If theres time, change this to actually change by degrees of the angle
    // NOTE Technically this does not actually change the pitch and rudder (as in, degrees or similar),
    //  just the position in the respective axis. But since this is only needed for testing, no need to get fancy here.
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
    auto keyState = GetKeyState(keyCode);

    // High order bit indicates pressed
    constexpr auto bits = CHAR_BIT * sizeof(SHORT);
    return std::bitset<bits>(keyState)[bits - 1];
}


