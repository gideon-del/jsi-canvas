
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <algorithm>
#include "../types/Node.h"

namespace CanvasMVP
{
    struct Node; // forward declaration

    class SceneGraph
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;
        std::vector<Node *> sortedNodes_;
        bool needsSort_ = false;

        class SpatialGrid
        {
        private:
            static constexpr float CELL_SIZE = 200.0f;
            std::unordered_map<int64_t, std::vector<Node *>> cells_;

            int64_t cellKey(float x, float y) const;

        public:
            void add(Node *node);
            void remove(Node *node);
            std::vector<Node *> query(const Rect &area) const;
            void clear();
            size_t cellCount() const;
            size_t totalEntries() const;
        };
        SpatialGrid spatialIndex_;

    public:
        SceneGraph();
        ~SceneGraph();

        SceneGraph(const SceneGraph &) = delete;
        SceneGraph &operator=(const SceneGraph &) = delete;

        bool addNode(std::unique_ptr<Node> node);
        Node *getNode(const std::string &id);
        bool removeNode(const std::string &id);

        std::vector<Node *> queryVisible(const Rect &viewport) const;
        const std::vector<Node *> &getSortedNodes();
        Node *hitTest(float x, float y) const;
        void updateIndex(Node *node);
        void clear();
        size_t nodeCount() const;
    };
}
