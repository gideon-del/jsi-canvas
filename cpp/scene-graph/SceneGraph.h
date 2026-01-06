#include "../types/Node.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

namespace CanvasMVP
{
    class SceneGraph
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;

        std::vector<Node *> sortedNodes_;
        bool needsSort_ = false;

    public:
        SceneGraph(/* args */);
        ~SceneGraph();

        SceneGraph(const SceneGraph &) = delete;
        SceneGraph &operator=(const SceneGraph &) = delete;

        // Core methods
        bool addNode(std::unique_ptr<Node> node)
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
            auto raw = node.get();

            nodes_.emplace(node->id, std::move(node));
            sortedNodes_.push_back(raw);

            needsSort_ = true;
            return true;
        };

        Node *getNode(const std::string &id)
        {
            auto it = nodes_.find(id);

            if (it != nodes_.end())
            {
                return it->second.get();
            }
            return nullptr;
        }

        bool removeNode(const std::string &id)
        {
            auto it = nodes_.find(id);

            if (it == nodes_.end())
            {
                return false;
            }
            auto raw = it->second.get();

            sortedNodes_.erase(std::remove(sortedNodes_.begin(), sortedNodes_.end(), raw), sortedNodes_.end());
            nodes_.erase(it);
            return true;
        }

        const std::vector<Node *> &getSortedNodes()
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

        void clear()
        {
            nodes_.clear();
            sortedNodes_.clear();
            needsSort_ = false;
        }

        size_t nodeCount() const
        {
            return nodes_.size();
        }
    };

    SceneGraph::SceneGraph(/* args */)
    {
    }

    SceneGraph::~SceneGraph()
    {
    }

}