#include "../path/CompoundPath.h"
class PathDasher
{
public:
    static CompoundPath dash(const Path &path, const std::vector<double> &pattern, double offset = 0);
};