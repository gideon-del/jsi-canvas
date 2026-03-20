#include "BezierClipper.h"

std::vector<Intersection> BezierClipper::findIntersections(const CubicBezier c1,
                                                           const CubicBezier c2,
                                                           double tolerance)
{
    std::vector<Intersection> results;
    findIntersectionsRecursive(c1, 0, 1, c2, 0, 1, tolerance, results);
    std::sort(results.begin(), results.end(),
              [](const Intersection &a, const Intersection &b)
              {
                  return a.t1 < b.t1;
              });

    results.erase(std::unique(results.begin(), results.end(),
                              [](const Intersection &a, const Intersection &b)
                              {
                                  return std::abs(a.t1 - b.t1) < 1e-4 &&
                                         std::abs(a.t2 - b.t2) < 1e-4;
                              }),
                  results.end());
    return results;
}
void BezierClipper::findIntersectionsRecursive(const CubicBezier &c1, double t1min, double t1max,
                                               const CubicBezier &c2, double t2min, double t2max,
                                               double tolerance,
                                               std::vector<Intersection> &results)
{
    CubicBezier sub1 = extractSubcurve(c1, t1min, t1max);
    CubicBezier sub2 = extractSubcurve(c2, t2min, t2max);

    if (!sub1.boundingBox().intersects(sub2.boundingBox()))
    {
        return;
    }

    double size1 = sub1.boundingBox().width + sub1.boundingBox().height;
    double size2 = sub2.boundingBox().width + sub2.boundingBox().height;

    if (size1 < tolerance && size2 < tolerance)
    {
        double t1 = (t1min + t1max) / 2;
        double t2 = (t2min + t2max) / 2;

        Vec2 p1 = c1.evaluate(t1);
        Vec2 p2 = c2.evaluate(t2);

        results.push_back({t1, t2, (p1 + p2) * 0.5});

        return;
    }

    if (size1 > size2)
    {
        double tmid = (t1min + t1max) / 2;
        findIntersectionsRecursive(c1, t1min, tmid, c2, t2min, t2max, tolerance, results);
        findIntersectionsRecursive(c1, tmid, t1max, c2, t2min, t2max, tolerance, results);
    }
    else
    {
        double tmid = (t2min + t2max) / 2;
        findIntersectionsRecursive(c1, t1min, t1max, c2, t2min, tmid, tolerance, results);
        findIntersectionsRecursive(c1, t1min, t1max, c2, tmid, t2max, tolerance, results);
    }
}
CubicBezier BezierClipper::extractSubcurve(const CubicBezier &c, double t0, double t1)
{
    if (t0 == 0 && t1 == 1)
        return c;
    auto [left, right] = c.splitAt(t1);

    if (t0 == 0)
        return left;

    double newT0 = t0 / t1;

    auto [leftLeft, leftRight] = left.splitAt(newT0);

    return leftRight;
}
