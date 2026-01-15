package com.canvasmvp.graph

import com.canvasmvp.types.CanvasTypes

class SceneGraphWrapper {


    fun release() {

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
        clearNative()
    }

    fun nodeCount(): Int {
        return  nodeCountNative();
    }



    // Query operation
  fun queryVisible(viewPort: CanvasTypes.Rect): List<NodeWrapper> {
      val nodePtrs = queryVisibleNative(
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




    private external fun destroyNative(handle: Long)
    private external fun addNodeNative(id: String,
                                       x: Float, y: Float, width: Float, height: Float,
                                       fillR: Float, fillG: Float, fillB: Float, fillA: Float,
                                       strokeR: Float, strokeG: Float, strokeB: Float, strokeA: Float,
                                       strokeWidth: Float, zIndex: Int
    ): Boolean

    private external fun clearNative()
    private external fun nodeCountNative(): Int
    private external fun queryVisibleNative( x: Float, y:Float, width:Float, height: Float): LongArray
}