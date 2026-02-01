#include "JSInvokerRegistery.h"

namespace CanvasMVP
{
    static std::shared_ptr<facebook::react::CallInvoker> s_jsCallInvoker;

    void setJSCallInvoker(std::shared_ptr<facebook::react::CallInvoker> invoker)
    {
        s_jsCallInvoker = invoker;
    }

    std::shared_ptr<facebook::react::CallInvoker> getJSCallInvoker()
    {
        return s_jsCallInvoker;
    }
}