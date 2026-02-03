#pragma once
#include <string>
#include "PlatformTypes.h"
#include "../event-emitter/EventEmitter.h"
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
    struct NodeData
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
    };
    struct Node
    {
        NodeData data;

        Node()
            : data(NodeData{
                  .id = "",
                  .type = NodeType::RECTANGLE,
                  .bounds = Rect{0, 0, 100, 100},
                  .zIndex = 0,
                  .fillColor = Color::gray(0.8f),
                  .strokeColor = Color::gray(0.3f),
                  .strokeWidth = 1.0f,
                  .selected = false,
                  .visible = true})
        {
        }
        Node(const std::string &nodeId, NodeType nodeType,
             float x, float y, float w, float h) : data(NodeData{
                                                       .id = nodeId,
                                                       .type = nodeType,
                                                       .bounds = Rect{x, y, w, h},
                                                       .zIndex = 0,
                                                       .fillColor = Color::gray(0.8f),
                                                       .strokeColor = Color::gray(0.3f),
                                                       .strokeWidth = 1.0f,
                                                       .selected = false,
                                                       .visible = true}) {};

        Node(const NodeData &nodeData) : data(nodeData) {};
        virtual ~Node() = default;

        bool containsPoint(float px, float py) const
        {
            return data.visible && data.bounds.contains(px, py);
        }

        bool intersectsRect(const Rect &rect) const
        {
            return data.visible && data.bounds.intersects(rect);
        }

        EventData toEventData()
        {
            auto event = EventData{};
            event.set("id", data.id);
            event.set("x", data.bounds.x);
            event.set("y", data.bounds.y);
            event.set("width", data.bounds.width);
            event.set("height", data.bounds.height);
            event.set("fillColor", data.fillColor.toHexColor());
            event.set("strokeColor", data.strokeColor.toHexColor());
            event.set("strokeWidth", data.strokeWidth);
            event.set("zIndex", data.zIndex);
            return event;
        };
    };

    struct NodeSnapShot
    {
        NodeData data;
        Node toNode() const
        {
            return Node(data);
        };
        static NodeSnapShot fromNode(const Node &node)
        {
            return NodeSnapShot{node.data};
        }
    };
}
