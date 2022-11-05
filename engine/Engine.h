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

class Engine final : public Entity {
public:
    ~Engine() override = default;

    void initialize() override;

    void update(double deltaTimeSeconds) override;

    [[nodiscard]] std::string render() const override;

    [[nodiscard]] Vector3 getPlayerFlightPath() const;

    [[nodiscard]] Vector3 getPlayerChordLine() const;

    [[nodiscard]] bool isBusPowered(Bus bus) const;

    [[nodiscard]] bool isCircuitBreakerOut(CircuitBreaker circuitBreaker) const;

private:
    static constexpr double CHANGE_PITCH_PER_SECOND = 0.2;
    static constexpr double CHANGE_RUDDER_PER_SECOND = 0.2;

    const std::map<int32_t, Bus> keyToBus{
            {controls::ESSENTIAL_DC_BUS_POWER,  Bus::ESSENTIAL_DC},
            {controls::RIGHT_MAIN_AC_BUS_POWER, Bus::RIGHT_MAIN_AC},
            {controls::INSTRUMENT_AC_BUS,       Bus::INSTRUMENT_AC}
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

    // NOTE Values can also be adjusted here manually to play with the system
    Vector3 playerFlightPath{1, 0, 0};
    Vector3 playerChordLine{1, 0, 0};
    std::unordered_set<Bus> poweredBuses{Bus::RIGHT_MAIN_AC, Bus::ESSENTIAL_DC, Bus::INSTRUMENT_AC};
    std::unordered_set<CircuitBreaker> pulledCircuitBreakers;
    std::chrono::high_resolution_clock::time_point lastToggleInput = std::chrono::high_resolution_clock::now();

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

    void updateAircraftOrientation(double deltaTimeSeconds);

    [[nodiscard]] bool isToggleInputOnCooldown();
};

#endif //PHANTOM_AOA_ENGINE_H
