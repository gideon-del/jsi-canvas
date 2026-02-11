#include "Command.h"

namespace CanvasMVP
{
    AddNodeCommand::AddNodeCommand(NodeSnapShot node) : node(node) {}
    void AddNodeCommand::execute(SceneGraph &sceneGraph)
    {
        if (executed_)
            return;
        sceneGraph.addNode(std::make_unique<Node>(node.data));
        executed_ = true;
    }
    void AddNodeCommand::undo(SceneGraph &sceneGraph)
    {
        if (!executed_)
            return;
        sceneGraph.removeNode(node.data.id);

        executed_ = false;
    }

    RemoveNodeComand::RemoveNodeComand(std::string nodeId) : nodeId(nodeId) {};

    void RemoveNodeComand::execute(SceneGraph &sceneGraph)
    {
        if (executed_)
            return;
        auto node = sceneGraph.getNode(nodeId);
        if (node == nullptr)
        {
            return;
        }

        removedNode_ = std::make_unique<NodeSnapShot>(
            NodeSnapShot::fromNode(*node));
        sceneGraph.removeNode(nodeId);
        executed_ = true;
    }
    void RemoveNodeComand::undo(SceneGraph &sceneGraph)
    {
        if (!executed_)
            return;
        if (removedNode_ == nullptr)
            return;
        sceneGraph.addNode(std::make_unique<Node>(removedNode_->toNode()));
        executed_ = false;
    }

    UpdateNodeCommand::UpdateNodeCommand(NodeSnapShot updatedNode) : newState_(updatedNode) {};

    void UpdateNodeCommand::execute(SceneGraph &sceneGraph)
    {
        auto node = sceneGraph.getNode(newState_.data.id);
        if (node == nullptr)
            return;
        if (!hasOldState)
        {
            oldState_ = NodeSnapShot::fromNode(*node);
            hasOldState = true;
        }
        applyState(&sceneGraph, newState_);
    }
    void UpdateNodeCommand::applyState(SceneGraph *sceneGraph, NodeSnapShot &state)
    {
        auto node = sceneGraph->getNode(state.data.id);
        if (node == nullptr)
            return;
        node->data = state.data;
        sceneGraph->updateIndex(node);
    }
    void UpdateNodeCommand::undo(SceneGraph &sceneGraph)
    {
        if (!hasOldState)
            return;
        applyState(&sceneGraph, oldState_);
    }
    bool UpdateNodeCommand::canMergeWith(Command *other) const
    {
        auto otherUpdateCmd = dynamic_cast<UpdateNodeCommand *>(other);
        if (!otherUpdateCmd)
            return false;
        return otherUpdateCmd->newState_.data.id == newState_.data.id;
    }
    void UpdateNodeCommand::mergeWith(Command *other)
    {
        auto otherUpdateCmd = dynamic_cast<UpdateNodeCommand *>(other);
        if (!otherUpdateCmd)
            return;
        newState_ = otherUpdateCmd->newState_;
    }

}