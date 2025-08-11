#pragma once
#include <cmath>
#include <string>

namespace grn
{

    struct Vector2i
    {
        int x, y;

        Vector2i(int x = 0, int y = 0) : x(x), y(y) {}
        Vector2i(const Vector2i &other) : x(other.x), y(other.y) {}
        Vector2i operator+(const Vector2i &other) const
        {
            return Vector2i(x + other.x, y + other.y);
        }
        Vector2i operator-(const Vector2i &other) const
        {
            return Vector2i(x - other.x, y - other.y);
        }
        operator const int *() const
        {
            return &x;
        }
        std::string toString() const
        {
            return "Vector2i(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };

    struct Vector2f
    {
        float x, y;

        Vector2f(float x = 0, float y = 0) : x(x), y(y) {}
        Vector2f(const Vector2f &other) : x(other.x), y(other.y) {}
        Vector2f operator+(const Vector2f &other) const
        {
            return Vector2f(x + other.x, y + other.y);
        }
        Vector2f operator-(const Vector2f &other) const
        {
            return Vector2f(x - other.x, y - other.y);
        }
        Vector2f operator*(float scalar) const
        {
            return Vector2f(x * scalar, y * scalar);
        }
        Vector2f operator/(float scalar) const
        {
            return Vector2f(x / scalar, y / scalar);
        }
        operator const float *() const
        {
            return &x;
        }
        const float *data() const
        {
            return &x;
        }
        float length() const
        {
            return std::sqrt(x * x + y * y);
        }
        Vector2f normalize() const
        {
            float len = length();
            if (len == 0)
                return Vector2f(0, 0);
            return Vector2f(x / len, y / len);
        }
        bool operator==(const Vector2f &other) const
        {
            return (x == other.x && y == other.y);
        }
        bool operator!=(const Vector2f &other) const
        {
            return !(*this == other);
        }
        std::string toString() const
        {
            return "Vector2f(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };

    struct Vector3f
    {
        float x, y, z;

        Vector3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
        Vector3f(const Vector3f &other) : x(other.x), y(other.y), z(other.z) {}

        Vector3f operator+(const Vector3f &other) const
        {
            return Vector3f(x + other.x, y + other.y, z + other.z);
        }

        Vector3f operator-(const Vector3f &other) const
        {
            return Vector3f(x - other.x, y - other.y, z - other.z);
        }

        Vector3f operator-() const
        {
            return Vector3f(-x, -y, -z);
        }

        Vector3f operator*(float scalar) const
        {
            return Vector3f(x * scalar, y * scalar, z * scalar);
        }

        Vector3f operator/(float scalar) const
        {
            return Vector3f(x / scalar, y / scalar, z / scalar);
        }

        operator const float *() const
        {
            return &x;
        }

        const float *data() const
        {
            return &x;
        }

        float dot(const Vector3f &other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }
        Vector3f cross(const Vector3f &other) const
        {
            return Vector3f(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
        }
        float length() const
        {
            return sqrt(x * x + y * y + z * z);
        }
        Vector3f normalize() const
        {
            float len = length();
            if (len == 0)
                return Vector3f(0, 0, 0);
            return Vector3f(x / len, y / len, z / len);
        }
        bool operator==(const Vector3f &other) const
        {
            return (x == other.x && y == other.y && z == other.z);
        }
        bool operator!=(const Vector3f &other) const
        {
            return !(*this == other);
        }
        std::string toString() const
        {
            return "Vector3f(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        }
    };

} // namespace grn
