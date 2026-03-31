#pragma once
#include "../bezier/CubicBezier.h"
#include "../path/CompoundPath.h"
#include "../path/PathIntersector.h"
enum class BooleanOp
{
    XOR,
    UNION,
    DIFFERENCE,
    INTERSECTION
};

struct SplitSegment
{
    CubicBezier curve;

    size_t originalPath;
    size_t segmentIndex;

    double tStart, tEnd;

    bool inA = false;
    bool inB = false;

    Vec2 startPoint() const { return curve.evaluate(0); }
    Vec2 endPoint() const { return curve.evaluate(1); }
};

class BooleanOperator
{
public:
    static CompoundPath execute(const Path &pathA, const Path &pathB, BooleanOp operation);

private:
    static std::vector<SplitSegment> splitPath(const Path &pathA, const Path &pathB, std::vector<PathIntersection> &intersections);
    static void splitSegment(const Segment &seg, size_t pathIdx, size_t segIdx,
                             std::vector<double> &splits,
                             std::vector<SplitSegment> &result);
    static void labelSegment(const Path &pathA, const Path &pathB, std::vector<SplitSegment> &result);
    static std::vector<SplitSegment> selectSegment(const std::vector<SplitSegment> &segments,
                                                   BooleanOp op);
    static CompoundPath assembleResult(const std::vector<SplitSegment> &segments, BooleanOp operation);
};