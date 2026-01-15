#include "SceneGraphModule.h"
#include "SceneGraphJSI.h"
namespace facebook::react
{
    SceneGraphModule::SceneGraphModule(std::shared_ptr<CallInvoker> jsInvoker)
        : NativeSceneGraphModuleCxxSpec<SceneGraphModule>(std::move(jsInvoker)) {};

    void SceneGraphModule::installSceneGraph(jsi::Runtime &runtime)
    {
        CanvasMVP::installSceneGraph(runtime);
    };
} // namespace facebook::react