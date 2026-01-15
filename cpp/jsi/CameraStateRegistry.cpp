#include "CameraStateRegistry.h"
#include <iostream>
namespace CanvasMVP
{
    static std::atomic<CameraState *> c_currentCamera{nullptr};

    CameraState *getCurrentCameraState()
    {
        return c_currentCamera.load(std::memory_order_acquire);
    }

    void setCurrentCameraState(CameraState *camera)
    {
        c_currentCamera.store(camera, std::memory_order_release);
    }
}