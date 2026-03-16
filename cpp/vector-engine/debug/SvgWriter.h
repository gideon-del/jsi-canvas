#pragma once
#include <sstream>
#include <string>
#include "../math/Primitives.h"
#include "../math/vec2.h"
#include "../bezier/CubicBezier.h"
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
    //----------------------------------------------------------
    // PRIMITIVES
    //----------------------------------------------------------
    void point(Vec2 p, std::string color = "red", double radius = 4);
    void line(Vec2 a, Vec2 b, std::string color = "black", double width = 4);
    void rect(Rect r, std::string stroke = "black", std::string fill = "none", double width = 1);
    void text(Vec2 p, const std::string &str, std::string color = "black", double size = 12, std::string anchor = "start");

    //----------------------------------------------------------
    // BEZIER CURVES
    //----------------------------------------------------------

    void polygon(const std::vector<Vec2> &points, std::string stroke = "black", std::string fill = "none", double width = 1);
    void quadraticBezier(Vec2 p0, Vec2 p1, Vec2 p2,
                         std::string color = "black", double width = 2);
    void cubicBezier(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3,
                     std::string color = "black", double width = 2);
    void path(const std::string &d, std::string stroke = "black",
              std::string fill = "none", double width = 2);

    //----------------------------------------------------------
    // DEBUG HELPERS
    //----------------------------------------------------------

    void bezierDebug(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, std::string color = "black");
    void bezierSamples(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, int samples = 10);
    void bezierTangents(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, int samples = 5, double scale = 30);
    void bezierNormals(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, int samples = 5, double scale = 20);
    void deCasteljauDebug(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, double t);
    void grid(double spacing, std::string color = "#eee");
    void boundingBox(Rect box, std::string color = "#FF9800");

    void labeledPoint(Vec2 p, const std::string &name, std::string color = "black");
    void arrow(Vec2 from, Vec2 to, std::string color = "black", double width = 1);
    void comment(const std::string &text)
    {
        content_ << "<!-- " << text << " -->\n";
    }
    bool save(const std::string &filename);
};