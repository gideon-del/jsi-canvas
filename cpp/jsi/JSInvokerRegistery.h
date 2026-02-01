#pragma once
#include <ReactCommon/CallInvoker.h>
namespace CanvasMVP
{
    void setJSCallInvoker(std::shared_ptr<facebook::react::CallInvoker>);
    std::shared_ptr<facebook::react::CallInvoker> getJSCallInvoker();
}