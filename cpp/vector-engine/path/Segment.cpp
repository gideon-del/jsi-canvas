#include "Segment.h"

const PathPoint &Segment::start() const { return *start_; }
const PathPoint &Segment::end() const { return *end_; }

bool Segment::isLine() const
{
    return !start_->hasHandleOut() && !end_->hasHandleIn();
}

CubicBezier Segment::toCubicBezier() const
{
    return CubicBezier(start_->position,
                       start_->absoluteHandleOut(),
                       end_->absoluteHandleIn(),
                       end_->position);
}

Vec2 Segment::pointAt(double t) const
{
    if (isLine())
    {
        return start_->position.lerp(end_->position, t);
    }

    return toCubicBezier().evaluate(t);
}

Vec2 Segment::tangentAt(double t) const
{
    if (isLine())
    {
        return (end_->position - start_->position).normalized();
    }
    return toCubicBezier().tangentAt(t);
}

Vec2 Segment::normalAt(double t) const
{
    return tangentAt(t).perpendicular();
}

Rect Segment::boundingBox() const
{
    if (isLine())
    {
        return Rect::fromPoints(start_->position, end_->position);
    }

    return toCubicBezier().boundingBox();
}

double Segment::length() const
{
    if (isLine())
    {
        return start_->position.distanceTo(end_->position);
    }
    // TODO
    // return toCubicBezier().arcLength();
    return 0.0;
}