#include "SceneGraphRegistry.h"

namespace CanvasMVP
{
    static std::atomic<SceneGraph *> s_currentSceneGraph{nullptr};

    SceneGraph *getCurrentSceneGraph()
    {
        return s_currentSceneGraph.load(std::memory_order_acquire);
    }

    void setCurrentSceneGraph(SceneGraph *graph)
    {
        s_currentSceneGraph.store(graph, std::memory_order_release);
    }
}