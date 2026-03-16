#include "SVGPathParser.h"
#include <sstream>
CompoundPath SVGPathParser::parse(const std::string &d)
{
    CompoundPath result;
    Path currentPath;
    Vec2 currentPos{0, 0};
    Vec2 startPos{0, 0};

    size_t i = 0;
    while (i < d.length())
    {
        skipWhitespace(d, i);

        if (i >= d.length())
            break;
        char cmd = d[i++];
        bool relative = std::islower(cmd);
        cmd = std::toupper(cmd);

        switch (cmd)
        {
        case 'M':
        {
            if (currentPath.pointCount() > 0)
            {
                result.addSubPath(std::move(currentPath));
                currentPath = Path();
            }

            Vec2 p = readPoint(d, i);
            if (relative)
                p = currentPos + p;
            currentPos = startPos = p;
            currentPath.addPoint({{0, 0}, {0, 0}, p});
            break;
        }
        case 'L':
        {
            Vec2 p = readPoint(d, i);
            if (relative)
                p = currentPos + p;
            currentPath.addPoint({{0, 0}, {0, 0}, p});
            currentPos = p;
            break;
        }

        case 'C':
        {
            Vec2 c1 = readPoint(d, i);
            Vec2 c2 = readPoint(d, i);
            Vec2 p = readPoint(d, i);
            if (relative)
            {
                c1 = currentPos + c1;
                c2 = currentPos + c2;
                p = currentPos + p;
            }
            if (currentPath.pointCount() > 0)
            {
                auto &prev = currentPath.pointAt(currentPath.pointCount() - 1);
                prev.handleOut = c1 - prev.position;
            }

            PathPoint newPoint;
            newPoint.position = p;
            newPoint.handleIn = c2 - p;
            currentPath.addPoint(newPoint);

            currentPos = p;
            break;
        }
        case 'Z':
        {
            currentPath.close();
            currentPos = startPos;
            break;
        }
        }
    }

    if (currentPath.pointCount() > 0)
    {
        result.addSubPath(std::move(currentPath));
    }
    return result;
}
std::string SVGPathParser::toSVGString(const CompoundPath &path)
{
    std::ostringstream ss;
    for (size_t pi = 0; pi < path.subpathCount(); pi++)
    {
        const Path &subpath = path.subpathAt(pi);
        if (subpath.pointCount() == 0)
            continue;

        // MoveTo first point
        const auto &first = subpath.pointAt(0);
        ss << "M " << first.position.x << " " << first.position.y;

        // Segments
        for (size_t i = 0; i < subpath.segmentCount(); i++)
        {
            auto seg = subpath.getSegment(i);

            if (seg.isLine())
            {
                ss << " L " << seg.end().position.x
                   << " " << seg.end().position.y;
            }
            else
            {
                ss << " C " << seg.start().absoluteHandleOut().x
                   << " " << seg.start().absoluteHandleOut().y
                   << " " << seg.end().absoluteHandleIn().x
                   << " " << seg.end().absoluteHandleIn().y
                   << " " << seg.end().position.x
                   << " " << seg.end().position.y;
            }
        }

        if (subpath.isClosed())
            ss << " Z";
    }

    return ss.str();
}
void SVGPathParser::skipWhitespace(const std::string &s, size_t &i)
{
    while (i < s.length() && (std::isspace(s[i]) || s[i] == ','))
        i++;
}

double SVGPathParser::readNumber(const std::string &s, size_t &i)
{
    skipWhitespace(s, i);
    size_t start = i;
    if (s[i] == '-' || s[i] == '+')
        i++;
    while (i < s.length() && (std::isdigit(s[i]) || s[i] == '.'))
        i++;
    return std::stod(s.substr(start, i - start));
}

Vec2 SVGPathParser::readPoint(const std::string &s, size_t &i)
{
    double x = readNumber(s, i);
    double y = readNumber(s, i);

    return {x, y};
}