#pragma once
#include <iostream>
typedef __UINT32_TYPE__ uint32_t;

namespace CanvasMVP
{
    // Platform-agnostic point
    struct Point
    {
        float x = 0.0f;
        float y = 0.0f;

        Point() : x(0), y(0) {}
        Point(float x, float y) : x(x), y(y) {}
    };

    // Platform-agnostic size

    struct Size
    {
        float width = 0.0f;
        float height = 0.0f;
    };

    // Platform-agnostic rectangle

    struct RectF
    {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;

        RectF() : x(0), y(0), width(0), height(0) {};
        RectF(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {};
        bool contains(const Point &point) const
        {
            return point.x >= x && point.x <= (x + width) &&
                   point.y >= y && point.y <= (y + height);
        }

        bool intersects(const RectF &other) const
        {
            return !(other.x > x + width ||
                     other.x + other.width < x ||
                     other.y > y + height ||
                     other.y + other.height < y);
        }

        Point center() const
        {
            return Point{x + width / 2.0f, y + height / 2.0f};
        }
    };

    struct Color
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 1.0f;
        Color() : r(0), g(0), b(0), a(1) {}
        Color(float r, float g, float b, float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}
        uint32_t toUInt32() const
        {
            return ((uint32_t)(r * 255) << 24) |
                   ((uint32_t)(g * 255) << 16) |
                   ((uint32_t)(b * 255) << 8) |
                   ((uint32_t)(a * 255));
        }

        static Color white() { return Color{1.0f, 1.0f, 1.0f, 1.0f}; }
        static Color black() { return Color{0.0f, 0.0f, 0.0f, 1.0f}; }
        static Color red() { return Color{1.0f, 0.0f, 0.0f, 1.0f}; }
        static Color green() { return Color{0.0f, 1.0f, 0.0f, 1.0f}; }
        static Color blue() { return Color{0.0f, 0.0f, 1.0f, 1.0f}; }
        static Color gray(float brightness)
        {
            return Color(brightness, brightness, brightness, 1.0);
        }
        static Color parseHexColor(std::string colorStr)
        {
            if (colorStr.size() != 7 || colorStr[0] != '#')
            {
                return Color::black();
            }

            unsigned int r = 0, g = 0, b = 0;

            int parsed = std::sscanf(
                colorStr.c_str(),
                "#%02x%02x%02x",
                &r, &g, &b);

            if (parsed != 3)
            {
                return Color::black();
            }

            return Color{
                r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                1.0f};
        }
        std::string toHexColor() const
        {
            char fillColorHex[8];
            snprintf(fillColorHex, sizeof(fillColorHex), "#%02x%02x%02x",
                     (int)(r * 255),
                     (int)(g * 255),
                     (int)(b * 255)

            );

            return std::string(fillColorHex);
        };
    };

} // namespace CanvasMVP
