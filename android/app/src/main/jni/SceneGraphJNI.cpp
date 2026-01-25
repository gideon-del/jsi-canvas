#include "jsi/SceneGraphJSI.h"
#include "jsi/SceneGraphRegistry.h"
#include <jni.h>
#include <string>
#include <memory>
#include "jsi/jsi.h"

using namespace CanvasMVP;
extern "C" {
extern JavaVM* g_jvm;
static std::unordered_map<int, jobject> g_javaCallbacks;


JNIEXPORT void JNICALL
Java_com_canvasmvp_graph_SceneGraphWrapper_destroyNative(JNIEnv* env, jobject obj, jlong handle) {



}


JNIEXPORT jint JNICALL
Java_com_canvasmvp_graph_SceneGraphWrapper_nativeAddEventListener(
        JNIEnv* env,
        jobject obj,
        jstring eventTypeStr,
        jobject eventCallback
        ) {
   auto graph = CanvasMVP::getCurrentSceneGraph();
   if(graph == nullptr) {
       return -1;
   }

   const char* eventTypeChars = env->GetStringUTFChars(eventTypeStr, nullptr);

   std::string eventType(eventTypeChars);
   env->ReleaseStringUTFChars(eventTypeStr, eventTypeChars);
  jobject globalCallback = env->NewGlobalRef(eventCallback);
  int listenerId = graph->addEventListener(eventType, [globalCallback]() {
      if(!g_jvm)return;
      JNIEnv* env;
      bool attached = false;

      if(g_jvm->GetEnv((void** )&env, JNI_VERSION_1_6) != JNI_OK){
          g_jvm->AttachCurrentThread(&env, nullptr);
          attached=true;
      }
      jclass callbackClass = env->GetObjectClass(globalCallback);
      jmethodID invokeMethod = env->GetMethodID(callbackClass,"invoke","()Ljava/lang/Object;");
      if(invokeMethod){
          env->CallObjectMethod(globalCallback, invokeMethod);
      }
      env->DeleteLocalRef(callbackClass);
      if (env->ExceptionCheck()) {
          env->ExceptionDescribe();
          env->ExceptionClear();
      }
      if(attached){
          g_jvm->DetachCurrentThread();
      }
  });

  return listenerId;
}

JNIEXPORT void JNICALL
Java_com_canvasmvp_graph_SceneGraphWrapper_nativeRemoveEventListener(
        JNIEnv* env,
        jobject obj,
        jint listenerId,
        jstring eventTypeStr
        ) {
    auto graph = CanvasMVP::getCurrentSceneGraph();
    if(!graph) return;
    const char* eventTypeChars = env->GetStringUTFChars(eventTypeStr, nullptr);

    std::string eventType(eventTypeChars);
    env->ReleaseStringUTFChars(eventTypeStr, eventTypeChars);

    graph->removeEventListener(eventType, listenerId);
}
// Node operations
JNIEXPORT jboolean JNICALL
Java_com_canvasmvp_graph_SceneGraphWrapper_addNodeNative(
        JNIEnv* env, jobject obj,
        jstring jid, jfloat x, jfloat y, jfloat width, jfloat height,
        jfloat fillR, jfloat fillG, jfloat fillB, jfloat fillA,
jfloat strokeR, jfloat strokeG, jfloat strokeB, jfloat strokeA,
jfloat strokeWidth, jint zIndex
        ) {

const char* idChars = env->GetStringUTFChars(jid, nullptr);
std::string id(idChars);
env->ReleaseStringUTFChars(jid, idChars);


 auto node = std::make_unique<Node>(id, NodeType::RECTANGLE, x, y, width, height);
  node->fillColor = Color(fillR, fillG, fillB, fillA);
  node->strokeColor = Color(strokeR, strokeG, strokeB, strokeA);
  node->strokeWidth = strokeWidth;
  node->zIndex = zIndex;
auto graph = CanvasMVP::getCurrentSceneGraph();

   if(graph == nullptr){
       return false;
   }
  return graph->addNode(std::move(node));
 }


 JNIEXPORT void JNICALL
Java_com_canvasmvp_graph_SceneGraphWrapper_clearNative(JNIEnv* env, jobject obj) {
     auto graph = CanvasMVP::getCurrentSceneGraph();
     if(graph == nullptr) {
         return;
     }
     graph->clear();
 }
 JNIEXPORT jint JNICALL
 Java_com_canvasmvp_graph_SceneGraphWrapper_nodeCountNative(JNIEnv* env, jobject obj){
     auto graph = CanvasMVP::getCurrentSceneGraph();
     if(graph == nullptr){
         return  0;
     }

     return static_cast<jint>(graph->nodeCount());
 }



 // Query operations

 JNIEXPORT jlongArray JNICALL
 Java_com_canvasmvp_graph_SceneGraphWrapper_queryVisibleNative(
         JNIEnv* env, jobject obj,
         jfloat x, jfloat y, jfloat w, jfloat h
         ){
Rect viewPort(x,y,w,h);
     auto graph = CanvasMVP::getCurrentSceneGraph();

    auto visible = graph->queryVisible(viewPort);

jlongArray  result = env->NewLongArray(visible.size());
     if(result == nullptr) return nullptr;
     std::vector<jlong> ptrs;
     ptrs.reserve(visible.size());
     for(Node* node: visible){
         ptrs.push_back(reinterpret_cast<jlong>(node));
     }

     env->SetLongArrayRegion(result,0, ptrs.size(), ptrs.data());

     return  result;
 }

 // Node data access
JNIEXPORT jstring JNICALL
Java_com_canvasmvp_graph_NodeWrapper_getNodeIdNative(
        JNIEnv* env, jobject obj, jlong handle
        ) {
     auto* node = reinterpret_cast<Node*>(handle);
    return env->NewStringUTF(node->id.c_str());
 }

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_graph_NodeWrapper_getNodeBoundsNative(JNIEnv* env, jobject obj, jlong nodePtr) {
    auto* node = reinterpret_cast<Node*>(nodePtr);

    jfloatArray result = env->NewFloatArray(4);
    jfloat bounds[4] = {
            node->bounds.x,
            node->bounds.y,
            node->bounds.width,
            node->bounds.height
    };
    env->SetFloatArrayRegion(result, 0, 4, bounds);
    return result;
}

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_graph_NodeWrapper_getNodeFillColorNative(JNIEnv* env, jobject obj, jlong nodePtr) {
    auto* node = reinterpret_cast<Node*>(nodePtr);

    jfloatArray result = env->NewFloatArray(4);
    jfloat color[4] = {
            node->fillColor.r,
            node->fillColor.g,
            node->fillColor.b,
            node->fillColor.a
    };
    env->SetFloatArrayRegion(result, 0, 4, color);
    return result;
}

JNIEXPORT jfloatArray JNICALL
Java_com_canvasmvp_graph_NodeWrapper_getNodeStrokeColorNative(JNIEnv* env, jobject obj, jlong nodePtr) {
    auto* node = reinterpret_cast<Node*>(nodePtr);

    jfloatArray result = env->NewFloatArray(4);
    jfloat color[4] = {
            node->strokeColor.r,
            node->strokeColor.g,
            node->strokeColor.b,
            node->strokeColor.a
    };
    env->SetFloatArrayRegion(result, 0, 4, color);
    return result;
}
JNIEXPORT jfloat JNICALL
Java_com_canvasmvp_graph_NodeWrapper_getNodeStrokeWidthNative(JNIEnv* env, jobject obj, jlong nodePtr) {
    auto* node = reinterpret_cast<Node*>(nodePtr);
    return node->strokeWidth;
}
JNIEXPORT jint JNICALL
Java_com_canvasmvp_graph_NodeWrapper_getNodeZIndexNative(JNIEnv* env, jobject obj, jlong nodePtr) {
    auto* node = reinterpret_cast<Node*>(nodePtr);
    return node->zIndex;
}
}