package com.canvasmvp.canvas

import android.content.Context
import android.graphics.Canvas
import android.view.View
import android.graphics.Paint
import android.graphics.Color
import android.util.Log
import com.canvasmvp.state.CameraState
import kotlin.math.floor
import androidx.core.graphics.withClip

class CanvasView(context: Context): View(context) {

    private val camera = CameraState()


    private val lightGridPaint = Paint().apply {
        color = Color.LTGRAY
        style = Paint.Style.STROKE
        strokeWidth = 1f
        isAntiAlias = true
    }

    private val darkGridPaint = Paint().apply {
        color = Color.GRAY
        style = Paint.Style.STROKE
        strokeWidth = 2f
        isAntiAlias = true
    }

    private val originPaint = Paint().apply {
        color = Color.RED
        style = Paint.Style.STROKE
        strokeWidth = 3f
        isAntiAlias = true
    }

    init {
        setWillNotDraw(false)

        Log.d(TAG, "✅ Camera initialized - offset:(${camera.offsetX}, ${camera.offsetY}) zoom:${camera.zoom}")
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.withClip(0f, 0f, width.toFloat(), height.toFloat()) {
            drawColor(Color.WHITE)

            translate(camera.offsetX, camera.offsetY)
            scale(camera.zoom, camera.zoom)


            drawGrid(this)

            drawOrigin(this)
        }
    }

    private fun drawGrid(canvas: Canvas) {
        val gridSpacing = 100f
        val minorSpacing = 20f

        val viewWidth = width.toFloat()
        val viewHeight = height.toFloat()
        val worldTopLeft = camera.screenToWorldPoint(0f,0f,viewWidth, viewHeight)
        val worldLeft = worldTopLeft[0]
        val worldTop = worldTopLeft[1]

        val worldBottomRight = camera.screenToWorldPoint(viewWidth,viewHeight,viewWidth, viewHeight);
        val worldRight = worldBottomRight[0]
        val worldBottom = worldBottomRight[1]

        val startX = floor(worldLeft / minorSpacing) * minorSpacing
        val startY = floor(worldTop / minorSpacing) * minorSpacing
        val endX = floor(worldRight/minorSpacing) * minorSpacing
        val endY = floor(worldBottom /minorSpacing) * minorSpacing

        var x = startX

        while(x <= endX) {
         val isMajorLine = x % gridSpacing == 0f
         val screenTopLeft = camera.worldToScreen(x, worldTop, viewWidth, viewHeight)
         val screenBottomRight = camera.worldToScreen(x, worldBottom, viewWidth, viewHeight)
         val paint = if(isMajorLine) darkGridPaint else lightGridPaint

         canvas.drawLine(
             screenTopLeft[0],
             screenTopLeft[1],
             screenBottomRight[0],
             screenBottomRight[1], paint)

         x += minorSpacing
        }
       var y = startY
        while(y <= endY) {
            val isMajorLine = y % gridSpacing == 0f
            val screenTopLeft = camera.worldToScreen(worldLeft, y, viewWidth, viewHeight)
            val screenBottomRight = camera.worldToScreen(worldRight, y, viewWidth, viewHeight)
            val paint = if(isMajorLine) darkGridPaint else lightGridPaint

            canvas.drawLine(
                screenTopLeft[0],
                screenTopLeft[1],
                screenBottomRight[0],
                screenBottomRight[1], paint)

            y += minorSpacing
        }

    }

    private fun drawOrigin(canvas: Canvas) {
        val viewWidth = width.toFloat()
        val viewHeight = height.toFloat()
        val originScreen = camera.worldToScreen(0f,0f, viewWidth, viewHeight)
        val originScreenX =originScreen[0]
        val originScreenY = originScreen[1]

        val crossSize = 20f

        // Horizontal line
        canvas.drawLine(
            originScreenX - crossSize, originScreenY,
            originScreenX + crossSize, originScreenY,
            originPaint
        )

        // Vertical line
        canvas.drawLine(
            originScreenX, originScreenY - crossSize,
            originScreenX, originScreenY + crossSize,
            originPaint
        )
    }
    override fun onLayout(changed: Boolean, left: Int, top: Int, right: Int, bottom: Int) {
        super.onLayout(changed, left, top, right, bottom)
        if(changed) invalidate()
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        Log.d("CanvasView", "Size changed to: $w x $h")


        invalidate()
    }


    companion object {
        const val TAG = "CanvasView"
    }


}