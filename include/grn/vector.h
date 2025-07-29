#pragma once
#include <cmath>

namespace grn
{

    struct Vector
    {
        float x, y, z;

        Vector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
        Vector(const Vector& other) : x(other.x), y(other.y), z(other.z) {}

        Vector operator+(const Vector& other) const
        {
            return Vector(x + other.x, y + other.y, z + other.z);
        }

        Vector operator-(const Vector& other) const
        {
            return Vector(x - other.x, y - other.y, z - other.z);
        }

        Vector operator-() const
        {
            return Vector(-x, -y, -z);
        }

        Vector operator*(float scalar) const
        {
            return Vector(x * scalar, y * scalar, z * scalar);
        }

        Vector operator/(float scalar) const
        {
            return Vector(x / scalar, y / scalar, z / scalar);
        }

        operator const float*() const
        {
            return &x;
        }

        const float* data() const
        {
            return &x;
        }

        float dot(const Vector& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }
        Vector cross(const Vector& other) const
        {            return Vector(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }
        float length() const
        {
            return sqrt(x * x + y * y + z * z);
        }
        Vector normalize() const
        {
            float len = length();
            if (len == 0) return Vector(0, 0, 0);
            return Vector(x / len, y / len, z / len);
        }

    };

} // namespace grn
