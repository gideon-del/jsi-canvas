#pragma once

#include <RNCanvasMVPSpecJSI.h>

namespace facebook::react
{
    class CameraStateModule : public NativeCameraStateModuleCxxSpec<CameraStateModule>
    {
    public:
        CameraStateModule(std::shared_ptr<CallInvoker> jsInvoker);

        bool installCameraState(jsi::Runtime &runtime);
    };
}