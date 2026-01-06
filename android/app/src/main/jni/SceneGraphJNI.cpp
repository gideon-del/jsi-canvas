#include <jni.h>
#include "scene-graph/SceneGraph.h"

extern "C" JNIEXPORT void JNICALL
Java_com_canvasmvp_CanvasView_testCppImport(JNIEnv* env, jobject obj) {
    CanvasMVP::SceneGraph graph;
    // We can access shared C++!
}