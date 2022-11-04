#ifndef PHANTOM_AOA_VECTOR3_H
#define PHANTOM_AOA_VECTOR3_H

#include <string>
#include <sstream>

struct Vector3 final {
    double x;
    double y;
    double z;

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }
};

#endif //PHANTOM_AOA_VECTOR3_H
