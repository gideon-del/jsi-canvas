#include "PathIntersector.h"
#include "../bezier/BezierClipper.h"
std::vector<PathIntersection> PathIntersector::findIntersections(const Path &path1,
                                                                 const Path &path2)
{
    std::vector<PathIntersection> results;
    std::vector<Rect> path2Bounds;

    for (size_t i = 0; i < path2.segmentCount(); i++)
    {
        path2Bounds.push_back(path2.getSegment(i).boundingBox());
    }

    for (size_t i = 0; i < path1.segmentCount(); i++)
    {
        auto seg1 = path1.getSegment(i);
        Rect bounds1 = seg1.boundingBox();

        for (size_t j = 0; j < path2.segmentCount(); j++)
        {
            if (!bounds1.intersects(path2Bounds[j]))
                continue;
            auto seg2 = path2.getSegment(j);
            auto intersections = BezierClipper::findIntersections(
                seg1.toCubicBezier(),
                seg2.toCubicBezier());
            for (const auto &isect : intersections)
            {
                results.push_back({i, j,
                                   isect.t1, isect.t2,
                                   isect.point});
            }
        }
    }

    return results;
}