#pragma once
#include "../math/vec2.h"
#include "../math/Primitives.h"
class CubicBezier
{
    Vec2 p0, p1, p2, p3;

public:
    CubicBezier(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3) : p0(p0), p1(p1), p2(p2), p3(p3) {};
    // Bernstein Form
    Vec2 evaluate(double t) const;
    // De Casteljau's algorithm
    Vec2 evaluateDeCasteljau(double t) const;
    static Vec2 evaluateQuadratic(Vec2 p0, Vec2 p1, Vec2 p2, double t);

    // First derivative — returns velocity vector at t
    Vec2 derivative(double t) const;

    // Tangent — normalized derivative
    Vec2 tangentAt(double t) const;

    // Normal — perpendicular to tangent
    Vec2 normalAt(double t) const;

    // Second derivative — for curvature
    Vec2 secondDerivative(double t) const;

    // Curvature — how sharply the curve bends
    double curvatureAt(double t) const;

    // Radius of curvature (1/curvature)
    double radiusOfCurvatureAt(double t) const;

    Rect boundingBox() const;
    Rect controlPointBounds() const;
};