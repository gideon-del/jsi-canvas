#include "SceneGraphJSI.h"
#include "SceneGraphRegistry.h"
#include <iostream>
#include "JSInvokerRegistery.h"
namespace CanvasMVP
{

    SceneGraphJSI::SceneGraphJSI(std::shared_ptr<SceneGraph> sceneGraph) : sceneGraph_(sceneGraph)
    {
        std::cout << "[SceneGraphJSI] created" << std::endl;
    }

    jsi::Value SceneGraphJSI::get(jsi::Runtime &runtime, const jsi::PropNameID &name)
    {
        auto propName = name.utf8(runtime);
        if (propName == "addNode")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                1,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count < 1)
                    {
                        throw jsi::JSError(rt, "addNode requires 1 argument");
                    }
                    if (!args[0].isObject())
                    {
                        throw jsi::JSError(rt, "addNode argument must be an object");
                    }
                    return addNode(rt, args[0].asObject(rt));
                });
        }

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
        if (propName == "removeNode")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                1,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count < 1)
                    {
                        throw jsi::JSError(rt, "removeNode requires 1 argument");
                    }

                    if (!args[0].isString())
                    {
                        throw jsi::JSError(rt, "removeNode argument must be a string");
                    }
                    auto id = args[0].asString(rt).utf8(rt);

                    return removeNode(id);
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

        if (propName == "clear")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    return clear();
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

        if (propName == "updateNode")
        {

            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                2,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count) -> jsi::Value
                {
                    if (count < 2)
                    {
                        throw jsi::JSError(rt, "updateNode requires two argument");
                    }
                    if (!args[0].isString())
                    {
                        throw jsi::JSError(rt, "updateNode first argument must be a string");
                    }

                    if (!args[1].isObject())
                    {
                        throw jsi::JSError(rt, "updateNode seconde argument must be an object");
                    }

                    auto id = args[0].asString(rt).utf8(rt);
                    auto nodeConfig = args[1].asObject(rt);
                    return updateNode(rt, id, nodeConfig);
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

        return jsi::Value::undefined();
    }

    void SceneGraphJSI::set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value)
    {
        throw jsi::JSError(runtime, "SceneGraph properties are readonly");
    }

    // Helpers
    jsi::Value SceneGraphJSI::addNode(jsi::Runtime &runtime, const jsi::Object &nodeConfig)
    {
        std::cout << "[SceneGraphJSI] addNode called" << std::endl;
        try
        {
            auto node = jsObjectToNode(runtime, nodeConfig);

            std::string nodeId = node->id;
            sceneGraph_->addNode(std::move(node));
            std::cout << "[SceneGraphJSI]  Node added: " << nodeId << std::endl;

            return jsi::String::createFromUtf8(runtime, nodeId);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[SceneGraphJSI] Error: " << e.what() << std::endl;

            throw jsi::JSError(runtime, std::string("Failed to add node: ") + e.what());
        }
    };

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

    jsi::Value SceneGraphJSI::removeNode(std::string nodeId)
    {
        return jsi::Value(sceneGraph_->removeNode(nodeId));
    }

    jsi::Value SceneGraphJSI::clear()
    {

        sceneGraph_->clear();

        return jsi::Value::undefined();
    }

    jsi::Value SceneGraphJSI::updateNode(jsi::Runtime &runtime, std::string nodeId, const jsi::Object &nodeConfig)
    {
        auto node = sceneGraph_->getNode(nodeId);
        auto boundsChanged = false;
        if (node == nullptr)
        {
            return jsi::Value(false);
        }

        if (nodeConfig.hasProperty(runtime, "x"))
        {
            auto xValue = nodeConfig.getProperty(runtime, "x");

            if (xValue.isNumber())
            {
                node->bounds.x = static_cast<float>(xValue.asNumber());
                boundsChanged = true;
            }
        }

        if (nodeConfig.hasProperty(runtime, "y"))
        {
            auto yValue = nodeConfig.getProperty(runtime, "y");

            if (yValue.isNumber())
            {
                node->bounds.y = static_cast<float>(yValue.asNumber());
                boundsChanged = true;
            }
        }

        if (nodeConfig.hasProperty(runtime, "width"))
        {
            auto widthValue = nodeConfig.getProperty(runtime, "width");

            if (widthValue.isNumber())
            {
                node->bounds.width = static_cast<float>(widthValue.asNumber());
                boundsChanged = true;
            }
        }

        if (nodeConfig.hasProperty(runtime, "height"))
        {
            auto heightValue = nodeConfig.getProperty(runtime, "height");

            if (heightValue.isNumber())
            {
                node->bounds.height = static_cast<float>(heightValue.asNumber());
                boundsChanged = true;
            }
        }

        if (nodeConfig.hasProperty(runtime, "fillColor"))
        {
            auto colorValue = nodeConfig.getProperty(runtime, "fillColor");

            if (colorValue.isString())
            {
                std::string colorStr = colorValue.asString(runtime).utf8(runtime);
                node->fillColor = Color::parseHexColor(colorStr);
            }
        }

        if (nodeConfig.hasProperty(runtime, "strokeColor"))
        {
            auto colorValue = nodeConfig.getProperty(runtime, "strokeColor");

            if (colorValue.isString())
            {
                std::string colorStr = colorValue.asString(runtime).utf8(runtime);
                node->strokeColor = Color::parseHexColor(colorStr);
            }
        }

        if (nodeConfig.hasProperty(runtime, "strokeWidth"))
        {
            auto strokeWidthValue = nodeConfig.getProperty(runtime, "strokeWidth");

            if (strokeWidthValue.isNumber())
            {
                node->strokeWidth = static_cast<float>(strokeWidthValue.asNumber());
            }
        }

        if (nodeConfig.hasProperty(runtime, "zIndex"))
        {
            auto zIndexValue = nodeConfig.getProperty(runtime, "zIndex");

            if (zIndexValue.isNumber())
            {
                node->zIndex = static_cast<int>(zIndexValue.asNumber());
            }
        }

        if (boundsChanged)
        {
            sceneGraph_->updateIndex(node);
        }
        return jsi::Value(true);
    }
    // Type conversion function
    std::unique_ptr<Node> SceneGraphJSI::jsObjectToNode(jsi::Runtime &runtime, const jsi::Object &obj)
    {

        auto node = std::make_unique<Node>();

        static int nodeCounter = 0;
        node->id = "node_" + std::to_string(++nodeCounter);

        if (obj.hasProperty(runtime, "x"))
        {
            auto xValue = obj.getProperty(runtime, "x");
            if (xValue.isNumber())
            {
                node->bounds.x = static_cast<float>(xValue.asNumber());
            }
        }

        if (obj.hasProperty(runtime, "y"))
        {
            auto yValue = obj.getProperty(runtime, "y");
            if (yValue.isNumber())
            {
                node->bounds.y = static_cast<float>(yValue.asNumber());
            }
        }

        if (obj.hasProperty(runtime, "width"))
        {
            auto widthValue = obj.getProperty(runtime, "width");
            if (widthValue.isNumber())
            {
                node->bounds.width = static_cast<float>(widthValue.asNumber());
            }
        }
        else
        {
            node->bounds.width = 100.0f;
        }

        if (obj.hasProperty(runtime, "height"))
        {
            auto heightValue = obj.getProperty(runtime, "height");
            if (heightValue.isNumber())
            {
                node->bounds.height = static_cast<float>(heightValue.asNumber());
            }
        }
        else
        {
            node->bounds.height = 100.0f;
        }

        if (obj.hasProperty(runtime, "fillColor"))
        {
            auto colorValue = obj.getProperty(runtime, "fillColor");

            if (colorValue.isString())
            {
                std::string colorStr = colorValue.asString(runtime).utf8(runtime);
                node->fillColor = Color::parseHexColor(colorStr);
            }
        }
        else
        {
            node->fillColor = Color::red();
        }

        node->strokeColor = Color::black();
        node->strokeWidth = 2.0f;

        if (obj.hasProperty(runtime, "zIndex"))
        {
            auto zValue = obj.getProperty(runtime, "zIndex");
            if (zValue.isNumber())
            {
                node->zIndex = static_cast<int>(zValue.asNumber());
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
        obj.setProperty(runtime, "id", jsi::String::createFromUtf8(runtime, node->id));
        obj.setProperty(runtime, "x", jsi::Value(node->bounds.x));
        obj.setProperty(runtime, "y", jsi::Value(node->bounds.y));
        obj.setProperty(runtime, "width", jsi::Value(node->bounds.width));
        obj.setProperty(runtime, "height", jsi::Value(node->bounds.height));
        obj.setProperty(runtime, "zIndex", jsi::Value(node->zIndex));

        obj.setProperty(runtime, "fillColor", jsi::String::createFromUtf8(runtime, node->fillColor.toHexColor()));

        return obj;
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