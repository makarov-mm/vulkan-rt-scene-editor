#include "vec3.h"

float Vec3::dot(const Vec3& v) const noexcept
{
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const noexcept
{
    return {
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    };
}

Vec3 Vec3::normalize() const noexcept
{
    float l = std::sqrt(this->dot(*this));
    return l > 0 ? Vec3{ x / l, y / l, z / l } : *this;
}
