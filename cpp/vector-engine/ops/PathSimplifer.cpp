#include "PathSimplifier.h"
#include "../math/Primitives.h"

std::vector<Vec2> PathSimplifier::simplifyPolyline(std::vector<Vec2> points, double epsilon)
{
    if (points.size() < 3)
        return points;

    Vec2 start = points.front();
    Vec2 end = points.back();
    Line line(start, (end - start).normalized());

    size_t maxIdx = 0;
    double maxDist = 0;

    for (size_t i = 1; i < points.size(); i++)
    {
        double dist = line.distanceToPoint(points[i]);
        if (dist > maxDist)
        {
            maxDist = dist;
            maxIdx = i;
        }
    }

    if (maxDist > epsilon)
    {
        std::vector<Vec2> left(points.begin(), points.begin() + maxIdx + 1);
        std::vector<Vec2> right(points.begin() + maxIdx, points.end());

        auto leftPoints = simplifyPolyline(left, epsilon);
        auto rightPoints = simplifyPolyline(right, epsilon);

        leftPoints.insert(leftPoints.end(), rightPoints.begin() + 1, rightPoints.end());

        return leftPoints;
    }
    else
    {
        return {start, end};
    }
}