package com.canvasmvp.state

import com.canvasmvp.types.CanvasTypes

class CameraState {






    fun pan(dx: Float, dy: Float) {
        nativePan( dx, dy)
    }

    fun zoomAt(newZoom: Float, centerX: Float, centerY: Float,
               viewWidth: Float, viewHeight: Float) {
        nativeZoomAt( newZoom, centerX, centerY, viewWidth, viewHeight)
    }

    fun worldToScreen(screenX: Float, screenY: Float,
                      viewWidth: Float, viewHeight: Float): FloatArray {
        return nativeWorldToScreen( screenX, screenY, viewWidth, viewHeight)
    }
    fun screenToWorldPoint(worldX: Float, worldY: Float,
                           viewWidth: Float, viewHeight: Float): FloatArray {
        return nativeScreenToWorld( worldX, worldY, viewWidth, viewHeight)
    }

    fun worldToScreenRect(rect: CanvasTypes.Rect): CanvasTypes.Rect {
        val screenRect =  nativeWorldToScreenRect(
            rect.x,
            rect.y,
            rect.width,
            rect.height
            )
        return CanvasTypes.Rect(
            x = screenRect[0],
            y=screenRect[1],
            width = screenRect[2],
            height = screenRect[3]
        )

    }
    fun screenToWorldRect(rect: CanvasTypes.Rect): CanvasTypes.Rect {
        val worldRect =  nativeScreenToWorldRect(
            rect.x,
            rect.y,
            rect.width,
            rect.height
        )
        return CanvasTypes.Rect(
            x = worldRect[0],
            y=worldRect[1],
            width = worldRect[2],
            height = worldRect[3]
        )

    }
    val zoom: Float
        get() = nativeGetZoom()

    val offsetX: Float
        get() = nativeGetOffsetX()

    val offsetY: Float
        get() = nativeGetOffsetY()

    // Native methods

    private external fun nativePan( dx: Float, dy: Float)
    private external fun nativeZoomAt( newZoom: Float,
                                      centerX: Float, centerY: Float,
                                      viewWidth: Float, viewHeight: Float)
    private external fun nativeWorldToScreen(
                                             screenX: Float, screenY: Float,
                                             viewWidth: Float, viewHeight: Float): FloatArray
    private external fun nativeScreenToWorld(
                                             worldX: Float, worldY: Float,
                                             viewWidth: Float, viewHeight: Float): FloatArray

    private  external fun nativeScreenToWorldRect( x: Float, y: Float, width: Float, height: Float): FloatArray
    private  external fun nativeWorldToScreenRect( x: Float, y: Float, width: Float, height: Float): FloatArray
    private external fun nativeGetZoom(): Float
    private external fun nativeGetOffsetX(): Float
    private external fun nativeGetOffsetY(): Float




}