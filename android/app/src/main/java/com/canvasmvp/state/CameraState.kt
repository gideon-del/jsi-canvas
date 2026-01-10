package com.canvasmvp.state

import com.canvasmvp.types.CanvasTypes

class CameraState {
    private var nativeHandle: Long = 0

    init {
        nativeHandle = nativeCreate()
    }

    fun destroy() {
        if (nativeHandle != 0L) {
            nativeDestroy(nativeHandle)
            nativeHandle = 0
        }
    }

    fun pan(dx: Float, dy: Float) {
        nativePan(nativeHandle, dx, dy)
    }

    fun zoomAt(newZoom: Float, centerX: Float, centerY: Float,
               viewWidth: Float, viewHeight: Float) {
        nativeZoomAt(nativeHandle, newZoom, centerX, centerY, viewWidth, viewHeight)
    }

    fun worldToScreen(screenX: Float, screenY: Float,
                      viewWidth: Float, viewHeight: Float): FloatArray {
        return nativeWorldToScreen(nativeHandle, screenX, screenY, viewWidth, viewHeight)
    }
    fun screenToWorldPoint(worldX: Float, worldY: Float,
                           viewWidth: Float, viewHeight: Float): FloatArray {
        return nativeScreenToWorld(nativeHandle, worldX, worldY, viewWidth, viewHeight)
    }

    fun worldToScreenRect(rect: CanvasTypes.Rect): CanvasTypes.Rect {
        val screenRect =  nativeWorldToScreenRect(nativeHandle,
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
        val worldRect =  nativeScreenToWorldRect(nativeHandle,
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
        get() = nativeGetZoom(nativeHandle)

    val offsetX: Float
        get() = nativeGetOffsetX(nativeHandle)

    val offsetY: Float
        get() = nativeGetOffsetY(nativeHandle)

    // Native methods
    private external fun nativeCreate(): Long
    private external fun nativeDestroy(handle: Long)
    private external fun nativePan(handle: Long, dx: Float, dy: Float)
    private external fun nativeZoomAt(handle: Long, newZoom: Float,
                                      centerX: Float, centerY: Float,
                                      viewWidth: Float, viewHeight: Float)
    private external fun nativeWorldToScreen(handle: Long,
                                             screenX: Float, screenY: Float,
                                             viewWidth: Float, viewHeight: Float): FloatArray
    private external fun nativeScreenToWorld(handle: Long,
                                             worldX: Float, worldY: Float,
                                             viewWidth: Float, viewHeight: Float): FloatArray

    private  external fun nativeScreenToWorldRect(handle: Long, x: Float, y: Float, width: Float, height: Float): FloatArray
    private  external fun nativeWorldToScreenRect(handle: Long, x: Float, y: Float, width: Float, height: Float): FloatArray
    private external fun nativeGetZoom(handle: Long): Float
    private external fun nativeGetOffsetX(handle: Long): Float
    private external fun nativeGetOffsetY(handle: Long): Float




}