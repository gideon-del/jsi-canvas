#pragma once
#include "Path.h"

enum class FillRule
{
    NonZero,
    EvenOdd
};

class CompoundPath
{
    std::vector<Path> subpaths_;
    FillRule fillRule_ = FillRule::NonZero;

public:
    void addSubPath(Path p);
    void removeSubPath(size_t index);

    size_t subpathCount() const;
    Path &subpathAt(size_t index);
    const Path &subpathAt(size_t index) const;

    void setFillRule(FillRule rule);
    FillRule fillRule() const;

    Rect bounds() const;
    int windingNumber(Vec2 point) const;
    bool contains(Vec2 point) const;

private:
    static int pathWindingNumber(const Path &path, Vec2 point);
    static double isLeft(Vec2 p1, Vec2 p2, Vec2 point);
};
