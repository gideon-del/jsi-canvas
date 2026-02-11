#pragma once
#include "../scene-graph/SceneGraph.h"

namespace CanvasMVP
{
    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void execute(SceneGraph &sceneGraph);
        virtual void undo(SceneGraph &sceneGraph);
        virtual bool canMergeWith(Command *other) const { return false; };
        virtual void mergeWith(Command *other) {}
    };

    class AddNodeCommand : public Command
    {
    private:
        const NodeSnapShot node;
        bool executed_ = false;

    public:
        AddNodeCommand(NodeSnapShot node);
        void execute(SceneGraph &sceneGraph) override;
        void undo(SceneGraph &sceneGraph) override;
    };

    class RemoveNodeComand : public Command
    {
    private:
        std::unique_ptr<NodeSnapShot> removedNode_;
        std::string nodeId;

    public:
        RemoveNodeComand(std::string nodeId);
        void execute(SceneGraph &sceneGraph) override;
        void undo(SceneGraph &sceneGraph) override;
    };

}