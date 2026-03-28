#pragma once
#include "../math/vec2.h"

class PathSimplifier
{
public:
    static std::vector<Vec2> simplifyPolyline(std::vector<Vec2> points, double epsilon);
};