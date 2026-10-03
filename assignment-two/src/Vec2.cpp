#include "Vec2.hpp"
#include <cmath>

Vec2::Vec2()
{
    this->x = 0.0f;
    this->y = 0.0f;
};

Vec2::Vec2(const float x, const float y)
{
    this->x = x;
    this->y = y;
};

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
};

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
};

Vec2 Vec2::operator*(const float scalar) const
{
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(const float scalar) const
{
    return Vec2(x / scalar, y / scalar);
};

Vec2 &Vec2::operator+=(const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
};

Vec2 &Vec2::operator-=(const Vec2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
};

Vec2 &Vec2::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
};

Vec2 &Vec2::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
};

bool Vec2::operator==(const Vec2 &rhs) const
{
    return x == rhs.x && y == rhs.y;
};

bool Vec2::operator!=(const Vec2 &rhs) const
{
    return !(*this == rhs);
};

float Vec2::length() const
{
    return sqrt(x * x + y * y);
};

Vec2 Vec2::normalize() const
{
    float len = this->length();

    if (len == 0.0f)
    {
        return Vec2();
    }

    return Vec2(x / len, y / len);
};

inline Vec2 operator*(float scalar, const Vec2& vector)
{
    return vector * scalar;
}
