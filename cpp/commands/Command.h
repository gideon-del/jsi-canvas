#pragma once
#include "../scene-graph/SceneGraph.h"

namespace CanvasMVP
{
    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void execute(SceneGraph &sceneGraph) = 0;
        virtual void undo(SceneGraph &sceneGraph) = 0;
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
        bool executed_ = false;

    public:
        RemoveNodeComand(std::string nodeId);
        void execute(SceneGraph &sceneGraph) override;
        void undo(SceneGraph &sceneGraph) override;
    };

    class UpdateNodeCommand : public Command
    {
    private:
        NodeSnapShot oldState_;
        NodeSnapShot newState_;
        bool hasOldState = false;
        void applyState(SceneGraph *sceneGraph, NodeSnapShot &state);

    public:
        UpdateNodeCommand(NodeSnapShot newNode);
        void execute(SceneGraph &sceneGraph) override;
        void undo(SceneGraph &sceneGraph) override;
        bool canMergeWith(Command *other) const override;
        void mergeWith(Command *other) override;
    };

}