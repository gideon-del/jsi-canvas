#ifndef CanvasMVP_SceneGraphJSI_h
#define CanvasMVP_SceneGraphJSI_h

#include <jsi/jsi.h>
#include <memory>
#include <string>
#include "../scene-graph/SceneGraph.h"
#include "../commands/CommandHistory.h"
#include "../logger/Logger.h"
namespace CanvasMVP
{
    using namespace facebook;
    class SceneGraphJSI : public jsi::HostObject
    {
    private:
        std::shared_ptr<SceneGraph> sceneGraph_;
        std::unique_ptr<CommandHistory> commandHistory_;
        Logger logger{"SceneGraphJSI"};

    public:
        SceneGraphJSI(std::shared_ptr<SceneGraph> sceneGraph);

        jsi::Value get(jsi::Runtime &runtime, const jsi::PropNameID &name) override;
        void set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value) override;

    private:
        std::unique_ptr<Node> jsObjectToNode(jsi::Runtime &runtime, const jsi::Object &obj);

        jsi::Object nodeToJSObject(jsi::Runtime &runtime, const Node *node);

        jsi::Value getNode(jsi::Runtime &runtime, std::string nodeId);
        jsi::Value getNodeCount();
        jsi::Value getAllNodes(jsi::Runtime &runtime);
        void updateNodeFromJSConfig(jsi::Runtime &runtime, Node &node, const jsi::Object &nodeConfig);

        jsi::Value on(EventType eventType, jsi::Function callback);
        jsi::Value off(EventType eventType, ListenerId id);

        // Command Related endpoint
        jsi::Value executeCommand(jsi::Runtime &runtime, jsi::Object &commandObj);
        jsi::Value undo();
        jsi::Value redo();
        jsi::Value canRedo();
        jsi::Value canUndo();
        // jsi::Value getHistory(jsi::Runtime &runtime);
        jsi::Value clearHistory();

        std::unique_ptr<Command> createCommandFromJS(jsi::Runtime &runtime, const jsi::Object &obj);
    };

    bool installSceneGraph(jsi::Runtime &runtime);

}

#endif