#include "Path.h"

size_t Path::pointCount() const { return points_.size(); }
PathPoint &Path::pointAt(size_t i) { return points_[i]; }
const PathPoint &Path::pointAt(size_t i) const { return points_[i]; }

void Path::addPoint(PathPoint p)
{
    points_.push_back(p);
}

void Path::insertPoint(size_t index, PathPoint p)
{
    points_.insert(points_.begin() + index, p);
}

void Path::removePoint(size_t index)
{
    points_.erase(points_.begin() + index);
}

void Path::close()
{
    closed_ = true;
}

void Path::open()
{
    closed_ = false;
}

bool Path::isClosed() const { return closed_; }

size_t Path::segmentCount() const
{
    if (points_.size() < 2)
        return 0;

    return closed_ ? points_.size() : points_.size() - 1;
}

Segment Path::getSegment(size_t index) const
{
    size_t nextIndex = (index + 1) % points_.size();

    return Segment(&points_[index], &points_[nextIndex]);
}
template <typename Func>
void Path::forEachSegment(Func &&fn) const
{
    for (size_t i = 0; i < segmentCount(); i++)
    {
        fn(getSegment(i), i);
    }
}

void Path::reverse()
{
    std::reverse(points_.begin(), points_.end());

    for (auto &p : points_)
    {
        std::swap(p.handleIn, p.handleOut);
    }
}

Rect Path::bounds() const
{
    if (points_.empty())
        return {0, 0, 0, 0};

    Rect result = getSegment(0).boundingBox();
    for (size_t i = 1; i < segmentCount(); i++)
    {
        result = result.united(getSegment(i).boundingBox());
    }
    return result;
}

double Path::length() const
{
    double total = 0;

    forEachSegment([&](const Segment &seg, size_t)
                   { total += seg.length(); });

    return total;
};

Vec2 Path::pointAtLength(double len) const
{
    double accumulated = 0;
    for (size_t i = 0; i < segmentCount(); i++)
    {
        Segment seg = getSegment(i);
        double segLen = seg.length();

        if (accumulated + segLen >= len)
        {
            double t = (len - accumulated) / segLen;
            return seg.pointAt(t);
        }

        accumulated += segLen;
    }

    return points_.back().position;
}

std::vector<Vec2> Path::flatten(double tolerance) const
{
    std::vector<Vec2> result;
    if (segmentCount() == 0 || !isClosed())
        return result;

    for (size_t i = 0; i < segmentCount(); ++i)
    {
        auto segmentPoints = getSegment(i).toCubicBezier().flatten(tolerance);
        result.insert(result.end(), segmentPoints.begin(), segmentPoints.end());
    }

    return result;
}

ClosestPointResult Path::closestPoint(Vec2 target) const
{
    ClosestPointResult best = {0, points_[0].position, target.distanceTo(points_[0].position)};

    for (size_t i = 0; i < segmentCount(); i++)
    {
        Segment seg = getSegment(i);
        auto result = seg.toCubicBezier().closestPoint(target);

        if (result.distance < best.distance)
        {
            best = {
                static_cast<double>(i) + result.t,
                result.point,
                result.distance,
            };
        }
    }
    return best;
}