#include "CameraStateModule.h"
#include "CameraJSI.h"
namespace facebook::react
{
    CameraStateModule::CameraStateModule(std::shared_ptr<CallInvoker> jsInvoker) : NativeCameraStateModuleCxxSpec<CameraStateModule>(std::move(jsInvoker)) {};

    bool CameraStateModule::installCameraState(jsi::Runtime &runtime)
    {
        CanvasMVP::installCameraState(runtime);
        return true;
    };
}