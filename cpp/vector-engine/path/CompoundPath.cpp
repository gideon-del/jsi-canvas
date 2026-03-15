#include "CompoundPath.h"

void CompoundPath::addSubPath(Path p)
{
    subpaths_.push_back(p);
}

void CompoundPath::removeSubPath(size_t index)
{
    subpaths_.erase(subpaths_.begin() + index);
}

size_t CompoundPath::subpathCount() const { return subpaths_.size(); }
Path &CompoundPath::subpathAt(size_t index) { return subpaths_[index]; }
const Path &CompoundPath::subpathAt(size_t index) const { return subpaths_[index]; }

void CompoundPath::setFillRule(FillRule rule)
{
    fillRule_ = rule;
}

FillRule CompoundPath::fillRule() const { return fillRule_; }
Rect CompoundPath::bounds() const
{

    if (subpaths_.empty())
        return {0, 0, 0, 0};
    Rect result = subpaths_[0].bounds();

    for (size_t i = 1; i < subpaths_.size(); i++)
    {
        Rect pathRect = subpaths_[i].bounds();
        result = result.united(pathRect);
    }

    return result;
}
int CompoundPath::windingNumber(Vec2 point) const
{
    int winding = 0;

    for (const auto &path : subpaths_)
    {
        winding += pathWindingNumber(path, point);
    }
    return winding;
}

bool CompoundPath::contains(Vec2 point) const
{
    int windingCount = windingNumber(point);

    if (fillRule_ == FillRule::NonZero)
    {
        return windingCount != 0;
    }
    else
    {
        return (windingCount % 2) != 0;
    }
}
int CompoundPath::pathWindingNumber(const Path &path, Vec2 point)
{
    if (!path.isClosed() || path.pointCount() < 2)
        return 0;

    int winding = 0;
    auto points = path.flatten(0.5);

    for (size_t i = 0; i < points.size(); i++)
    {
        Vec2 p1 = points[i];
        Vec2 p2 = points[(i + 1) % points.size()];

        if (p1.y <= point.y)
        {
            if (p2.y > point.y)
            {
                if (isLeft(p1, p2, point) > 0)
                {
                    winding++;
                }
            }
        }
        else
        {
            if (p2.y <= point.y)
            {
                if (isLeft(p1, p2, point) < 0)
                {
                    winding--;
                }
            }
        }
    }
    return winding;
}

double CompoundPath::isLeft(Vec2 p1, Vec2 p2, Vec2 point)
{

    return (p2.x - p1.x) * (point.y - p1.y) - (point.x - p1.x) * (p2.y - p1.y);
}