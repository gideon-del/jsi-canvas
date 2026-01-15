#pragma once
#include "../camera-state/CameraState.h"
namespace CanvasMVP
{
    CameraState *getCurrentCameraState();
    void setCurrentCameraState(CameraState *camera);
}