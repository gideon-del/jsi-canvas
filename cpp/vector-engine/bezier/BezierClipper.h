#include "CubicBezier.h"

struct Intersection
{
    double t1;  // parameter on First curve;
    double t2;  // parameter on Second curve
    Vec2 point; // The actual intersection points
};

class BezierClipper
{
public:
    static std::vector<Intersection> findIntersections(
        const CubicBezier c1,
        const CubicBezier c2,
        double tolerance = 1e-6);
    static CubicBezier extractSubcurve(const CubicBezier &c, double t0, double t1);

private:
    static void findIntersectionsRecursive(
        const CubicBezier &c1, double t1min, double t1max,
        const CubicBezier &c2, double t2min, double t2max,
        double tolerance,
        std::vector<Intersection> &results);
};