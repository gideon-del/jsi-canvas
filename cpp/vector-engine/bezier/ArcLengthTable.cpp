#include "ArcLengthTable.h"

ArcLengthTable::ArcLengthTable(const CubicBezier &curve, int samples) : samples_(samples)
{
    lengths_.reserve(samples + 1);
    lengths_.push_back(0);

    for (int i = 1; i <= samples; i++)
    {
        double t = static_cast<double>(i) / samples_;
        double len = curve.arcLengthAt(t);
        lengths_.push_back(len);
    }

    totalLength_ = lengths_.back();
}

double ArcLengthTable::tAtLength(double len) const
{
    if (len <= 0)
        return 0;
    if (len >= totalLength_)
        return 1;

    auto it = std::lower_bound(lengths_.begin(), lengths_.end(), len);

    int index = it - lengths_.begin();

    if (index == 0)
        return 0;
    double t0 = static_cast<double>(index - 1) / samples_;
    double t1 = static_cast<double>(index) / samples_;
    double len0 = lengths_[index - 1];
    double len1 = lengths_[index];

    double fraction = (len - len0) / (len1 - len0);

    return t0 + fraction * (t1 - t0);
}

double ArcLengthTable::totalLength() const { return totalLength_; }