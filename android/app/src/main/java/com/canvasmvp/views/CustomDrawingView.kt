package com.canvasmvp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.Log
import android.view.View

class CustomDrawingView(context: Context) : View(context) {

    // Paint objects define HOW to draw
    private val blueFillPaint = Paint().apply {
        // TODO: Set color to blue
        // TODO: Set style to FILL
        color = Color.BLUE
        style = Paint.Style.FILL

    }

    private val redFillPaint = Paint().apply {
        // TODO: Set color to blue
        // TODO: Set style to FILL
        color = Color.RED
        style = Paint.Style.FILL_AND_STROKE
        strokeWidth = 3f

    }

    private val blackStrokePaint = Paint().apply {
        // TODO: Set color to black
        // TODO: Set style to STROKE
        // TODO: Set stroke width to 3
        color = Color.BLACK
        style = Paint.Style.STROKE
        strokeWidth = 5f
    }

    init {
        // IMPORTANT: Views in ViewGroups don't draw by default
        // TODO: What method do you call to enable drawing?

        setWillNotDraw(false)

    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        // Clear background
        canvas.drawColor(Color.WHITE)

        // TODO: Draw filled blue rectangle from (50, 50) to (200, 200)
        // Hint: canvas.drawRect(left, top, right, bottom, paint)
        canvas.drawRect(
            50f ,
             50f,
            200f,
             200f,
            blueFillPaint
        )
        // TODO: Draw black stroked rectangle (same coordinates)
        canvas.drawRect(
            100f ,
            100f,
            250f,
            250f,
            redFillPaint
        )
        canvas.drawRect(
            50f ,
            50f,
            200f,
            200f,
            blackStrokePaint
        )



        Log.d("CustomDrawingView", "onDraw called")
    }
}