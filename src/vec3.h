#pragma once
#include <cmath>

struct Vec3 
{
    float x = 0, y = 0, z = 0;

    constexpr Vec3() = default;
    constexpr Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) { }

    float dot(const Vec3& v) const noexcept;
    Vec3 cross(const Vec3& v) const noexcept;
    Vec3 normalize() const noexcept;
};

constexpr Vec3 operator+(const Vec3& a, const Vec3& b) noexcept
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

constexpr Vec3 operator-(const Vec3& a, const Vec3& b) noexcept
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

constexpr Vec3 operator*(const Vec3& a, float s) noexcept
{
	return { a.x * s, a.y * s, a.z * s };
}
