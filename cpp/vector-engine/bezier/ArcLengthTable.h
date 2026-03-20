#pragma once
#include "CubicBezier.h"

class ArcLengthTable
{
    std::vector<double> lengths_;
    double totalLength_;
    int samples_;

public:
    ArcLengthTable(const CubicBezier &curve, int samples = 100);
    double tAtLength(double len) const;
    double totalLength() const;
};