package com.canvasmvp.graph

import com.canvasmvp.types.CanvasTypes

class NodeWrapper(private val nodePtr: Long) {
  val id: String
      get() = getNodeIdNative(nodePtr)

 val bounds: CanvasTypes.Rect
     get() {
      val bound = getNodeBoundsNative(nodePtr)
      return CanvasTypes.Rect(
          x = bound[0],
          y= bound[1],
          width = bound[2],
          height = bound[3]
      )

     }
 val fillColor: CanvasTypes.Color
     get() {
         val color = getNodeFillColorNative(nodePtr)
         return CanvasTypes.Color(
             r = color[0],
             g= color[1],
             b=color[2],
             a=color[3]
         )
     }
val strokeColor: CanvasTypes.Color
    get() {
        val color = getNodeStrokeColorNative(nodePtr)
        return CanvasTypes.Color(
            r = color[0],
            g= color[1],
            b=color[2],
            a=color[3]
        )
    }
val strokeWidth: Float
    get() = getNodeStrokeWidthNative(nodePtr)

val zIndex: Int
    get() = getNodeZIndexNative(nodePtr)

   private external fun getNodeIdNative(handle: Long): String
   private external fun getNodeBoundsNative(handle: Long): FloatArray

    private external fun getNodeFillColorNative(handle: Long): FloatArray
    private external fun getNodeStrokeColorNative(handle: Long): FloatArray
    private external fun getNodeStrokeWidthNative(handle: Long): Float

    private external fun  getNodeZIndexNative(handle: Long): Int
}