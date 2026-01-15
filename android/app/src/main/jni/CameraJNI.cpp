#include <jni.h>
#include "camera-state/CameraState.h"
#include "jsi/CameraStateRegistry.h"

using namespace CanvasMVP;

extern "C" {


JNIEXPORT void JNICALL
Java_com_canvasmvp_state_CameraState_nativeDestroy(JNIEnv* env, jobject obj, jlong handle) {

}

JNIEXPORT void JNICALL
Java_com_canvasmvp_state_CameraState_nativePan(
        JNIEnv* env, jobject obj,  jfloat dx, jfloat dy
) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    if(camera == nullptr){
        return;
    }
    camera->pan(dx, dy);
}

JNIEXPORT void JNICALL
Java_com_canvasmvp_state_CameraState_nativeZoomAt(
        JNIEnv* env, jobject obj,
        jfloat newZoom, jfloat centerX, jfloat centerY,
        jfloat viewWidth, jfloat viewHeight
) {
    auto* camera =CanvasMVP::getCurrentCameraState();

    if(camera == nullptr){
        return;
    }
    camera->zoomAt(
            newZoom,
            Point{centerX, centerY}

    );
}

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_state_CameraState_nativeWorldToScreen(
        JNIEnv* env, jobject obj,
        jfloat worldX, jfloat worldY,
        jfloat viewWidth, jfloat viewHeight
) {
    auto* camera = CanvasMVP::getCurrentCameraState();

    Point screen = camera->worldToScreen(
            Point{worldX, worldY}

    );

    jfloatArray result = env->NewFloatArray(2);
    float coords[] = {screen.x, screen.y};
    env->SetFloatArrayRegion(result, 0, 2, coords);
    return result;
}
JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_state_CameraState_nativeScreenToWorld(
        JNIEnv* env, jobject obj,
        jfloat worldX, jfloat worldY,
        jfloat viewWidth, jfloat viewHeight
) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    Point screen = camera->screenToWorld(
            Point{worldX, worldY}

    );

    jfloatArray result = env->NewFloatArray(2);
    float coords[] = {screen.x, screen.y};
    env->SetFloatArrayRegion(result, 0, 2, coords);
    return result;
}

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_state_CameraState_nativeScreenToWorldRect(
        JNIEnv* env, jobject obj,
        jfloat x, jfloat y,
        jfloat width, jfloat height
) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    auto screen = camera->screenToWorld(
            RectF(x, y, width, height)

    );

    jfloatArray result = env->NewFloatArray(4);
    float coords[] = {screen.x, screen.y, screen.width, screen.height};
    env->SetFloatArrayRegion(result, 0, 4, coords);
    return result;
}

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_state_CameraState_nativeWorldToScreenRect(
        JNIEnv* env, jobject obj,
        jfloat x, jfloat y,
        jfloat width, jfloat height
) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    auto screen = camera->worldToScreen(
            RectF(x, y, width, height)

    );

    jfloatArray result = env->NewFloatArray(4);
    float coords[] = {screen.x, screen.y, screen.width, screen.height};
    env->SetFloatArrayRegion(result, 0, 4, coords);
    return result;
}
JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_state_CameraState_nativeGetZoom(JNIEnv* env, jobject obj) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    return camera->zoom;
}

JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_state_CameraState_nativeGetOffsetX(JNIEnv* env, jobject obj) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    return camera->offsetX;
}

JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_state_CameraState_nativeGetOffsetY(JNIEnv* env, jobject obj) {
    auto* camera = CanvasMVP::getCurrentCameraState();
    return camera->offsetY;
}

} // extern "C"