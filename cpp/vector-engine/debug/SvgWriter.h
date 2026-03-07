#pragma once
#include <sstream>
#include <string>
#include "../math/Primitives.h"
#include "../math/vec2.h"
class SvgWriter
{

private:
    std::ostringstream content_;
    Rect viewBox_;
    double width;
    double height;

public:
    SvgWriter(Rect viewBox, double width = 0, double height = 0) : viewBox_(viewBox), width(width), height(height) {}
    SvgWriter(double size) : SvgWriter(Rect{0, 0, size, size}) {}

    void point(Vec2 p, std::string color = "red", double radius = 4);
    void line(Vec2 a, Vec2 b, std::string color = "black", double width = 4);
    void rect(Rect r, std::string stroke = "black", std::string fill = "none", double width = 1);
    void text(Vec2 p, const std::string &str, std::string color = "black", double size = 12, std::string anchor = "start");
    void polygon(const std::vector<Vec2> &points, std::string stroke = "black", std::string fill = "none", double width = 1);
    void grid(double spacing, std::string color = "#eee");

    void labeledPoint(Vec2 p, const std::string &name, std::string color = "black");
    void arrow(Vec2 from, Vec2 to, std::string color = "black", double width = 1);
    void comment(const std::string &text)
    {
        content_ << "<!-- " << text << " -->\n";
    }
    bool save(const std::string &filename);
};