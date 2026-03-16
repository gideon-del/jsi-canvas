#include "SvgWriter.h"
#include <fstream>

void SvgWriter::point(Vec2 p, std::string color, double radius)
{
    content_ << "<circle cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\""
             << radius << "\" fill=\"" << color << "\"/> \n";
}
void SvgWriter::line(Vec2 a, Vec2 b, std::string color, double width)
{
    content_ << "<line x1=\"" << a.x << "\" y1=\"" << a.y << "\" x2=\""
             << b.x << "\" y2=\"" << b.y << "\" stroke=\""
             << color << "\" stroke-width=\"" << width << "\" />\n";
}

void SvgWriter::rect(Rect r, std::string stroke, std::string fill, double width)
{
    content_ << "<rect x=\"" << r.x << "\" y=\"" << r.y << "\" width=\"" << r.width << "\" height=\"" << r.height << "\" stroke=\"" << stroke << "\" fill=\"" << fill << "\" stroke-width=\"" << width << "\" />\n";
}
void SvgWriter::polygon(const std::vector<Vec2> &points, std::string stroke, std::string fill, double width)
{
    if (points.empty())
        return;

    content_ << "<polygon points=\"";

    for (const auto &p : points)
    {
        content_ << p.x << "," << p.y << " ";
    }

    content_ << "\" stroke=\""
             << stroke << "\" stroke-width=\""
             << width << "\" fill=\""
             << fill << "\"/>\n";
}

void SvgWriter::text(Vec2 p, const std::string &str, std::string color, double size, std::string anchor)
{

    content_ << "<text x=\"" << p.x << "\" y=\"" << p.y << "\" fill=\"" << color << "\" font-size=\"" << size << "\" font-family=\"Arial,sans-serif\" text-anchor=\"" << anchor << "\">" << str << "</text> \n";
}

void SvgWriter::quadraticBezier(Vec2 p0, Vec2 p1, Vec2 p2,
                                std::string color, double width)
{

    content_ << "<path d=\"M" << p0.x << " " << p0.y
             << " Q " << p1.x << " " << p1.y
             << " " << p2.x << " " << p2.y
             << "\" stroke=\"" << color << "\" stroke-width=\"" << width
             << "\" fill=\"none\"/>\n";
}
void SvgWriter::cubicBezier(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3,
                            std::string color, double width)
{
    content_ << "<path d=\"M" << p0.x << " " << p0.y
             << " C " << p1.x << " " << p1.y
             << " " << p2.x << " " << p2.y
             << " " << p3.x << " " << p3.y
             << "\" stroke=\"" << color << "\" stroke-width=\"" << width
             << "\" fill=\"none\"/>\n";
}
void SvgWriter::path(const std::string &d, std::string stroke,
                     std::string fill, double width)
{
    content_ << "<path d=\"" << d << "\" stroke=\"" << stroke
             << "\" stroke-width=\"" << width << "\" fill=\"" << fill << "\"/>\n";
}

void SvgWriter::bezierDebug(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, std::string color)
{
    line(p0, p1, "#aaa");
    line(p3, p2, "#aaa");

    cubicBezier(p0, p1, p2, p3, color);

    point(p0, "#2196F3", 6);
    point(p3, "#2196F3", 6);

    point(p1, "#F44336", 4);
    point(p2, "#F44336", 4);
}

void SvgWriter::bezierSamples(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, int samples)
{
    for (int i = 1; i <= samples; i++)
    {
        double t = static_cast<double>(i) / samples;

        CubicBezier bezier = CubicBezier(p0, p1, p2, p3);
        Vec2 pt = bezier.evaluateDeCasteljau(t);
        point(pt, "#4CAF50", 3);
    }
}

void SvgWriter::bezierTangents(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, int samples, double scale)
{
    for (int i = 0; i <= samples; i++)
    {
        double t = static_cast<double>(i) / samples;
        CubicBezier bezier = CubicBezier(p0, p1, p2, p3);

        Vec2 pt = bezier.evaluateDeCasteljau(t);
        double mt = 1.0 - t;
        Vec2 tangent = (p1 - p0) * (3 * mt * mt) +
                       (p2 - p1) * (6 * mt * t) +
                       (p3 - p0) * (3 * t * t);

        tangent = tangent.normalized() * scale;

        line(pt, pt + tangent, "#9C27B0", 1);
        point(pt, "#9C27B0", 3);
    }
}

void SvgWriter::bezierNormals(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, int samples, double scale)
{
    for (int i = 0; i <= samples; i++)
    {
        double t = static_cast<double>(i) / samples;
        CubicBezier bezier = CubicBezier(p0, p1, p2, p3);

        Vec2 pt = bezier.evaluateDeCasteljau(t);
        double mt = 1.0 - t;
        Vec2 tangent = (p1 - p0) * (3 * mt * mt) +
                       (p2 - p1) * (6 * mt * t) +
                       (p3 - p2) * (3 * t * t);

        // Normal (perpendicular to tangent)
        Vec2 normal = tangent.perpendicular().normalized() * scale;

        line(pt - normal, pt + normal, "#00BCD4", 1);
    }
}

void SvgWriter::deCasteljauDebug(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, double t)
{
    // Original control polygon
    line(p0, p1, "#ccc", 1);
    line(p1, p2, "#ccc", 1);
    line(p2, p3, "#ccc", 1);

    // Level 1
    Vec2 p01 = p0.lerp(p1, t);
    Vec2 p12 = p1.lerp(p2, t);
    Vec2 p23 = p2.lerp(p3, t);

    line(p01, p12, "#aaa", 1);
    line(p12, p23, "#aaa", 1);
    point(p01, "#FF9800", 4);
    point(p12, "#FF9800", 4);
    point(p23, "#FF9800", 4);

    // Level 2
    Vec2 p012 = p01.lerp(p12, t);
    Vec2 p123 = p12.lerp(p23, t);

    line(p012, p123, "#888", 1);
    point(p012, "#E91E63", 4);
    point(p123, "#E91E63", 4);

    // Final point
    Vec2 p0123 = p012.lerp(p123, t);
    point(p0123, "#4CAF50", 6);

    // The curve itself
    cubicBezier(p0, p1, p2, p3, "#2196F3", 2);

    // Original points
    point(p0, "#2196F3", 5);
    point(p1, "#F44336", 4);
    point(p2, "#F44336", 4);
    point(p3, "#2196F3", 5);
}

void SvgWriter::boundingBox(Rect box, std::string color)
{
    rect(box, color, "none", 1);
    // Corner markers
    double m = 3;
    point({box.left(), box.top()}, color, m);
    point({box.right(), box.top()}, color, m);
    point({box.left(), box.bottom()}, color, m);
    point({box.right(), box.bottom()}, color, m);
}

void SvgWriter::grid(double spacing, std::string color)
{
    for (double x = viewBox_.x; x <= viewBox_.right(); x += spacing)
    {
        line({x, viewBox_.top()}, {x, viewBox_.bottom()}, color, 0.5);
    }
    for (double y = viewBox_.y; y <= viewBox_.bottom(); y += spacing)
    {
        line({viewBox_.left(), y}, {viewBox_.right(), y}, color, 0.5);
    }
}

void SvgWriter::labeledPoint(Vec2 p, const std::string &name, std::string color)
{
    point(p, color, 5);
    std::ostringstream label;
    label << name << " (" << static_cast<int>(p.x) << "," << static_cast<int>(p.y) << ")";
    text(p + Vec2{8, -8}, label.str(), color, 10);
}

void SvgWriter::arrow(Vec2 from, Vec2 to, std::string color, double width)
{
    line(from, to, color, width);
    Vec2 dir = (to - from).normalized();
    Vec2 perp = dir.perpendicular();

    double headSize = 8;
    Vec2 left = to - dir * headSize + perp * (headSize * 0.4);
    Vec2 right = to - dir * headSize - perp * (headSize * 0.4);
    polygon({to, left, right}, color, color, width);
}
bool SvgWriter::save(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
    file << "viewBox=\"" << viewBox_.x << " " << viewBox_.y << " " << viewBox_.width << " " << viewBox_.height << "\">\n";

    file << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n";
    file << content_.str();

    file << "</svg>\n";
    file.close();
    return true;
}
