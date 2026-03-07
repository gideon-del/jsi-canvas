#pragma once
#include <iostream>
struct Vec2
{
    double x, y;

    Vec2() : x(0), y(0) {};
    Vec2(double x, double y) : x(x), y(y) {};

    // Operator overloading
    Vec2 operator+(const Vec2 &o) const { return {x + o.x, y + o.y}; };
    Vec2 operator-(const Vec2 &o) const { return {x - o.x, y - o.y}; };
    Vec2 operator*(double s) const { return {x * s, y * s}; };
    Vec2 operator/(double s) const { return {x / s, y / s}; };

    double dot(const Vec2 &o) const { return x * o.x + y * o.y; }
    double cross(const Vec2 &o) const { return x * o.y - y * o.x; }

    double lengthSquared() const { return x * x + y * y; }
    double length() const { return std::sqrt(lengthSquared()); }

    Vec2 normalized() const
    {
        double len = length();
        return len > 0 ? Vec2{x / len, y / len} : Vec2{0, 0};
    }

    double distanceTo(const Vec2 &o) const { return (*this - o).length(); }
    double angleTo(const Vec2 &o) const { return std::atan2(cross(o), dot(o)); }
    Vec2 perpendicular() const { return {-y, x}; };
    Vec2 lerp(const Vec2 o, double t) const { return *this + (o - *this) * t; }
};
