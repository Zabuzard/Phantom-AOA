#ifndef PHANTOM_AOA_ENGINE_H
#define PHANTOM_AOA_ENGINE_H

#include <unordered_set>
#include <map>
#include <sstream>
#include <chrono>

#include "../math/Vector3.h"
#include "Entity.h"
#include "CircuitBreaker.h"
#include "Bus.h"
#include "Controls.h"
#include "Flag.h"
#include "Knob.h"

class Engine final : public Entity {
public:
    ~Engine() override = default;

    void initialize() override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::string render() const override;

    [[nodiscard]] Vector3 getPlayerFlightPath() const;

    /**
     * The axis around which the players aircraft would roll. Normal vector on the planes x-axis,
     * i.e. where the nose and chords are pointing to, or if the pilot looks straight ahead.
     * @return the players aircraft roll axis
     */
    [[nodiscard]] Vector3 getPlayerRollAxis() const;

    /**
     * The axis around which the players aircraft would pitch. Normal vector on the planes y-axis,
     * i.e. if the pilot would look to the left.
     * @return the players aircraft pitch axis
     */
    [[nodiscard]] Vector3 getPlayerPitchAxis() const;

    /**
     * The axis around which the players aircraft would yaw. Normal vector on the planes z-axis,
     * i.e. if the pilot would look up.
     * @return the players aircraft yaw axis
     */
    [[nodiscard]] Vector3 getPlayerYawAxis() const;

    [[nodiscard]] bool isBusPowered(Bus bus) const;

    [[nodiscard]] bool isCircuitBreakerOut(CircuitBreaker circuitBreaker) const;

    [[nodiscard]] double getOutsideTemperatureCelsius() const;

    [[nodiscard]] bool isFlagActive(Flag flag) const;

    [[nodiscard]] double getKnobValue(Knob knob) const;

private:
    static constexpr double CHANGE_PITCH_DEG_PER_SECOND = 5;
    static constexpr double CHANGE_YAW_DEG_PER_SECOND = 5;
    static constexpr double CHANGE_KNOB_VALUE_PER_SECOND = 0.2;

    const std::map<int32_t, Bus> keyToBus{
            {controls::ESSENTIAL_DC_BUS_POWER,  Bus::ESSENTIAL_DC},
            {controls::RIGHT_MAIN_AC_BUS_POWER, Bus::RIGHT_MAIN_AC},
            {controls::INSTRUMENT_AC_BUS_POWER, Bus::INSTRUMENT_AC}
    };

    const std::map<int32_t, CircuitBreaker> keyToCircuitBreaker{
            {controls::AOA_PROBE_HTR_PWR_CB,    CircuitBreaker::AOA_PROBE_HTR_PWR},
            {controls::AOA_PROBE_HTR_CONT_CB,   CircuitBreaker::AOA_PROBE_HTR_CONT},
            {controls::ESS_DC_CADC_CB,          CircuitBreaker::ESS_DC_CADC},
            {controls::INSTR_AC_CADC_CB,        CircuitBreaker::INSTR_AC_CADC},
            {controls::RM_VAC_AC_CADC_PWR_1_CB, CircuitBreaker::RM_VAC_AC_CADC_PWR_1},
            {controls::RM_VAC_AC_CADC_PWR_2_CB, CircuitBreaker::RM_VAC_AC_CADC_PWR_2},
            {controls::RM_VAC_AC_CADC_PWR_3_CB, CircuitBreaker::RM_VAC_AC_CADC_PWR_3}
    };

    const std::map<int32_t, Flag> keyToFlag{
            {controls::NOSE_WHEEL_FLAG, Flag::NOSE_WHEEL_EXTENDED},
            {controls::WEIGHT_ON_WHEEL_FLAG, Flag::WEIGHT_ON_WHEEL},
            {controls::GEAR_FLAG, Flag::GEAR_EXTENDED},
            {controls::SLATS_FLAG, Flag::SLATS_IN}
    };

    const std::map<int32_t, Knob> keyToKnobDecrease{
            {controls::INSTR_PANEL_LIGHT_DECR_KNOB, Knob::INSTR_PANEL_LIGHT_INTENSITY},
            {controls::EMERGENCY_FLOODLIGHT_DECR_KNOB, Knob::EMERGENCY_FLOODLIGHT_INTENSITY}
    };

    const std::map<int32_t, Knob> keyToKnobIncrease{
            {controls::INSTR_PANEL_LIGHT_INCR_KNOB, Knob::INSTR_PANEL_LIGHT_INTENSITY},
            {controls::EMERGENCY_FLOODLIGHT_INCR_KNOB, Knob::EMERGENCY_FLOODLIGHT_INTENSITY}
    };

    // NOTE Values can also be adjusted here manually to play with the system
    Vector3 playerFlightPath{10, 0, 0};
    Vector3 playerRollAxis{10, 0, 0};
    Vector3 playerPitchAxis{0, 10, 0};
    Vector3 playerYawAxis{0, 0, 10};
    std::unordered_set<Bus> poweredBuses{Bus::RIGHT_MAIN_AC, Bus::ESSENTIAL_DC, Bus::INSTRUMENT_AC};
    std::unordered_set<CircuitBreaker> pulledCircuitBreakers;
    std::chrono::high_resolution_clock::time_point lastToggleInput = std::chrono::high_resolution_clock::now();
    std::unordered_set<Flag> activeFlags;
    std::map<Knob, double> knobToValue{
            {Knob::INSTR_PANEL_LIGHT_INTENSITY,    0},
            {Knob::EMERGENCY_FLOODLIGHT_INTENSITY, 0}
    };

    static bool isKeyPressed(int keyCode);

    template<class T>
    void updateToggleInputs(std::unordered_set<T>& activeComponents, const std::map<int32_t, T>& keyToComponent) {
        if (isToggleInputOnCooldown()) {
            return;
        }

        // NOTE This is not atomic, but that is not really an issue for now
        bool hadInput = false;
        for (const auto& [key, component]: keyToComponent) {
            if (!isKeyPressed(key)) {
                continue;
            }
            hadInput = true;

            if (activeComponents.erase(component) == 0) {
                activeComponents.emplace(component);
            }
        }

        if (hadInput) {
            lastToggleInput = std::chrono::high_resolution_clock::now();
        }
    }

    void updateKnobValues(double deltaTimeSeconds);

    void updateAircraftOrientation(double deltaTimeSeconds);

    [[nodiscard]] bool isToggleInputOnCooldown();
};

#endif //PHANTOM_AOA_ENGINE_H
