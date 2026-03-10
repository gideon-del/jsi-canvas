#pragma once
#include "../math/vec2.h"
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
};