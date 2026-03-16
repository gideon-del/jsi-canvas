#pragma once
#include "../math/vec2.h"
enum class PointType
{
    Corner,
    Smooth,
    Symmetric
};

struct PathPoint
{
    Vec2 handleIn;
    Vec2 handleOut;
    Vec2 position;

    PointType type = PointType::Corner;

    Vec2 absoluteHandleIn() const { return position + handleIn; }
    Vec2 absoluteHandleOut() const { return position + handleOut; }

    bool hasHandleIn() const { return handleIn.lengthSquared() > 1e-10; }
    bool hasHandleOut() const { return handleOut.lengthSquared() > 1e-10; }

    void setHandleIn(Vec2 h)
    {
        handleIn = h;
        enforceTypeConstraint(true);
    }
    void setHandleOut(Vec2 h)
    {
        handleOut = h;
        enforceTypeConstraint(false);
    }

    void setType(PointType newType)
    {
        type = newType;
        enforceTypeConstraint(true);
    }

private:
    void enforceTypeConstraint(bool inChanged)
    {
        if (type == PointType::Corner)
            return;

        Vec2 &changed = inChanged ? handleIn : handleOut;
        Vec2 &other = inChanged ? handleOut : handleIn;

        if (changed.lengthSquared() < 1e-10)
            return;

        // Make other handle collinear (opposite direction)
        Vec2 dir = changed.normalized() * -1;

        if (type == PointType::Symmetric)
        {
            // Same length
            other = dir * changed.length();
        }
        else
        {
            other = dir * other.length();
        }
    }
};
