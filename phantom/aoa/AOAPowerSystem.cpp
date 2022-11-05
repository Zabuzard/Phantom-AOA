#include "AOAPowerSystem.h"

namespace phantom {
AOAPowerSystem::AOAPowerSystem(std::weak_ptr<const Engine> engine) : engine{std::move(engine)} {}

boolean AOAPowerSystem::hasPrimarySystemPower() const {
    auto lockedEngine = engine.lock();

    // TODO The details on this are a bit unclear, needs further clarification
    bool isRightMainBusPowered = lockedEngine->isBusPowered(Bus::RIGHT_MAIN_AC)
                                 && !lockedEngine->isCircuitBreakerOut(CircuitBreaker::RM_VAC_AC_CADC_PWR_1)
                                 && !lockedEngine->isCircuitBreakerOut(CircuitBreaker::RM_VAC_AC_CADC_PWR_2)
                                 && !lockedEngine->isCircuitBreakerOut(CircuitBreaker::RM_VAC_AC_CADC_PWR_3);

    return isRightMainBusPowered
           && !lockedEngine->isCircuitBreakerOut(CircuitBreaker::AOA_PROBE_HTR_PWR);
}

boolean AOAPowerSystem::hasSecondarySystemPower() const {
    auto lockedEngine = engine.lock();

    // TODO The details on this are a bit unclear, needs further clarification
    bool isEssentialBusPowered = lockedEngine->isBusPowered(Bus::ESSENTIAL_DC)
                                 && !lockedEngine->isCircuitBreakerOut(CircuitBreaker::ESS_DC_CADC);

    return isEssentialBusPowered
           && !lockedEngine->isCircuitBreakerOut(CircuitBreaker::AOA_PROBE_HTR_CONT);
}
} // phantom
