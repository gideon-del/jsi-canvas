#include "PathDasher.h"

CompoundPath PathDasher::dash(const Path &path, const std::vector<double> &pattern, double offset)
{
    CompoundPath result;
    double totalLength = path.length();
    double patternLen = 0;

    for (double d : pattern)
        patternLen += d;
    if (patternLen <= 0)
    {
        result.addSubPath(path);
        return result;
    }
    offset = std::fmod(offset, patternLen);
    if (offset < 0)
        offset += patternLen;
    double pos = -offset;
    bool drawing = true;
    size_t patternIdx = 0;
    Path currentDash;
    while (pos < totalLength)
    {
        double dashLen = pattern[patternIdx];
        double dashEnd = pos + dashLen;

        if (drawing)
        {
            double startLen = std::max(0.0, pos);
            double endLen = std::min(totalLength, dashEnd);
            if (startLen < endLen)
            {
                const int samples = 5;
                for (int i = 0; i <= samples; i++)
                {
                    double len = startLen + (endLen - startLen) * i / samples;
                    Vec2 p = path.pointAtLength(len);
                    currentDash.addPoint({{0, 0}, {0, 0}, p});
                }
            }
        }
        else
        {
            if (currentDash.pointCount() > 0)
            {
                result.addSubPath(std::move(currentDash));
                currentDash = Path();
            }
        }

        pos = dashEnd;
        drawing = !drawing;

        patternIdx = (patternIdx + 1) % pattern.size();
    }

    if (currentDash.pointCount() > 0)
    {
        result.addSubPath(std::move(currentDash));
    }

    return result;
}