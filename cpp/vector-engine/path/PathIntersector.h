#pragma once
#include "Path.h"
struct PathIntersection
{
    size_t path1Segment;
    size_t path2Segment;
    double t1; // parameter on segment 1
    double t2; // parameter on segment 2
    Vec2 point;
};

class PathIntersector
{
public:
    static std::vector<PathIntersection> findIntersections(
        const Path &path1,
        const Path &path2);
};
