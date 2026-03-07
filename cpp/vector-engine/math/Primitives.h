#pragma once
#include "vec2.h"
#include <optional>
#include <utility>
struct Line
{
    Vec2 origin;
    Vec2 direction;

    Line(Vec2 o, Vec2 d) : origin(o), direction(d.normalized()) {}

    Vec2 pointAt(double t) { return origin + direction * t; }

    double distanceToPoint(const Vec2 &p)
    {
        Vec2 toPoint = p - origin;

        return std::abs(toPoint.cross(direction));
    }

    std::optional<std::pair<double, double>> intersect(const Line &other)
    {
        double denom = direction.cross(other.direction);
        if (std::abs(denom) < 1e-10)
        {
            return std::nullopt;
        }
        Vec2 diff = other.origin - origin;
        double t1 = diff.cross(other.direction) / denom;
        double t2 = diff.cross(direction) / denom;

        return std::make_pair(t1, t2);
    }
};

struct Rect
{
    double x, y, width, height;
    double left() const { return x; }
    double right() const { return x + width; }
    double top() const { return y; }
    double bottom() const { return y + height; }

    bool contains(const Vec2 &p) const
    {
        return p.x >= x && p.x <= x + width && p.y >= y && p.y <= y + height;
    }
    bool intersects(const Rect &o) const
    {
        return !(o.right() < left() || o.left() > right() || o.bottom() < top() || o.top() > bottom());
    }

    Rect united(const Rect &o)
    {
        auto l = std::min(o.left(), left());
        auto t = std::min(o.top(), top());
        auto b = std::max(o.bottom(), bottom());
        auto r = std::max(o.right(), right());

        return {l, t, r - l, b - t};
    }

    static Rect fromPoints(Vec2 a, Vec2 b)
    {
        return {
            std::min(a.x, b.x), std::min(a.y, b.y),
            std::abs(b.x - a.x), std::abs(b.y - a.y)};
    }
};