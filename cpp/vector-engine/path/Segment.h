#pragma once
#include "./PathPoint.h"
#include "../bezier/CubicBezier.h"
class Segment
{
    const PathPoint *start_;
    const PathPoint *end_;

public:
    Segment(const PathPoint *start, const PathPoint *end) : start_(start), end_(end) {}
    const PathPoint &start();
    const PathPoint &end();
    bool isLine() const;

    CubicBezier toCubicBezier() const;

    // Delegate to bezier for operations
    Vec2 pointAt(double t) const;

    Vec2 tangentAt(double t) const;
    Vec2 normalAt(double t) const;

    Rect boundingBox() const;

    double length() const;
};