#ifndef CanvasMVP_Node_h
#define CanvasMVP_Node_h
#include <string>
#include "PlatformTypes.h"

namespace CanvasMVP
{

    enum class NodeType
    {
        RECTANGLE
    };

    struct Rect
    {
        float x, y, width, height;

        Rect() : x(0), y(0), width(0), height(0) {}

        Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

        bool contains(float px, float py) const
        {
            return (px >= x && px <= x + width && py >= y && py <= y + height);
        };
        // With comment explaining logic:
        bool intersects(const Rect &other) const
        {
            // Check if rectangles DON'T overlap):
            // - other is completely right of this
            // - other is completely left of this
            // - other is completely below this
            // - other is completely above this
            // If NONE of these are true, they DO overlap
            return !(other.x > x + width ||       // other is right
                     other.x + other.width < x || // other is left
                     other.y > y + height ||      // other is below
                     other.y + other.height < y); // other is above
        }

        Point center() const
        {
            return Point(x + width / 2.0f, y + height / 2.0f);
        }

        RectF toRectF() const
        {
            return RectF(x, y, width, height);
        }
    };

    struct Node
    {
        std::string id;
        NodeType type;

        Rect bounds;
        int zIndex;

        Color fillColor;
        Color strokeColor;
        float strokeWidth;

        bool selected;
        bool visible;

        Node()
            : id(""),
              type(NodeType::RECTANGLE),
              bounds(0, 0, 100, 100),
              zIndex(0),
              fillColor(Color::gray(0.8f)),
              strokeColor(Color::gray(0.3f)),
              strokeWidth(1.0f),
              selected(false),
              visible(true)
        {
        }
        Node(const std::string &nodeId, NodeType nodeType,
             float x, float y, float w, float h) : id(nodeId),
                                                   type(nodeType),
                                                   bounds(x, y, w, h),
                                                   zIndex(0),
                                                   fillColor(Color::gray(0.8f)),
                                                   strokeColor(Color::gray(0.3f)),
                                                   strokeWidth(1.0f),
                                                   selected(false),
                                                   visible(true) {};

        virtual ~Node() = default;

        bool containsPoint(float px, float py) const
        {
            return visible && bounds.contains(px, py);
        }

        bool intersectsRect(const Rect &rect) const
        {
            return visible && bounds.intersects(rect);
        }
    };

}

#endif