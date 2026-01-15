#pragma once
#include "../scene-graph/SceneGraph.h"

namespace CanvasMVP
{

    SceneGraph *getCurrentSceneGraph();
    void setCurrentSceneGraph(SceneGraph *);
};
