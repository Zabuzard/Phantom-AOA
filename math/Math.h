#ifndef PHANTOM_AOA_MATH_H
#define PHANTOM_AOA_MATH_H

#include <numbers>

namespace math {
    double radToDeg(double radians) {
        return radians * (180.0 / std::numbers::pi);
    }
} // math

#endif //PHANTOM_AOA_MATH_H
