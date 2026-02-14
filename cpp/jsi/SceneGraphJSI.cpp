#include "SceneGraphJSI.h"
#include "SceneGraphRegistry.h"
#include <iostream>
#include "JSInvokerRegistery.h"
namespace CanvasMVP
{

    SceneGraphJSI::SceneGraphJSI(std::shared_ptr<SceneGraph> sceneGraph) : sceneGraph_(sceneGraph)
    {
        logger.info("[SceneGraphJSI] created");
        commandHistory_ = std::make_unique<CommandHistory>(*sceneGraph, (size_t)100);
    }

    jsi::Value SceneGraphJSI::get(jsi::Runtime &runtime, const jsi::PropNameID &name)
    {
        auto propName = name.utf8(runtime);

        if (propName == "getNode")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                1,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count < 1)
                    {
                        throw jsi::JSError(rt, "getNode requires 1 argument");
                    }

                    if (!args[0].isString())
                    {
                        throw jsi::JSError(rt, "getNode argument must be a string");
                    }
                    auto id = args[0].asString(rt).utf8(rt);

                    return getNode(rt, id);
                });
        }

        if (propName == "getAllNodes")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return getAllNodes(rt);
                });
        }

        if (propName == "getNodeCount")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return getNodeCount();
                });
        }

        if (propName == "on")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                2,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count < 2)
                    {
                        throw jsi::JSError(rt, "on requires two argument");
                    }
                    if (!args[0].isString())
                    {
                        throw jsi::JSError(rt, "on first argument must be a string");
                    }

                    if (!(args[1].isObject() && args[1].asObject(rt).isFunction(rt)))
                    {
                        throw jsi::JSError(rt, "on seconde argument must be a function");
                    }

                    return on(args[0].asString(rt).utf8(rt), args[1].asObject(rt).asFunction(rt));
                });
        }
        if (propName == "off")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                2,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count < 2)
                    {
                        throw jsi::JSError(rt, "off requires two argument");
                    }
                    if (!args[0].isString())
                    {
                        throw jsi::JSError(rt, "off first argument must be a string");
                    }

                    if (!(args[1].isNumber()))
                    {
                        throw jsi::JSError(rt, "off seconde argument must be a number");
                    }

                    return off(args[0].asString(rt).utf8(rt), static_cast<ListenerId>(args[1].asNumber()));
                });
        }

        if (propName == "executeCommand")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                1,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count > 1)
                    {
                        throw jsi::JSError(rt, "executeCommand must have 1 argument");
                    }
                    if (!args[0].isObject())
                    {
                        throw jsi::JSError(rt, "executeCommand argument must be an Object");
                    }
                    auto obj = args[0].asObject(rt);
                    return executeCommand(rt, obj);
                });
        }
        if (propName == "undo")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return undo();
                });
        }
        if (propName == "redo")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return redo();
                });
        }
        if (propName == "canRedo")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return canRedo();
                });
        }
        if (propName == "canUndo")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return canUndo();
                });
        }
        if (propName == "clearHistory")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return clearHistory();
                });
        }
        return jsi::Value::undefined();
    }

    void SceneGraphJSI::set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value)
    {
        throw jsi::JSError(runtime, "SceneGraph properties are readonly");
    }

    jsi::Value SceneGraphJSI::getNode(jsi::Runtime &runtime, std::string nodeId)
    {
        auto node = sceneGraph_->getNode(nodeId);

        if (node == nullptr)
        {
            return jsi::Value::null();
        }

        return nodeToJSObject(runtime, node);
    }

    jsi::Value SceneGraphJSI::getNodeCount()
    {
        int count = sceneGraph_->nodeCount();
        return jsi::Value(count);
    }
    jsi::Value SceneGraphJSI::getAllNodes(jsi::Runtime &runtime)
    {
        auto allNodes = sceneGraph_->getAllNodes();
        auto jsArray = jsi::Array(runtime, allNodes.size());

        for (size_t i = 0; i < allNodes.size(); ++i)
        {
            jsArray.setValueAtIndex(runtime, i, nodeToJSObject(runtime, allNodes[i]));
        }

        return jsArray;
    }

    void SceneGraphJSI::updateNodeFromJSConfig(jsi::Runtime &runtime, Node &node, const jsi::Object &nodeConfig)
    {

        if (nodeConfig.hasProperty(runtime, "x"))
        {
            auto xValue = nodeConfig.getProperty(runtime, "x");

            if (xValue.isNumber())
            {
                node.data.bounds.x = static_cast<float>(xValue.asNumber());
            }
        }

        if (nodeConfig.hasProperty(runtime, "y"))
        {
            auto yValue = nodeConfig.getProperty(runtime, "y");

            if (yValue.isNumber())
            {
                node.data.bounds.y = static_cast<float>(yValue.asNumber());
            }
        }

        if (nodeConfig.hasProperty(runtime, "width"))
        {
            auto widthValue = nodeConfig.getProperty(runtime, "width");

            if (widthValue.isNumber())
            {
                node.data.bounds.width = static_cast<float>(widthValue.asNumber());
            }
        }

        if (nodeConfig.hasProperty(runtime, "height"))
        {
            auto heightValue = nodeConfig.getProperty(runtime, "height");

            if (heightValue.isNumber())
            {
                node.data.bounds.height = static_cast<float>(heightValue.asNumber());
            }
        }

        if (nodeConfig.hasProperty(runtime, "fillColor"))
        {
            auto colorValue = nodeConfig.getProperty(runtime, "fillColor");

            if (colorValue.isString())
            {
                std::string colorStr = colorValue.asString(runtime).utf8(runtime);
                node.data.fillColor = Color::parseHexColor(colorStr);
            }
        }

        if (nodeConfig.hasProperty(runtime, "strokeColor"))
        {
            auto colorValue = nodeConfig.getProperty(runtime, "strokeColor");

            if (colorValue.isString())
            {
                std::string colorStr = colorValue.asString(runtime).utf8(runtime);
                node.data.strokeColor = Color::parseHexColor(colorStr);
            }
        }

        if (nodeConfig.hasProperty(runtime, "strokeWidth"))
        {
            auto strokeWidthValue = nodeConfig.getProperty(runtime, "strokeWidth");

            if (strokeWidthValue.isNumber())
            {
                node.data.strokeWidth = static_cast<float>(strokeWidthValue.asNumber());
            }
        }

        if (nodeConfig.hasProperty(runtime, "zIndex"))
        {
            auto zIndexValue = nodeConfig.getProperty(runtime, "zIndex");

            if (zIndexValue.isNumber())
            {
                node.data.zIndex = static_cast<int>(zIndexValue.asNumber());
            }
        }
    }
    // Command Related Methods
    jsi::Value SceneGraphJSI::executeCommand(jsi::Runtime &runtime, jsi::Object &commandObj)
    {

        try
        {
            auto command = createCommandFromJS(runtime, commandObj);
            if (!command)
            {
                logger.error("Failed to create command");
                throw jsi::JSError(runtime, "Failed to create command");
            }
            commandHistory_->execute(std::move(command));

            return jsi::Value(true);
        }
        catch (const std::exception &e)
        {
            logger.error(std::string("Failed to execute command") + e.what());
            throw jsi::JSError(runtime, std::string("Failed to execute command") + e.what());
        }
    }
    jsi::Value SceneGraphJSI::undo()
    {
        return jsi::Value(commandHistory_->undo());
    }
    jsi::Value SceneGraphJSI::redo()
    {
        bool success = commandHistory_->redo();
        return jsi::Value(success);
    }
    jsi::Value SceneGraphJSI::canRedo()
    {
        return jsi::Value(commandHistory_->canRedo());
    }
    jsi::Value SceneGraphJSI::canUndo()
    {
        return jsi::Value(commandHistory_->canUndo());
    }
    jsi::Value SceneGraphJSI::clearHistory()
    {

        commandHistory_->clear();
        return jsi::Value::undefined();
    }
    // Type conversion function
    std::unique_ptr<Node> SceneGraphJSI::jsObjectToNode(jsi::Runtime &runtime, const jsi::Object &obj)
    {

        auto node = std::make_unique<Node>();

        static int nodeCounter = 0;
        node->data.id = "node_" + std::to_string(++nodeCounter);

        if (obj.hasProperty(runtime, "x"))
        {
            auto xValue = obj.getProperty(runtime, "x");
            if (xValue.isNumber())
            {
                node->data.bounds.x = static_cast<float>(xValue.asNumber());
            }
        }

        if (obj.hasProperty(runtime, "y"))
        {
            auto yValue = obj.getProperty(runtime, "y");
            if (yValue.isNumber())
            {
                node->data.bounds.y = static_cast<float>(yValue.asNumber());
            }
        }

        if (obj.hasProperty(runtime, "width"))
        {
            auto widthValue = obj.getProperty(runtime, "width");
            if (widthValue.isNumber())
            {
                node->data.bounds.width = static_cast<float>(widthValue.asNumber());
            }
        }
        else
        {
            node->data.bounds.width = 100.0f;
        }

        if (obj.hasProperty(runtime, "height"))
        {
            auto heightValue = obj.getProperty(runtime, "height");
            if (heightValue.isNumber())
            {
                node->data.bounds.height = static_cast<float>(heightValue.asNumber());
            }
        }
        else
        {
            node->data.bounds.height = 100.0f;
        }

        if (obj.hasProperty(runtime, "fillColor"))
        {
            auto colorValue = obj.getProperty(runtime, "fillColor");

            if (colorValue.isString())
            {
                std::string colorStr = colorValue.asString(runtime).utf8(runtime);
                node->data.fillColor = Color::parseHexColor(colorStr);
            }
        }
        else
        {
            node->data.fillColor = Color::red();
        }

        node->data.strokeColor = Color::black();
        node->data.strokeWidth = 2.0f;

        if (obj.hasProperty(runtime, "zIndex"))
        {
            auto zValue = obj.getProperty(runtime, "zIndex");
            if (zValue.isNumber())
            {
                node->data.zIndex = static_cast<int>(zValue.asNumber());
            }
        }

        return node;
    }
    jsi::Value SceneGraphJSI::on(EventType eventType, jsi::Function fn)
    {
        auto fnPtr = std::make_shared<jsi::Function>(std::move(fn));
        auto callInvoker = CanvasMVP::getJSCallInvoker();
        if (callInvoker == nullptr)
        {
            return -1;
        }
        std::function<void(EventData event)> callback = [callInvoker, fnPtr](EventData event) -> void
        {
            auto eventPtr = std::make_shared<EventData>(std::move(event));
            callInvoker->invokeAsync([fnPtr, eventPtr](jsi::Runtime &runtime)
                                     { fnPtr->call(runtime, eventPtr->toJSI(runtime)); });
        };

        auto listenerId = sceneGraph_->addEventListener(eventType, callback);
        return jsi::Value(static_cast<double>(listenerId));
    }
    jsi::Value SceneGraphJSI::off(EventType eventType, ListenerId id)
    {
        sceneGraph_->removeEventListener(eventType, id);
        return jsi::Value::undefined();
    }

    jsi::Object SceneGraphJSI::nodeToJSObject(jsi::Runtime &runtime, const Node *node)
    {
        auto obj = jsi::Object(runtime);
        obj.setProperty(runtime, "id", jsi::String::createFromUtf8(runtime, node->data.id));
        obj.setProperty(runtime, "x", jsi::Value(node->data.bounds.x));
        obj.setProperty(runtime, "y", jsi::Value(node->data.bounds.y));
        obj.setProperty(runtime, "width", jsi::Value(node->data.bounds.width));
        obj.setProperty(runtime, "height", jsi::Value(node->data.bounds.height));
        obj.setProperty(runtime, "zIndex", jsi::Value(node->data.zIndex));

        obj.setProperty(runtime, "fillColor", jsi::String::createFromUtf8(runtime, node->data.fillColor.toHexColor()));

        return obj;
    }

    std::unique_ptr<Command> SceneGraphJSI::createCommandFromJS(jsi::Runtime &runtime, const jsi::Object &obj)
    {
        if (!obj.hasProperty(runtime, "type"))
        {
            throw jsi::JSError(runtime, "Command must have 'type' property");
        }

        auto typeProp = obj.getProperty(runtime, "type");

        if (!typeProp.isString())
        {
            throw jsi::JSError(runtime, "Command 'type' must be a string");
        }

        std::string type = typeProp.asString(runtime).utf8(runtime);

        if (type == "addNode")
        {

            if (!obj.hasProperty(runtime, "node"))
            {
                throw jsi::JSError(runtime, "Add Command must have 'node' property");
            }
            auto nodeProp = obj.getProperty(runtime, "node");

            if (!nodeProp.isObject())
            {
                throw jsi::JSError(runtime, "Add Command  'node' must be an object");
            }

            auto node = jsObjectToNode(runtime, nodeProp.asObject(runtime));

            return std::make_unique<AddNodeCommand>(NodeSnapShot{node->data});
        }
        if (type == "removeNode")
        {

            if (!obj.hasProperty(runtime, "nodeId"))
            {
                throw jsi::JSError(runtime, "RemoveCommand must have 'nodeId' property");
            }
            auto nodeIdProp = obj.getProperty(runtime, "nodeId");

            if (!nodeIdProp.isString())
            {
                throw jsi::JSError(runtime, "RemoveCommand  'nodeId' must be a string");
            }

            std::string nodeId = nodeIdProp.asString(runtime).utf8(runtime);

            return std::make_unique<RemoveNodeComand>(nodeId);
        }
        if (type == "updateNode")
        {

            if (!obj.hasProperty(runtime, "nodeId"))
            {
                throw jsi::JSError(runtime, "UpdateCommand must have 'nodeId' property");
            }
            auto nodeIdProp = obj.getProperty(runtime, "nodeId");

            if (!nodeIdProp.isString())
            {
                throw jsi::JSError(runtime, "UpdateCommand  'nodeId' must be a string");
            }
            if (!obj.hasProperty(runtime, "node"))
            {
                throw jsi::JSError(runtime, "UpdateCommand must have 'node' property");
            }
            auto nodeProp = obj.getProperty(runtime, "node");

            if (!nodeProp.isObject())
            {
                throw jsi::JSError(runtime, "UpdateCommand  'node' must be an object");
            }
            std::string nodeId = nodeIdProp.asString(runtime).utf8(runtime);
            auto nodeConfig = nodeProp.asObject(runtime);
            auto node = sceneGraph_->getNode(nodeId);

            if (!node)
            {
                throw jsi::JSError(runtime, "Node not found");
            }
            auto newNode = Node(node->data);
            updateNodeFromJSConfig(runtime, newNode, nodeConfig);
            return std::make_unique<UpdateNodeCommand>(NodeSnapShot{newNode.data});
        }

        return nullptr;
    }
    bool installSceneGraph(jsi::Runtime &runtime)
    {
        auto sceneGraph = std::make_shared<SceneGraph>();

        auto hostObject = std::make_shared<SceneGraphJSI>(sceneGraph);

        CanvasMVP::setCurrentSceneGraph(sceneGraph.get());

        runtime.global().setProperty(
            runtime,
            "sceneGraph",
            jsi::Object::createFromHostObject(runtime, hostObject));

        return true;
    }

}