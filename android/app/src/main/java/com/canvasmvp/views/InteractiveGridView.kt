package com.canvasmvp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Color

import android.view.MotionEvent
import android.view.View
import android.util.Log
import kotlin.math.hypot

class InteractiveGridView(context: Context) : View(context) {

    private var touchX = 0f
    private var touchY = 0f
    private var isTouching = false
    private var lastTouchTime: Long = 0L
    private var lastTouchX: Float = 0f
    private var lastTouchY: Float = 0f
    private val touchPaint = Paint().apply {
        color = Color.RED
        style = Paint.Style.FILL
    }

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

    override fun onTouchEvent(event: MotionEvent): Boolean {
        Log.d("Event Pointer","Event pointer ${event.pointerCount}")
        val currentTime = System.currentTimeMillis()
        when (event.action) {

            MotionEvent.ACTION_DOWN -> {
                touchX = event.x
                touchY = event.y
                isTouching = true


                lastTouchTime = currentTime
                lastTouchX = touchX
                lastTouchY = touchY
                Log.d("GridView", "Touch down at ($touchX, $touchY)")

                // TODO: Trigger redraw
              invalidate()

                return true  // We handled this event
            }

            MotionEvent.ACTION_MOVE -> {
                // TODO: Update touch position
                // TODO: Trigger redraw

                val deltaTime = currentTime - lastTouchTime // ms
                val dx = event.x - lastTouchX
                val dy = event.y - lastTouchY
                val distance = hypot(dx.toDouble(), dy.toDouble())

                // Velocity in pixels per second
                val velocity = if (deltaTime > 0) distance / deltaTime * 1000 else 0.0

                Log.d("GridView", "Move to (${event.x}, ${event.y}), Δt=${deltaTime}ms, velocity=${velocity} px/s")

                touchX = event.x
                touchY = event.y
                invalidate()
                return true
            }

            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                // TODO: Set isTouching = false
                // TODO: Trigger redraw
                isTouching = false
                invalidate()
                return true
            }
        }
        return super.onTouchEvent(event)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        // Draw grid (from previous example)
        val spacing = 50f
        val gridSize = 10

        // TODO: Draw vertical lines
        for (i in 0..gridSize) {
            val x = i * spacing
            val isMajor = i % 5 == 0
            val paint = if (isMajor) darkGridPaint else lightGridPaint
            canvas.drawLine(x, 0f, x, height.toFloat(), paint)
        }



        // TODO: Draw horizontal lines

        for (i in 0..gridSize) {
            val y = i * spacing
            val isMajor = i % 5 == 0
            val paint = if (isMajor) darkGridPaint else lightGridPaint
            canvas.drawLine(0f, y, width.toFloat(), y, paint)
        }
        // TODO: Draw origin marker (cross at 0, 0)
        val markerSize = 60f
        canvas.drawLine(-markerSize, 0f,markerSize,0f, originPaint)
        canvas.drawLine(0f, -markerSize,0f,markerSize, originPaint)


        // Draw circle at touch point
        if (isTouching) {
            canvas.drawCircle(touchX, touchY, 60f, touchPaint)
        }
    }
}