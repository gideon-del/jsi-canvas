#include "PathOffsetter.h"

Path PathOffsetter::offset(const Path &path, double distance, JoinStyle join, double miterLimit)
{
    Path result;
    for (size_t i = 0; i < path.segmentCount(); i++)
    {
        auto seg = path.getSegment(i);
        // Look for neighboring points withing the specified distance in the segment
        offsetSegment(seg, distance, result);

        if (i < path.segmentCount() - 1 || path.isClosed())
        {
            size_t nextIdx = (i + 1) % path.segmentCount();
            auto nextSeg = path.getSegment(nextIdx);

            addJoin(seg, nextSeg, distance, join, miterLimit, result);
        }
    }

    return result;
}

CompoundPath PathOffsetter::stroke(const Path &path, double width, JoinStyle join, CapStyle cap)
{
    double halfWidth = width / 2;
    Path outer = offset(path, halfWidth, join);

    Path inner = offset(path, -halfWidth, join);
    inner.reverse();

    CompoundPath result;

    if (path.isClosed())
    {
        result.addSubPath(outer);
        result.addSubPath(inner);
    }
    else
    {
        Vec2 startTangent = path.getSegment(0)
                                .tangentAt(0.0);
        Vec2 endTangent = path.getSegment(path.segmentCount() - 1)
                              .tangentAt(1.0);

        Path combined;
        for (size_t i = 0; i < outer.pointCount(); i++)
        {
            combined.addPoint(outer.pointAt(i));
        }

        addCap(path.pointAt(path.pointCount() - 1), endTangent, halfWidth, cap, false, combined);

        for (size_t i = 0; i < inner.pointCount(); i++)
        {
            combined.addPoint(inner.pointAt(i));
        }

        addCap(path.pointAt(0), startTangent, halfWidth, cap, true, combined);
        combined.close();
        result.addSubPath(combined);
    }

    return result;
}

void PathOffsetter::offsetSegment(const Segment &seg, double dist, Path &result)
{
    const int samples = 10;

    for (int i = 0; i <= samples; i++)
    {
        double t = static_cast<double>(i) / samples;
        Vec2 p = seg.pointAt(t);
        Vec2 n = seg.normalAt(t);

        Vec2 offset = p + n * dist;

        if (i == 0 && result.pointCount() == 0)
        {
            result.addPoint({{0, 0}, {0, 0}, offset});
        }
        else if (i == samples)
        {
            result.addPoint({{0, 0}, {0, 0}, offset});
        }
        else
        {
            result.addPoint({{0, 0}, {0, 0}, offset, PointType::Smooth});
        }
    }
}

void PathOffsetter::addJoin(const Segment &seg1, const Segment &seg2, double dist, JoinStyle style, double miterLimit, Path &result)
{
    Vec2 n1 = seg1.normalAt(1.0);
    Vec2 n2 = seg2.normalAt(0.0);

    Vec2 p = seg1.end().position;
    Vec2 o1 = p + n1 * dist;
    Vec2 o2 = p + n2 * dist;

    double angle = std::acos(std::clamp(n1.dot(n2), -1.0, 1.0));
    bool isOutside = n1.cross(n2) * dist > 0;

    if (!isOutside || angle < 1e-10)
    {

        result.addPoint({{0, 0}, {0, 0}, o2});
        return;
    }

    switch (style)
    {
    case JoinStyle::Bevel:
        break;
    case JoinStyle::Miter:
    {
        double miterLen = dist / std::cos(angle / 2);
        if (miterLen <= dist * miterLimit)
        {
            Vec2 miterDir = (n1 + n2).normalized();
            Vec2 miterPt = p + miterDir * miterLen;
            result.addPoint({{0, 0}, {0, 0}, miterPt});
            return;
        }
        break;
    }
    case JoinStyle::Round:
    {
        addArc(p, o1, o2, dist, result, -1);
        return;
    }
    }

    result.addPoint({{0, 0}, {0, 0}, o2});
}
Vec2 PathOffsetter::rotate(Vec2 v, double angle)
{
    double c = std::cos(angle), s = std::sin(angle);
    return {
        v.x * c - v.y * s,
        v.x * s + v.y * c};
}
void PathOffsetter::addArc(Vec2 center, Vec2 start, Vec2 end,
                           double radius, Path &result, int sweepSign)
{

    Vec2 s = start - center;
    Vec2 e = end - center;

    double crossZ = s.cross(e);
    double dot = std::clamp(s.normalized().dot(e.normalized()), -1.0, 1.0);
    double absAngle = std::acos(dot);
    double totalAngle;
    if (sweepSign != 0 && std::abs(crossZ) < 1e-9 * radius * radius)
    {

        totalAngle = sweepSign * absAngle;
    }
    else
    {
        totalAngle = std::atan2(crossZ, s.dot(e));
    }
    int numSegs = std::max(6, (int)std::ceil(std::abs(totalAngle) / (M_PI / 2.0)));
    double segAngle = totalAngle / numSegs;

    double k = (4.0 / 3.0) * std::tan(segAngle / 4.0);

    Vec2 curDir = s.normalized();
    Vec2 curPt = center + curDir * radius;

    for (int i = 0; i < numSegs; i++)
    {

        Vec2 curTan = curDir.perpendicular();
        Vec2 cp1 = curPt + curTan * (radius * k);

        Vec2 nextDir = rotate(curDir, segAngle);
        Vec2 nextPt = center + nextDir * radius;

        Vec2 nextTan = Vec2(-nextDir.y, nextDir.x);
        Vec2 cp2 = nextPt - nextTan * (radius * k);

        if (result.pointCount() > 0)
        {
            PathPoint lastPoint = result.pointAt(result.pointCount() - 1);
            lastPoint.setHandleOut(cp1 - curPt);
            result.removePoint(result.pointCount() - 1);
            result.addPoint(lastPoint);
        }
        result.addPoint({cp2 - nextPt, {0, 0}, nextPt, PointType::Smooth});

        curDir = nextDir;
        curPt = nextPt;
    }
}

void PathOffsetter::addCap(const PathPoint &point, Vec2 tangent, double radius,
                           CapStyle style, bool isStart, Path &result)
{

    Vec2 normal = tangent.perpendicular();

    // The two stroke-edge points at this endpoint.
    Vec2 outerEdge = point.position + normal * radius;
    Vec2 innerEdge = point.position + normal * -radius;

    // Direction the cap face points (away from the stroke body).
    Vec2 outward = isStart ? tangent * -1 : tangent;

    switch (style)
    {
    case CapStyle::Butt:
        // No extra geometry required.
        break;

    case CapStyle::Square:
    {
        // Project each edge forward by radius, producing a rectangle.
        Vec2 proj = outward * radius;
        if (isStart)
        {
            // path: innerEdge → innerEdge+proj → outerEdge+proj → (close) → outerEdge
            result.addPoint({{0, 0}, {0, 0}, innerEdge + proj});
            result.addPoint({{0, 0}, {0, 0}, outerEdge + proj});
        }
        else
        {
            // path: outerEdge → outerEdge+proj → innerEdge+proj → innerEdge
            result.addPoint({{0, 0}, {0, 0}, outerEdge + proj});
            result.addPoint({{0, 0}, {0, 0}, innerEdge + proj});
        }
        break;
    }

    case CapStyle::Round:
        // Semicircle: sweep outerEdge → innerEdge (or vice-versa for start)
        // so the arc always bulges *outward* from the path body.
        if (isStart)
        {
            addArc(point.position, innerEdge, outerEdge, radius, result, -1);
        }
        else
        {
            addArc(point.position, outerEdge, innerEdge, radius, result, -1);
        }
        break;
    }
}
