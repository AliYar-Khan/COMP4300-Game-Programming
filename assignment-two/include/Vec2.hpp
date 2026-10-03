#pragma once

class Vec2
{

public:
    float x;
    float y;

    Vec2();
    Vec2(const float x, const float y);

    Vec2 operator+(const Vec2 &rhs) const;
    Vec2 operator-(const Vec2 &rhs) const;
    Vec2 operator/(const float scalar) const;
    Vec2 operator*(const float scalar) const;

    Vec2 &operator+=(const Vec2 &rhs);
    Vec2 &operator-=(const Vec2 &rhs);
    Vec2 &operator/=(const float scalar);
    Vec2 &operator*=(const float scalar);

    bool operator==(const Vec2 &rhs) const;

    bool operator!=(const Vec2 &rhs) const;

    float length() const;
    Vec2 normalize() const;
    
};