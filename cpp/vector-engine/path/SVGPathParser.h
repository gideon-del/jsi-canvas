#pragma once
#include "CompoundPath.h"
class SVGPathParser
{
public:
    static CompoundPath parse(const std::string &d);
    static std::string toSVGString(const CompoundPath &path);

private:
    static void skipWhitespace(const std::string &s, size_t &i);
    static double readNumber(const std::string &s, size_t &i);
    static Vec2 readPoint(const std::string &s, size_t &i);
};