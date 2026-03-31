#include "BooleanOperator.h"
#include "../bezier/BezierClipper.h"
struct PairHash
{
    size_t operator()(const std::pair<int, size_t> &p) const
    {
        return std::hash<int>()(p.first) ^ (std::hash<size_t>()(p.second) << 1);
    }
};
CompoundPath BooleanOperator::execute(const Path &pathA, const Path &pathB, BooleanOp operation)
{

    if (operation == BooleanOp::XOR)
    {
        CompoundPath aDiffB = execute(pathA, pathB, BooleanOp::DIFFERENCE);
        CompoundPath bDiffA = execute(pathB, pathA, BooleanOp::DIFFERENCE);

        CompoundPath result;
        for (size_t i = 0; i < aDiffB.subpathCount(); i++)
            result.addSubPath(aDiffB.subpathAt(i));
        for (size_t i = 0; i < bDiffA.subpathCount(); i++)
            result.addSubPath(bDiffA.subpathAt(i));

        return result;
    }
    std::vector<PathIntersection> intersections = PathIntersector::findIntersections(pathA, pathB);
    // if (intersections.empty())
    // {
    //     CompoundPath result;
    //     result.addSubPath(pathA);
    //     result.addSubPath(pathB);
    //     return result;
    // }
    std::vector<SplitSegment> segments = splitPath(pathA, pathB, intersections);

    labelSegment(pathA, pathB, segments);
    std::vector<SplitSegment> selected = selectSegment(segments, operation);

    return assembleResult(selected, operation);
}

std::vector<SplitSegment> BooleanOperator::splitPath(const Path &pathA, const Path &pathB, std::vector<PathIntersection> &intersections)
{

    std::unordered_map<std::pair<int, size_t>, std::vector<double>, PairHash> splits;
    std::vector<SplitSegment> result;
    for (auto &isect : intersections)
    {
        splits[{0, isect.path1Segment}].push_back(isect.t1);
        splits[{1, isect.path2Segment}].push_back(isect.t2);
    }

    for (size_t i = 0; i < pathA.segmentCount(); i++)
    {
        splitSegment(pathA.getSegment(i), 0, i, splits[{0, i}], result);
    }
    for (size_t i = 0; i < pathB.segmentCount(); i++)
    {
        splitSegment(pathB.getSegment(i), 1, i, splits[{1, i}], result);
    }

    return result;
}

void BooleanOperator::splitSegment(const Segment &seg, size_t pathIdx, size_t segIdx,
                                   std::vector<double> &splits,
                                   std::vector<SplitSegment> &result)
{
    // Sort t-values
    std::sort(splits.begin(), splits.end());

    // Add Endpoints
    splits.insert(splits.begin(), 0.0);
    splits.push_back(1.0);

    // Erase duplicate points
    splits.erase(std::unique(splits.begin(), splits.end(),
                             [](double a, double b)
                             { return std::abs(a - b) < 1e-9; }),
                 splits.end());
    CubicBezier curve = seg.toCubicBezier();
    for (size_t i = 0; i < splits.size() - 1; i++)
    {
        double t1 = splits[i];
        double t2 = splits[i + 1];

        CubicBezier subCurve = BezierClipper::extractSubcurve(curve, t1, t2);

        result.push_back({
            subCurve,
            pathIdx,
            segIdx,
            t1,
            t2,
        });
    }
}

void BooleanOperator::labelSegment(const Path &pathA, const Path &pathB, std::vector<SplitSegment> &result)
{
    for (auto &splitSeg : result)
    {

        Vec2 midPoint = splitSeg.curve.evaluate(0.5);

        Vec2 normal = splitSeg.curve.normalAt(0.5);

        Vec2 testPoint = midPoint + normal * 0.01;

        splitSeg.inA = CompoundPath::pathWindingNumber(pathA, testPoint) != 0;
        splitSeg.inB = CompoundPath::pathWindingNumber(pathB, testPoint) != 0;
    }
}

std::vector<SplitSegment> BooleanOperator::selectSegment(const std::vector<SplitSegment> &segments,
                                                         BooleanOp op)
{
    std::vector<SplitSegment> result;
    for (const auto &seg : segments)
    {
        bool fromA = seg.originalPath == 0;
        bool select = false;

        switch (op)
        {
        case BooleanOp::UNION:
            if (fromA)
                select = !seg.inB;
            else
                select = !seg.inA;
            break;
        case BooleanOp::INTERSECTION:

            if (fromA)
                select = seg.inB;
            else
                select = seg.inA;
            break;
        case BooleanOp::DIFFERENCE:
            if (fromA)
            {
                select = !seg.inB;
            }
            else
            {
                if (seg.inA)
                {

                    SplitSegment rev = seg;
                    std::swap(rev.curve.p0, rev.curve.p3);
                    std::swap(rev.curve.p1, rev.curve.p2);
                    result.push_back(rev);
                    continue;
                }
            }
            break;
        case BooleanOp::XOR:
            std::cout << seg.inA << "in a " << seg.inB << "in b" << "\n";
            if (fromA)
                select = !seg.inB;
            else
                select = !seg.inA;
            break;
        }

        if (select)
        {
            result.push_back(seg);
        }
    }

    return result;
}

CompoundPath BooleanOperator::assembleResult(const std::vector<SplitSegment> &segments, BooleanOp op)
{
    CompoundPath result;
    std::vector<bool> used(segments.size(), false);

    std::cout << "assembleResult segments:\n";
    for (auto &s : segments)
        std::cout << "  p0=(" << s.curve.p0.x << "," << s.curve.p0.y
                  << ") p3=(" << s.curve.p3.x << "," << s.curve.p3.y << ")\n";
    while (true)
    {
        // Find first unused segment
        int startIdx = -1;
        for (size_t i = 0; i < segments.size(); i++)
        {
            if (!used[i])
            {
                startIdx = (int)i;
                break;
            }
        }
        if (startIdx < 0)
            break;

        Path path;
        int currentIdx = startIdx;

        while (currentIdx >= 0 && !used[currentIdx])
        {
            used[currentIdx] = true;
            const auto &seg = segments[currentIdx];

            // Add start point with handleOut on first point
            if (path.pointCount() == 0)
            {
                PathPoint start;
                start.position = seg.curve.p0;
                start.handleOut = seg.curve.p1 - seg.curve.p0;
                start.handleIn = {0, 0};
                path.addPoint(start);
            }
            else
            {
                // Update handleOut of last point to match this segment's P1
                PathPoint &last = path.pointAt(path.pointCount() - 1);
                last.handleOut = seg.curve.p1 - last.position;
            }

            // Add end point with handleIn
            PathPoint end;
            end.position = seg.curve.p3;
            end.handleIn = seg.curve.p2 - seg.curve.p3;
            end.handleOut = {0, 0};
            path.addPoint(end);

            // Find next segment whose start connects to this end
            Vec2 endPt = seg.curve.p3;
            currentIdx = -1;

            for (size_t i = 0; i < segments.size(); i++)
            {
                if (!used[i] &&
                    segments[i].curve.p0.distanceTo(endPt) < 0.5)
                {
                    currentIdx = (int)i;
                    break;
                }
            }
        }

        if (path.pointCount() >= 3)
        {
            // Connect last point's handleOut back to start
            PathPoint &last = path.pointAt(path.pointCount() - 1);
            PathPoint &first = path.pointAt(0);

            if (last.position.distanceTo(first.position) < 1e-4)
            {
                // Last and first are same point — merge handles
                first.handleIn = last.handleIn;
                path.removePoint(path.pointCount() - 1);
            }

            path.close();
            result.addSubPath(path);
        }
    }

    return result;
}