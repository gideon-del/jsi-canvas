#pragma once
#include "Segment.h"

class Path
{
    std::vector<PathPoint> points_;
    bool closed_ = false;

public:
    size_t pointCount() const;
    PathPoint &pointAt(size_t i);
    const PathPoint &pointAt(size_t i) const;

    void addPoint(PathPoint p);
    void insertPoint(size_t index, PathPoint p);
    void removePoint(size_t index);

    void close();
    void open();
    bool isClosed() const;

    size_t segmentCount() const;
    Segment getSegment(size_t index) const;

    template <typename Func>
    void forEachSegment(Func &&fn) const;

    void reverse();
    Rect bounds() const;
    double length() const;
    Vec2 pointAtLength(double len) const;
};