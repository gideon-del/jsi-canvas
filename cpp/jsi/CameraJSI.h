#pragma once
#include <jsi/jsi.h>
#include "../camera-state/CameraState.h"

namespace CanvasMVP
{
    using namespace facebook;

    class CameraJSI : public jsi::HostObject
    {
    private:
        std::shared_ptr<CameraState> cameraState;

    public:
        CameraJSI(std::shared_ptr<CameraState> cameraState);

        jsi::Value get(jsi::Runtime &runtime, const jsi::PropNameID &name) override;
        void set(jsi::Runtime &runtime, const jsi::PropNameID &name, const jsi::Value &value) override;

        jsi::Object screenToWorld(jsi::Runtime &runtime, float x, float y);
        jsi::Object worldToScreen(jsi::Runtime &runtime, float x, float y);

        jsi::Value getZoom();
        jsi::Object getOffset(jsi::Runtime &runtime);
    };

    bool installCameraState(jsi::Runtime &runtime);
}