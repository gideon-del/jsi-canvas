#ifndef CanvasMVP_SceneGraphJSI_h
#define CanvasMVP_SceneGraphJSI_h

#include <jsi/jsi.h>
#include <memory>
#include <string>
#include "../scene-graph/SceneGraph.h"

namespace CanvasMVP
{
    using namespace facebook;
    class SceneGraphJSI : public jsi::HostObject
    {
    private:
        std::shared_ptr<SceneGraph> sceneGraph_;

    public:
        SceneGraphJSI(std::shared_ptr<SceneGraph> sceneGraph);

        jsi::Value get(jsi::Runtime &runtime, const jsi::PropNameID &name) override;
        void set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value) override;

    private:
        std::unique_ptr<Node> jsObjectToNode(jsi::Runtime &runtime, const jsi::Object &obj);

        jsi::Object nodeToJSObject(jsi::Runtime &runtime, const Node *node);

        jsi::Value addNode(jsi::Runtime &runtime, const jsi::Object &nodeConfig);
    };

    bool installSceneGraph(jsi::Runtime &runtime);

}

#endif