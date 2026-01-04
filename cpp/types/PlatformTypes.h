#ifndef CanvasMVP_PlatformTypes_h
#define CanvasMVP_PlatformTypes_h

typedef __UINT32_TYPE__ uint32_t;

namespace CanvasMVP
{
    // Platform-agnostic point
    struct Point
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    // Platform-agnostic size

    struct Size
    {
        float width = 0.0f;
        float height = 0.0f;
    };

    // Platform-agnostic rectangle

    struct Rect
    {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;

        bool contains(const Point &point) const
        {
            return point.x >= x && point.x <= (x + width) &&
                   point.y >= y && point.y <= (y + height);
        }

        bool intersects(const Rect &other) const
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
    };

} // namespace CanvasMVP

#endif