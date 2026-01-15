#include "CameraJSI.h"
#include "CameraStateRegistry.h"
namespace CanvasMVP
{
    using namespace facebook;

    CameraJSI::CameraJSI(std::shared_ptr<CameraState> cameraState) : cameraState(cameraState) {};

    jsi::Value CameraJSI::get(jsi::Runtime &runtime, const jsi::PropNameID &name)
    {

        auto propName = name.utf8(runtime);

        if (propName == "screenToWorld")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                2,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count)
                {
                    if (count != 2)
                    {
                        throw jsi::JSError(rt, "screenToWorld requires 2 argument");
                    }

                    if (!args[0].isNumber() || !args[1].isNumber())
                    {
                        throw jsi::JSError(rt, "x and y must be numbers");
                    }

                    return screenToWorld(rt, static_cast<float>(args[0].asNumber()), static_cast<float>(args[1].asNumber()));
                });
        };

        if (propName == "worldToScreen")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                2,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count)
                {
                    if (count != 2)
                    {
                        throw jsi::JSError(rt, "screenToWorld requires 2 argument");
                    }

                    if (!args[0].isNumber() || !args[1].isNumber())
                    {
                        throw jsi::JSError(rt, "x and y must be numbers");
                    }

                    return worldToScreen(rt, static_cast<float>(args[0].asNumber()), static_cast<float>(args[1].asNumber()));
                });
        };
        if (propName == "getZoom")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count)
                {
                    return getZoom();
                });
        };
        if (propName == "getOffset")
        {
            return jsi::Function::createFromHostFunction(
                runtime,
                name,
                0,
                [this](jsi::Runtime &rt, const jsi::Value &thisVal, const jsi::Value *args, size_t count)
                {
                    return getOffset(rt);
                });
        };

        return jsi::Value::undefined();
    }
    void CameraJSI::set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value)
    {
        throw jsi::JSError(runtime, "Canvas Camera properties are readonly");
    }
    jsi::Object CameraJSI::screenToWorld(jsi::Runtime &runtime, float x, float y)
    {
        auto worldPoint = cameraState->screenToWorld(Point{
            x, y});

        auto obj = jsi::Object(runtime);

        obj.setProperty(runtime, "x", jsi::Value(worldPoint.x));
        obj.setProperty(runtime, "y", jsi::Value(worldPoint.y));
        return obj;
    }
    jsi::Object CameraJSI::worldToScreen(jsi::Runtime &runtime, float x, float y)
    {
        auto screenPoint = cameraState->worldToScreen(Point{
            x, y});

        auto obj = jsi::Object(runtime);

        obj.setProperty(runtime, "x", jsi::Value(screenPoint.x));
        obj.setProperty(runtime, "y", jsi::Value(screenPoint.y));
        return obj;
    }

    jsi::Value CameraJSI::getZoom()
    {
        return jsi::Value(cameraState->zoom);
    }
    jsi::Object CameraJSI::getOffset(jsi::Runtime &runtime)
    {
        auto obj = jsi::Object(runtime);

        obj.setProperty(runtime, "x", jsi::Value(cameraState->offsetX));
        obj.setProperty(runtime, "y", jsi::Value(cameraState->offsetY));
        return obj;
    }

    bool installCameraState(jsi::Runtime &runtime)
    {
        auto camera = std::make_shared<CameraState>();

        setCurrentCameraState(camera.get());
        auto hostObject = std::make_shared<CameraJSI>(camera);

        runtime.global().setProperty(
            runtime,
            "canvasCamera",
            jsi::Object::createFromHostObject(runtime, hostObject));

        return true;
    }
}