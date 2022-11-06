#ifndef PHANTOM_AOA_CONTROLS_H
#define PHANTOM_AOA_CONTROLS_H

#include <cstdint>
#include <windows.h>

namespace controls {
constexpr int32_t PITCH_UP = VK_DOWN;
constexpr int32_t PITCH_DOWN = VK_UP;
constexpr int32_t RUDDER_LEFT = VK_LEFT;
constexpr int32_t RUDDER_RIGHT = VK_RIGHT;

constexpr int32_t ESSENTIAL_DC_BUS_POWER = 'Q';
constexpr int32_t RIGHT_MAIN_AC_BUS_POWER = 'W';
constexpr int32_t INSTRUMENT_AC_BUS_POWER = 'E';
constexpr int32_t LEFT_MAIN_AC_BUS_POWER = 'R';

constexpr int32_t AOA_PROBE_HTR_PWR_CB = 'A';
constexpr int32_t AOA_PROBE_HTR_CONT_CB = 'S';
constexpr int32_t ESS_DC_CADC_CB = 'D';
constexpr int32_t INSTR_AC_CADC_CB = 'F';
constexpr int32_t RM_VAC_AC_CADC_PWR_1_CB = 'G';
constexpr int32_t RM_VAC_AC_CADC_PWR_2_CB = 'H';
constexpr int32_t RM_VAC_AC_CADC_PWR_3_CB = 'J';

constexpr int32_t NOSE_WHEEL_FLAG = 'Y';
constexpr int32_t WEIGHT_ON_WHEEL_FLAG = 'X';
constexpr int32_t GEAR_FLAG = 'C';
constexpr int32_t SLATS_FLAG = 'V';
constexpr int32_t AGM_45_SELECTED_FLAG = 'B';
constexpr int32_t FROZEN_AOA_SENSOR_FLAG = 'N';

constexpr int32_t INSTR_PANEL_LIGHT_DECR_KNOB = 'T';
constexpr int32_t INSTR_PANEL_LIGHT_INCR_KNOB = 'Z';
constexpr int32_t EMERGENCY_FLOODLIGHT_DECR_KNOB = 'U';
constexpr int32_t EMERGENCY_FLOODLIGHT_INCR_KNOB = 'I';
constexpr int32_t INDEXER_LIGHT_DECR_KNOB = 'O';
constexpr int32_t INDEXER_LIGHT_INCR_KNOB = 'P';
constexpr int32_t AURAL_TONE_VOLUME_DECR_KNOB = 'K';
constexpr int32_t AURAL_TONE_VOLUME_INCR_KNOB = 'L';
}

#endif //PHANTOM_AOA_CONTROLS_H
