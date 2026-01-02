package com.canvasmvp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.view.View
import kotlin.math.floor
import kotlin.math.truncate

class TransformedGridView(context: Context) : View(context) {

    private var offsetX = 20f
    private var offsetY = 20f
    private var zoom = 2f

    private val lightGridPaint = Paint().apply {
        // TODO: Light gray color
        // TODO: Style STROKE
        // TODO: Width 1
        color = Color.LTGRAY
        style = Paint.Style.STROKE
        strokeWidth = 1f
    }

    private val darkGridPaint = Paint().apply {
        // TODO: Darker gray color
        // TODO: Style STROKE
        // TODO: Width 2
        color = Color.DKGRAY
        style = Paint.Style.STROKE
        strokeWidth = 2f
    }

    private val originPaint = Paint().apply {
        // TODO: Red color
        // TODO: Style STROKE
        // TODO: Width 3
        color = Color.RED
        style = Paint.Style.STROKE
        strokeWidth = 3f
    }


    init {
        setWillNotDraw(false)
    }
    private  val gridSpacing = 50f
    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawColor(Color.WHITE)

        // TODO: Save canvas state
        canvas.save()
        // TODO: Apply camera transform
        // Hint: First translate, then scale
         canvas.translate(offsetX, offsetY)
        canvas.scale(zoom, zoom)
        // TODO: Calculate visible world bounds
        // Use screenToWorld on corner points
        val startX = worldToScreenX(0f)
        val endX = worldToScreenX(width.toFloat())
        val startY = worldToScreenY(0f)
        val endY = worldToScreenY(height.toFloat())

        // TODO: Draw only visible grid lines
        val minX = (floor(startX / gridSpacing) * gridSpacing)
        val minY = (floor(startY / gridSpacing) * gridSpacing)
        val maxX =  (truncate(endX / gridSpacing) * gridSpacing)
        val maxY =  (truncate(endY / gridSpacing) * gridSpacing)

        var x = minX
      // Vertical lines
       while (x <= maxX){
           canvas.drawLine(x, minY, x, maxY,lightGridPaint)
           x += gridSpacing
       }
var y = minY
        // Horizontal lines
        while (y <= maxY){
            canvas.drawLine(minX, y, maxX, y,lightGridPaint)
            y += gridSpacing
        }
        // TODO: Draw origin marker
        val markerSize = 60f
        canvas.drawLine(-markerSize, 0f,markerSize,0f, originPaint)
        canvas.drawLine(0f, -markerSize,0f,markerSize, originPaint)
        // TODO: Restore canvas state
        canvas.restore()
    }

    // Helper functions
    private fun screenToWorldX(screenX: Float): Float {
        return screenX / zoom - offsetX
    }

    private fun screenToWorldY(screenY: Float): Float {
        return screenY / zoom - offsetY
    }

    private fun worldToScreenX(worldX: Float): Float {
        return (worldX + offsetX) * zoom
    }

    private fun worldToScreenY(worldY: Float): Float {
        return (worldY + offsetY) * zoom
    }
}