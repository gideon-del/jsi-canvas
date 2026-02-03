#pragma once
#include "../scene-graph/SceneGraph.h"

namespace CanvasMVP
{
    class Command
    {
    public:
        virtual void execute(SceneGraph &sceneGraph);
        virtual void undo(SceneGraph &sceneGraph);
        virtual void redo(SceneGraph &sceneGraph);
    };

}