#pragma once

#include <RNCanvasMVPSpecJSI.h>

namespace facebook::react
{
    class SceneGraphModule : public NativeSceneGraphModuleCxxSpec<SceneGraphModule>
    {
    public:
        SceneGraphModule(std::shared_ptr<CallInvoker> jsInvoker);

        bool installSceneGraph(jsi::Runtime &runtime);
    };
}; // namespace facebook::react