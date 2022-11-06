#ifndef PHANTOM_AOA_MATH_H
#define PHANTOM_AOA_MATH_H

#include <numbers>

namespace math {
[[nodiscard]] inline double radToDeg(double radians) {
    return radians * (180.0 / std::numbers::pi);
}

[[nodiscard]] inline double degToRad(double degrees) {
    return degrees * (std::numbers::pi / 180.0);
}

[[nodiscard]] inline double clamp(double value, double min, double max) {
    return std::max(min, std::min(max, value));
}

[[nodiscard]] inline double lerp(double factor, double min, double max) {
    auto range = (max - min);
    return min + range * factor;
}

[[nodiscard]] inline double inverseLerp(double value, double min, double max) {
    auto range = (max - min);
    return (value - min) / range;
}

[[nodiscard]] inline double rangeLerp(double value1, double min1, double max1, double min2, double max2) {
    double factor1 = inverseLerp(value1, min1, max1);
    return lerp(factor1, min2, max2);
}

[[nodiscard]] inline double moveTowards(double source, double step, double target) {
    return source < target ? std::min(target, source + step)
                           : std::max(target, source - step);
}
} // math

#endif //PHANTOM_AOA_MATH_H
