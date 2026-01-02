package com.canvasmvp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Color

import android.view.View

class GridView(context: Context) : View(context) {

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

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawColor(Color.WHITE)

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


        // canvas.drawLine(...)
    }
}