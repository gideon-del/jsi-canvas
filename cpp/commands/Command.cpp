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
        auto node = sceneGraph.getNode(nodeId);
        if (node == nullptr)
        {
            return;
        }

        removedNode_ = std::make_unique<NodeSnapShot>(
            NodeSnapShot::fromNode(*node));
        sceneGraph.removeNode(nodeId);
    }
    void RemoveNodeComand::undo(SceneGraph &sceneGraph)
    {
        if (removedNode_ == nullptr)
            return;
        sceneGraph.addNode(std::make_unique<Node>(removedNode_->toNode()));
        }

}