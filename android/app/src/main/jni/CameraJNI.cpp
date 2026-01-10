#include <jni.h>
#include "camera-state/CameraState.h"

using namespace CanvasMVP;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_canvasmvp_state_CameraState_nativeCreate(JNIEnv* env, jobject obj) {
    auto* camera = new CameraState();
    return reinterpret_cast<jlong>(camera);
}

JNIEXPORT void JNICALL
Java_com_canvasmvp_state_CameraState_nativeDestroy(JNIEnv* env, jobject obj, jlong handle) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    delete camera;
}

JNIEXPORT void JNICALL
Java_com_canvasmvp_state_CameraState_nativePan(
        JNIEnv* env, jobject obj, jlong handle, jfloat dx, jfloat dy
) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    camera->pan(dx, dy);
}

JNIEXPORT void JNICALL
Java_com_canvasmvp_state_CameraState_nativeZoomAt(
        JNIEnv* env, jobject obj, jlong handle,
        jfloat newZoom, jfloat centerX, jfloat centerY,
        jfloat viewWidth, jfloat viewHeight
) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    camera->zoomAt(
            newZoom,
            Point{centerX, centerY}

    );
}

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_state_CameraState_nativeWorldToScreen(
        JNIEnv* env, jobject obj, jlong handle,
        jfloat worldX, jfloat worldY,
        jfloat viewWidth, jfloat viewHeight
) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
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
        JNIEnv* env, jobject obj, jlong handle,
        jfloat worldX, jfloat worldY,
        jfloat viewWidth, jfloat viewHeight
) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
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
        JNIEnv* env, jobject obj, jlong handle,
        jfloat x, jfloat y,
        jfloat width, jfloat height
) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
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
        JNIEnv* env, jobject obj, jlong handle,
        jfloat x, jfloat y,
        jfloat width, jfloat height
) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    auto screen = camera->worldToScreen(
            RectF(x, y, width, height)

    );

    jfloatArray result = env->NewFloatArray(4);
    float coords[] = {screen.x, screen.y, screen.width, screen.height};
    env->SetFloatArrayRegion(result, 0, 4, coords);
    return result;
}
JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_state_CameraState_nativeGetZoom(JNIEnv* env, jobject obj, jlong handle) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    return camera->zoom;
}

JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_state_CameraState_nativeGetOffsetX(JNIEnv* env, jobject obj, jlong handle) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    return camera->offsetX;
}

JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_state_CameraState_nativeGetOffsetY(JNIEnv* env, jobject obj, jlong handle) {
    auto* camera = reinterpret_cast<CameraState*>(handle);
    return camera->offsetY;
}

} // extern "C"