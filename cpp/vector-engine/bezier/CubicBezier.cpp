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

Vec2 CubicBezier::derivative(double t) const
{
    double mt = 1.0 - t;
    Vec2 d0 = p1 - p0;
    Vec2 d1 = p2 - p1;
    Vec2 d2 = p3 - p2;
    return d0 * (3.0 * mt * mt) +
           d1 * (6 * mt * t) +
           d2 * (3.0 * t * t);
}

Vec2 CubicBezier::tangentAt(double t) const
{
    Vec2 d = derivative(t);
    double len = d.length();
    if (len < 1e-10)
    {
        if (t < 0.5)
            return tangentAt(t + 0.01);
        else
            return tangentAt(t - 0.01);
    }

    return d / len;
}

Vec2 CubicBezier::normalAt(double t) const
{
    return tangentAt(t).perpendicular();
}

Vec2 CubicBezier::secondDerivative(double t) const
{
    Vec2 d0 = p1 - p0;
    Vec2 d1 = p2 - p1;
    Vec2 d2 = p3 - p2;

    Vec2 dd0 = d1 - d0;
    Vec2 dd1 = d2 - d1;

    return dd0 * (6.0 * (1.0 - t)) + dd1 * (6.0 * t);
}

double CubicBezier::curvatureAt(double t) const
{
    Vec2 d1 = derivative(t);
    Vec2 d2 = secondDerivative(t);

    double cross = d1.cross(d2);

    double lenCubed = std::pow(d1.length(), 3);

    if (lenCubed < 1e-10)
        return 0;
    return cross / lenCubed;
}

double CubicBezier::radiusOfCurvatureAt(double t) const
{
    double k = curvatureAt(t);
    if (std::abs(k) < 1e-10)
        return std::numeric_limits<double>::infinity();

    return 1.0 / k;
}

std::vector<double> solveQuadraticInUnitInterval(double a, double b, double c)
{
    std::vector<double> roots;
    if (std::abs(a) < 1e-10)
    {
        // bt + c = 0
        if (std::abs(b) > 1e-10)
        {
            double t = -c / b;
            if (t > 0 && t < 1)
                roots.push_back(t);
        }
        return roots;
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return roots;

    double sqrtD = std::sqrt(discriminant);

    double t1 = (-b - sqrtD) / (2 * a);
    double t2 = (-b + sqrtD) / (2 * a);
    if (t1 > 0 && t1 < 1)
        roots.push_back(t1);
    if (t2 > 0 && t2 < 1 && std::abs(t2 - t1) > 1e-10)
        roots.push_back(t2);

    return roots;
}

Rect CubicBezier::boundingBox() const
{
    double minX = std::min(p0.x, p3.x);
    double maxX = std::max(p0.x, p3.x);
    double minY = std::min(p0.y, p3.y);
    double maxY = std::max(p0.y, p3.y);

    // Find extrema by solving B'(t) = 0 for each axis
    // B'(t) = at² + bt + c where:
    // a = 3(-P0 + 3P1 - 3P2 + P3)
    // b = 6(P0 - 2P1 + P2)
    // c = 3(P1 - P0)

    auto findExtrema = [this](double p0c, double p1c, double p2c, double p3c)
    {
        double a = 3.0 * (-p0c + 3 * p1c - 3 * p2c + p3c);
        double b = 6.0 * (p0c - 2 * p1c + p2c);
        double c = 3.0 * (p1c - p0c);

        return solveQuadraticInUnitInterval(a, b, c);
    };

    // Checking for x extrema
    for (double t : findExtrema(p0.x, p1.x, p2.x, p3.x))
    {
        double x = evaluate(t).x;
        minX = std::min(x, minX);
        maxX = std::max(x, maxX);
    }

    // Checking for y extrema
    for (double t : findExtrema(p0.y, p1.y, p2.y, p3.y))
    {
        double y = evaluate(t).y;
        minY = std::min(y, minY);
        maxY = std::max(y, maxY);
    }

    return {minX, minY, maxX - minX, maxY - minY};
}
Rect CubicBezier::controlPointBounds() const
{
    double minX = std::min({p0.x, p1.x, p2.x, p3.x});
    double maxX = std::max({p0.x, p1.x, p2.x, p3.x});
    double minY = std::min({p0.y, p1.y, p2.y, p3.y});
    double maxY = std::max({p0.y, p1.y, p2.y, p3.y});
    return {minX, minY, maxX - minX, maxY - minY};
}