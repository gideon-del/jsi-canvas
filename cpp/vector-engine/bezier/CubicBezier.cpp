#include "CubicBezier.h"

Vec2 CubicBezier::evaluate(double t) const
{
    double mt = 1.0 - t;
    double mt2 = mt * mt;
    double mt3 = mt2 * mt;
    double t2 = t * t;
    double t3 = t2 * t;
    return p0 * mt3 +
           p1 * (3.0 * mt2 * t) +
           p2 * (3.0 * mt * t2) +
           p3 * t3;
}

Vec2 CubicBezier::evaluateDeCasteljau(double t) const
{
    Vec2 q0 = p0.lerp(p1, t);
    Vec2 q1 = p1.lerp(p2, t);
    Vec2 q2 = p2.lerp(p3, t);

    Vec2 r1 = q0.lerp(q1, t);
    Vec2 r2 = q1.lerp(q2, t);

    return r1.lerp(r2, t);
}

Vec2 CubicBezier::evaluateQuadratic(Vec2 p0, Vec2 p1, Vec2 p2, double t)
{
    double mt = 1.0 - t;
    return p0 * (mt * mt) + p1 * (2.0 * mt * t) + p2 * (t * t);
}