#include "SceneGraphJSI.h"
#include "SceneGraphRegistry.h"
#include <iostream>
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

        return jsi::Value::undefined();
    }

    void SceneGraphJSI::set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value)
    {
        throw jsi::JSError(runtime, "SceneGraph properties are readonly");
    }

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
                if (colorStr[0] == '#' && colorStr.length() == 7)
                {
                    unsigned int r, g, b;

                    sscanf(colorStr.c_str(), "#%02x%02x%02x", &r, &g, &b);
                    node->fillColor = Color{r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
                }
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

    jsi::Object SceneGraphJSI::nodeToJSObject(jsi::Runtime &runtime, const Node *node)
    {
        auto obj = jsi::Object(runtime);
        obj.setProperty(runtime, "id", jsi::String::createFromUtf8(runtime, node->id));
        obj.setProperty(runtime, "x", jsi::Value(node->bounds.x));
        obj.setProperty(runtime, "y", jsi::Value(node->bounds.y));
        obj.setProperty(runtime, "width", jsi::Value(node->bounds.width));
        obj.setProperty(runtime, "height", jsi::Value(node->bounds.height));
        obj.setProperty(runtime, "zIndex", jsi::Value(node->zIndex));

        char fillColorHex[8];
        snprintf(fillColorHex, sizeof(fillColorHex), "#%02x%02x%02x",
                 (int)(node->fillColor.r * 255),
                 (int)(node->fillColor.g * 255),
                 (int)(node->fillColor.b * 255)

        );

        obj.setProperty(runtime, "fillColor", jsi::String::createFromUtf8(runtime, fillColorHex));

        return obj;
    }

    void installSceneGraph(jsi::Runtime &runtime)
    {
        auto sceneGraph = std::make_shared<SceneGraph>();

        auto hostObject = std::make_shared<SceneGraphJSI>(sceneGraph);

        CanvasMVP::setCurrentSceneGraph(sceneGraph.get());

        runtime.global().setProperty(
            runtime,
            "sceneGraph",
            jsi::Object::createFromHostObject(runtime, hostObject));
    }
}