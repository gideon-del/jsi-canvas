
#include "SceneGraph.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <algorithm>

namespace CanvasMVP
{
    struct Node;

    SceneGraph::SceneGraph() {}
    SceneGraph::~SceneGraph() {}

    int64_t SceneGraph::SpatialGrid::cellKey(float x, float y) const
    {
        int32_t cellX = static_cast<int32_t>(std::floor(x / CELL_SIZE));
        int32_t cellY = static_cast<int32_t>(std::floor(y / CELL_SIZE));
        return (static_cast<int64_t>(cellX) << 32) |
               (static_cast<int64_t>(cellY) & 0xFFFFFFFF);
    }

    void SceneGraph::SpatialGrid::add(Node *node)
    {
        if (!node)
            return;

        int32_t minCellX = static_cast<int32_t>(std::floor(node->bounds.x / CELL_SIZE));
        int32_t minCellY = static_cast<int32_t>(std::floor(node->bounds.y / CELL_SIZE));
        int32_t maxCellX = static_cast<int32_t>(std::floor((node->bounds.x + node->bounds.width) / CELL_SIZE));
        int32_t maxCellY = static_cast<int32_t>(std::floor((node->bounds.y + node->bounds.height) / CELL_SIZE));

        for (int32_t cx = minCellX; cx <= maxCellX; cx++)
            for (int32_t cy = minCellY; cy <= maxCellY; cy++)
                cells_[cellKey(cx * CELL_SIZE, cy * CELL_SIZE)].push_back(node);
    }

    void SceneGraph::SpatialGrid::remove(Node *node)
    {
        if (!node)
            return;

        int32_t minCellX = floor(node->bounds.x / CELL_SIZE);
        int32_t minCellY = floor(node->bounds.y / CELL_SIZE);
        int32_t maxCellX = floor((node->bounds.x + node->bounds.width) / CELL_SIZE);
        int32_t maxCellY = floor((node->bounds.y + node->bounds.height) / CELL_SIZE);

        for (int32_t cx = minCellX; cx <= maxCellX; cx++)
        {
            for (int32_t cy = minCellY; cy <= maxCellY; cy++)
            {
                auto key = cellKey(cx * CELL_SIZE, cy * CELL_SIZE);

                auto it = cells_.find(key);
                if (it == cells_.end())
                    continue;
                auto &vec = it->second;
                vec.erase(
                    std::remove(vec.begin(), vec.end(), node),
                    vec.end());
                if (vec.empty())
                {
                    cells_.erase(it);
                }
            }
        }
    }

    std::vector<Node *> SceneGraph::SpatialGrid::query(const Rect &area) const
    {
        std::unordered_set<Node *> seen;
        std::vector<Node *> results;

        int32_t minCellX = floor(area.x / CELL_SIZE);
        int32_t minCellY = floor(area.y / CELL_SIZE);
        int32_t maxCellX = floor((area.x + area.width) / CELL_SIZE);
        int32_t maxCellY = floor((area.y + area.height) / CELL_SIZE);

        for (int32_t cx = minCellX; cx <= maxCellX; cx++)
        {
            for (int32_t cy = minCellY; cy <= maxCellY; cy++)
            {

                auto key = cellKey(cx * CELL_SIZE, cy * CELL_SIZE);

                auto it = cells_.find(key);

                if (it == cells_.end())
                    continue;

                for (Node *node : it->second)
                {
                    if (seen.count(node) > 0)
                        continue;
                    if (node->intersectsRect(area))
                    {
                        results.push_back(node);
                        seen.insert(node);
                    };
                }
            }
        }

        return results;
    }
    void SceneGraph::SpatialGrid::clear()
    {
        cells_.clear();
    }
    size_t SceneGraph::SpatialGrid::cellCount() const
    {
        return cells_.size();
    }

    size_t SceneGraph::SpatialGrid::totalEntries() const
    {
        size_t count = 0;

        for (const auto &pair : cells_)
        {
            count += pair.second.size();
        }
        return count;
    }

    bool SceneGraph::addNode(std::unique_ptr<Node> node)
    {
        if (node == nullptr)
        {
            return false;
        }

        auto it = nodes_.find(node->id);

        if (it != nodes_.end())
        {
            return false;
        }
        auto nodePtr = node.get();

        nodes_.emplace(node->id, std::move(node));
        sortedNodes_.push_back(nodePtr);

        needsSort_ = true;

        spatialIndex_.add(nodePtr);
        EventEmitter::emit("nodeAdded");
        return true;
    }

    Node *SceneGraph::getNode(const std::string &id)
    {
        auto it = nodes_.find(id);

        if (it != nodes_.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    bool SceneGraph::removeNode(const std::string &id)
    {
        auto it = nodes_.find(id);
        if (it == nodes_.end())
        {
            return false;
        }
        auto nodePtr = it->second.get();
        spatialIndex_.remove(nodePtr);
        sortedNodes_.erase(std::remove(sortedNodes_.begin(), sortedNodes_.end(), nodePtr), sortedNodes_.end());
        nodes_.erase(it);
        EventEmitter::emit("nodeRemoved");
        return true;
    }

    std::vector<Node *> SceneGraph::queryVisible(const Rect &viewPort) const
    {
        return spatialIndex_.query(viewPort);
    }
    const std::vector<Node *> &SceneGraph::getSortedNodes()
    {
        if (needsSort_)
        {
            std::sort(sortedNodes_.begin(),
                      sortedNodes_.end(), [](Node *a, Node *b)
                      { return a->zIndex < b->zIndex; });

            needsSort_ = false;
        }
        return sortedNodes_;
    }

    Node *SceneGraph::hitTest(float x, float y) const
    {
        // Create tiny rect around point
        Rect pointArea(x - 1, y - 1, 2, 2);

        // Query candidates
        auto candidates = spatialIndex_.query(pointArea);

        // Sort by z-index (highest first = on top)
        std::sort(candidates.begin(), candidates.end(),
                  [](const Node *a, const Node *b)
                  {
                      return a->zIndex > b->zIndex;
                  });
        // Return first that contains point
        for (Node *node : candidates)
        {
            if (node->containsPoint(x, y))
            {
                EventEmitter::emit("selectionChanged");
                return node;
            }
        }

        return nullptr;
    }

    void SceneGraph::updateIndex(Node *node)
    {
        if (!node)
            return;
        spatialIndex_.remove(node);
        spatialIndex_.add(node);
        EventEmitter::emit("nodeUpdated");
    }

    void SceneGraph::clear()
    {
        nodes_.clear();
        sortedNodes_.clear();
        spatialIndex_.clear();
        needsSort_ = false;
        EventEmitter::emit("cleared");
    }

    size_t SceneGraph::nodeCount() const
    {
        return nodes_.size();
    }
    std::vector<Node *> SceneGraph::getAllNodes()
    {
        std::vector<Node *> nodes;

        nodes.reserve(nodes_.size());
        for (const auto &[id, node] : nodes_)
        {
            nodes.push_back(node.get());
        }

        return nodes;
    }
    ListenerId SceneGraph::addEventListener(EventType eventType, std::function<void()> callback)
    {
        return EventEmitter::on(eventType, callback);
    }

    void SceneGraph::removeEventListener(EventType eventType, ListenerId id)
    {
        return EventEmitter::off(eventType, id);
    }
}