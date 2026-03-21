#pragma once
#include "../math/vec2.h"
#include <iostream>
struct Transform
{
    // 2D affine transform: [a b tx]
    //                      [c d ty]
    //                      [0 0 1 ]
    double a, b, c, d, tx, ty;
    Transform() : a(1), b(0), c(0), d(1), tx(0), ty(0) {}
    Transform(double a, double b, double c, double d, double tx, double ty)
        : a(a), b(b), c(c), d(d), tx(tx), ty(ty) {}
    static Transform identity() { return {}; }

    static Transform translate(double x, double y)
    {
        return {1, 0, 0, 1, x, y};
    }

    static Transform scale(double sx, double sy)
    {
        return {sx, 0, 0, sy, 0, 0};
    }

    static Transform rotate(double radians)
    {
        double cos = std::cos(radians);
        double sin = std::sin(radians);

        return {cos, -sin, sin, cos, 0, 0};
    }
    static Transform skewX(double radians)
    {
        return {1, std::tan(radians), 0, 1, 0, 0};
    }
    static Transform skewY(double radians)
    {
        return {1, 0, std::tan(radians), 1, 0, 0};
    }

    Transform operator*(const Transform &o) const
    {
        return {
            a * o.a + b * o.c,
            a * o.b + b * o.d,
            c * o.a + d * o.c,
            c * o.b + d * o.d,
            a * o.tx + b * o.ty + tx,
            c * o.tx + d * o.ty + ty,
        };
    }

    Vec2 apply(Vec2 p) const
    {
        return {
            a * p.x + b * p.y + tx,
            c * p.x + d * p.y + ty};
    }

    // Apply to vector (no translation)
    Vec2 applyToVector(Vec2 v) const
    {
        return {a * v.x + b * v.y, c * v.x + d * v.y};
    }

    Transform inverse() const
    {
        double det = a * d - b * c;

        if (std::abs(det) < 1e-10)
        {
            return identity(); // Singular matrix
        }

        double invDet = 1.0 / det;
        return {
            d * invDet,
            -b * invDet,
            -c * invDet,
            a * invDet,
            (b * ty - d * tx) * invDet,
            (c * tx - a * ty) * invDet};
    }
};