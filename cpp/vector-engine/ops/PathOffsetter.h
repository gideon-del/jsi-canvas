#pragma once
#include "../path/CompoundPath.h"
enum class JoinStyle
{
    Miter,
    Bevel,
    Round,
};
enum class CapStyle
{
    Butt,
    Square,
    Round,
};
class PathOffsetter
{
public:
    static Path offset(const Path &path, double distance,
                       JoinStyle join = JoinStyle::Miter,
                       double miterLimit = 4.0);
    static CompoundPath stroke(const Path &path, double width,
                               JoinStyle join = JoinStyle::Miter,
                               CapStyle cap = CapStyle::Butt);

private:
    static void offsetSegment(const Segment &seg, double dist, Path &result);
    static void addJoin(const Segment &seg1, const Segment &seg2,
                        double dist, JoinStyle style, double miterLimit,
                        Path &result);
    static Vec2 rotate(Vec2 v, double angle);
    static void addCap(const PathPoint &point, Vec2 tangent, double radius,
                       CapStyle style, bool isStart, Path &result);
    static void addArc(Vec2 center, Vec2 start, Vec2 end,
                       double radius, Path &result, int sweepSign = 0);
};