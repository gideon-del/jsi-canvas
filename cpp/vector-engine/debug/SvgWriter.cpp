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
void SvgWriter::text(Vec2 p, const std::string &str, std::string color, double size, std::string anchor)
{

    content_ << "<text x=\"" << p.x << "\" y=\"" << p.y << "\" fill=\"" << color << "\" font-size=\"" << size << "\" font-family=\"Arial,sans-serif\" text-anchor=\"" << anchor << "\">" << str << "</text> \n";
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
