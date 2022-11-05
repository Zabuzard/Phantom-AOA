#ifndef PHANTOM_AOA_CONTROLS_H
#define PHANTOM_AOA_CONTROLS_H

#include <cstdint>
#include <windows.h>

namespace controls {
constexpr int32_t PITCH_UP = VK_UP;
constexpr int32_t PITCH_DOWN = VK_DOWN;
constexpr int32_t RUDDER_LEFT = VK_LEFT;
constexpr int32_t RUDDER_RIGHT = VK_RIGHT;

constexpr int32_t ESSENTIAL_DC_BUS_POWER = 'Q';
constexpr int32_t RIGHT_MAIN_AC_BUS_POWER = 'W';
constexpr int32_t INSTRUMENT_AC_BUS = 'E';

constexpr int32_t AOA_PROBE_HTR_PWR_CB = 'A';
constexpr int32_t AOA_PROBE_HTR_CONT_CB = 'S';
}

#endif //PHANTOM_AOA_CONTROLS_H
