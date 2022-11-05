#ifndef PHANTOM_AOA_VECTOR3_H
#define PHANTOM_AOA_VECTOR3_H

#include <string>
#include <sstream>
#include <cmath>

struct Vector3 final {
    double x;
    double y;
    double z;

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }

    [[nodiscard]] double dot(const Vector3 &other) const {
        return x * other.x
               + y * other.y
               + z * other.z;
    }

    [[nodiscard]] double length() const {
        return std::sqrt(x * x
                         + y * y
                         + z * z);
    }

    [[nodiscard]] double angleRad(const Vector3 &other) const {
        return std::acos(dot(other) /
                         (length() * other.length()));
    }
};

#endif //PHANTOM_AOA_VECTOR3_H
