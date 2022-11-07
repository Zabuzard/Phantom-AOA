#ifndef PHANTOM_AOA_VECTOR3_H
#define PHANTOM_AOA_VECTOR3_H

#include <string>
#include <sstream>
#include <cmath>

struct Vector3 final {
    // TODO This class should be heavily unit tested to ensure that there are no bugs at the core of the logic
    double x;
    double y;
    double z;

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "(" << x << ", "
           << y << ", "
           << z << ")";
        return ss.str();
    }

    [[nodiscard]] Vector3 operator+(const Vector3& other) const {
        return {x + other.x,
                y + other.y,
                z + other.z};
    }

    [[nodiscard]] Vector3 operator-(const Vector3& other) const {
        return {x - other.x,
                y - other.y,
                z - other.z};
    }

    [[nodiscard]] double dot(const Vector3& other) const {
        return x * other.x
               + y * other.y
               + z * other.z;
    }

    [[nodiscard]] Vector3 cross(const Vector3& other) const {
        return {y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x};
    }

    [[nodiscard]] Vector3 operator*(double scalar) const {
        return {scalar * x,
                scalar * y,
                scalar * z};
    }

    [[nodiscard]] Vector3 operator/(double scalar) const {
        return {x / scalar,
                y / scalar,
                z / scalar};
    }

    [[nodiscard]] double length() const {
        return std::sqrt(x * x
                         + y * y
                         + z * z);
    }

    [[nodiscard]] Vector3 normalized() const {
        return this->operator/(length());
    }

    /**
     * Computes the angle between this and the given vector, in radians.
     * <p>
     * This does not differentiate between reflex angles and others, as soon as the angle would go
     * beyond PI (180°), the angle is measured from other direction and decreases back to 0.
     *
     * @param other to measure the angle against
     * @return the angle between the two vectors, between 0 (inclusive) and PI (inclusive)
     */
    [[nodiscard]] double angleRad(const Vector3& other) const {
        return std::acos(normalized().dot(other.normalized()));
    }

    /**
     * Computes the signed angle between this and the given vector, in radians.
     * <p>
     * This does not differentiate between reflex angles and others, as soon as the angle would go
     * beyond PI (180°), the angle is measured from other direction. Hence, it jumps to -PI (-180°)
     * and increases back to 0.
     * <p>
     * A negative angle indicates that the vector goes into the other direction than the given other vector,
     * projected on the given plane.
     *
     * @param other to measure the angle against
     * @param directionNormal the normal vector of the plane used as reference for the direction.
     *                        The direction is measured by projecting both vectors onto that plane.
     * @return the angle between the two vectors, between -PI (inclusive) and PI (inclusive)
     */
    [[nodiscard]] double signedAngleRad(const Vector3& other, const Vector3& directionNormal) const {
        Vector3 currentNormal = cross(other);
        double orientation = directionNormal.dot(currentNormal);

        double result = angleRad(other);
        return orientation > 0 ? -result : result;
    }

    /**
     * Rotates this vector around the X-axis. Rotation occurs clockwise.
     * <p>
     * A left-pointing vector, such as (0, 1, 0) will be rotated upwards for a positive angle.
     *
     * @param angleRad the angle to rotate the vector around the axis, in radians
     * @return the rotated vector
     */
    [[nodiscard]] Vector3 rotatedAroundXAxis(double angleRad) const {
        return {x,
                y * std::cos(angleRad) - z * std::sin(angleRad),
                y * std::sin(angleRad) + z * std::cos(angleRad)};
    }

    /**
     * Rotates this vector around the Y-axis. Rotation occurs clockwise.
     * <p>
     * A forward-pointing vector, such as (1, 0, 0) will be pitched down for a positive angle.
     *
     * @param angleRad the angle to rotate the vector around the axis, in radians
     * @return the rotated vector
     */
    [[nodiscard]] Vector3 rotatedAroundYAxis(double angleRad) const {
        return {x * std::cos(angleRad) + z * std::sin(angleRad),
                y,
                -x * std::sin(angleRad) + z * std::cos(angleRad)};
    }

    /**
     * Rotates this vector around the Z-axis. Rotation occurs clockwise.
     * <p>
     * A forward-pointing vector, such as (1, 0, 0) will be rotated to the left for a positive angle.
     *
     * @param angleRad the angle to rotate the vector around the axis, in radians
     * @return the rotated vector
     */
    [[nodiscard]] Vector3 rotatedAroundZAxis(double angleRad) const {
        return {x * std::cos(angleRad) - y * std::sin(angleRad),
                x * std::sin(angleRad) + y * std::cos(angleRad),
                z};
    }

    [[nodiscard]] Vector3 projectedOntoPlane(const Vector3& planeSpanning1, const Vector3& planeSpanning2) const {
        double scalar1 = dot(planeSpanning1) / planeSpanning1.dot(planeSpanning1);
        double scalar2 = dot(planeSpanning2) / planeSpanning2.dot(planeSpanning2);

        return (planeSpanning1 * scalar1) + (planeSpanning2 * scalar2);
    }
};

#endif //PHANTOM_AOA_VECTOR3_H
