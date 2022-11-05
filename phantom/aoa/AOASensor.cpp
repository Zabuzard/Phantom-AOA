#include "AOASensor.h"

#include <sstream>
#include "../../math/Math.h"

namespace phantom {
AOASensor::AOASensor(std::weak_ptr<const Engine> engine, std::weak_ptr<const AOAPowerSystem> powerSystem)
        : engine{std::move(engine)}, powerSystem{std::move(powerSystem)} {}

void AOASensor::initialize() {}

std::optional<double> AOASensor::getAOADeg() const {
    return measuredAOADeg;
}

std::string AOASensor::render() const {
    std::stringstream ss;
    ss << "sensor (";

    if (warmedUpAt) {
        auto duration = *warmedUpAt - std::chrono::high_resolution_clock::now();
        auto secondsUntilWarmedUp = std::chrono::duration_cast<std::chrono::seconds>(duration);

        ss << "warming up... " << secondsUntilWarmedUp.count() << "s";
    } else if (!measuredAOADeg) {
        ss << "no power";
    } else {
        ss << "measured: " << *measuredAOADeg << " deg";
    }

    ss << ", physical: " << physicalAOADeg << " deg)";

    return ss.str();
}

void AOASensor::update(double deltaTimeSeconds) {
    updatePhysicalAOA();
    simulateSensorReading();
}

void AOASensor::updatePhysicalAOA() {
    const Vector3 chordLine = engine.lock()->getPlayerChordLine();
    const Vector3 flightPath = engine.lock()->getPlayerFlightPath();

    double angleDeg = math::radToDeg(flightPath.angleRad(chordLine));

    physicalAOADeg = angleDeg;
}

void AOASensor::simulateSensorReading() {
    // TODO nose wheel changes, side slipping error, frozen error ...
    bool wasTurnedOff = !isTurnedOn;

    if (!powerSystem.lock()->hasPrimarySystemPower()) {
        isTurnedOn = false;
        measuredAOADeg = std::nullopt;
        return;
    }
    isTurnedOn = true;

    if (wasTurnedOff) {
        startWarmup();
    }
    if (warmedUpAt) {
        auto now = std::chrono::high_resolution_clock::now();
        if (now < warmedUpAt) {
            return;
        }

        warmedUpAt = std::nullopt;
    }

    measuredAOADeg = physicalAOADeg;
}

void AOASensor::startWarmup() {
    // NOTE Technically a full warmup would not be needed if just
    //  switching off and on, it should have some sort of cooldown
    using namespace std::chrono_literals;

    double temperature = engine.lock()->getOutsideTemperatureCelsius();
    double relevantTemperature = math::clamp(temperature, -54, 30);

    double warmUpDurationSeconds = math::rangeLerp(relevantTemperature, -54, 30, 120, 2);
    warmedUpAt = std::chrono::high_resolution_clock::now() +
                 std::chrono::seconds(static_cast<int32_t>(warmUpDurationSeconds));
}
} // phantom
