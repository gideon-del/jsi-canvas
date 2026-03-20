#pragma once
#include "../math/vec2.h"
#include "../math/Primitives.h"
#include <optional>
struct ClosestPointResult
{
    double t;
    Vec2 point;
    double distance;
};
class CubicBezier
{

public:
    Vec2 p0, p1, p2, p3;
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

    // subDivision

    std::pair<CubicBezier, CubicBezier> splitAt(double t) const;
    double flatness() const;
    // Adaptive flattening — convert to polyline
    std::vector<Vec2> flatten(double tolerance = 0.5) const;
    std::vector<CubicBezier> subdivide(int n) const;
    double arcLength() const;
    double arcLengthBetween(double a, double b) const;
    double arcLengthAt(double t) const;
    double tAtLength(double targetLen) const;
    std::vector<double> findInflectionPoints() const;
    std::optional<std::pair<double, double>> selfIntersection() const;
    ClosestPointResult closestPoint(Vec2 target) const;

private:
    void flattenRecursive(std::vector<Vec2> &result, double tolerance) const;
};