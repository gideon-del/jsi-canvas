package com.canvasmvp.graph

import com.canvasmvp.types.CanvasTypes

class SceneGraphWrapper {
    private var  nativeHandle: Long =0
    init {
nativeHandle = createNative()
    }

    fun release() {
        if(nativeHandle != 0L){
            destroyNative(nativeHandle)
            nativeHandle= 0
        }
    }

    // Node operations
    fun addNode(
        id: String,
        bounds: CanvasTypes.Rect,
        fillColor: CanvasTypes.Color,
        strokeColor: CanvasTypes.Color,
        strokeWidth: Float,
        zIndx: Int
                ): Boolean {
     return addNodeNative(
         nativeHandle, 
         id,
         bounds.x,
         bounds.y,
         bounds.width,
         bounds.height,
         fillColor.r,
         fillColor.g,
         fillColor.b,
         fillColor.a,
         strokeColor.r,
         strokeColor.g,
         strokeColor.b,
         strokeColor.a,
         strokeWidth,
         zIndx
         )
    }
    fun  clear() {
        clearNative(nativeHandle)
    }

    fun nodeCount(): Int {
        return  nodeCountNative(nativeHandle);
    }



    // Query operation
  fun queryVisible(viewPort: CanvasTypes.Rect): List<NodeWrapper> {
      val nodePtrs = queryVisibleNative(nativeHandle,
          viewPort.x,
          viewPort.y,
          viewPort.width,
          viewPort.height
          );

        val results = mutableListOf<NodeWrapper>()
        nodePtrs.forEach {
            results.add(NodeWrapper(it))
        }

        return  results
  }



    private  external fun createNative(): Long
    private external fun destroyNative(handle: Long)
    private external fun addNodeNative(handle: Long,id: String,
                                       x: Float, y: Float, width: Float, height: Float,
                                       fillR: Float, fillG: Float, fillB: Float, fillA: Float,
                                       strokeR: Float, strokeG: Float, strokeB: Float, strokeA: Float,
                                       strokeWidth: Float, zIndex: Int
    ): Boolean

    private external fun clearNative(handle: Long)
    private external fun nodeCountNative(handle: Long): Int
    private external fun queryVisibleNative(handle: Long, x: Float, y:Float, width:Float, height: Float): LongArray
}