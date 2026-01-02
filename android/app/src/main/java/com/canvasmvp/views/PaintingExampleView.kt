package com.canvasmvp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Color
import android.graphics.DashPathEffect
import android.view.View

class PaintingExampleView(context: Context): View(context) {
    // Solid fill
    private val solidPaint = Paint().apply {
        color = Color.RED
        style = Paint.Style.FILL
    }

    // Stroked outline
    private val strokePaint = Paint().apply {
        color = Color.BLUE
        style = Paint.Style.STROKE
        strokeWidth = 5f
    }

    // Both fill and stroke
    private val fillAndStrokePaint = Paint().apply {
        color = Color.GREEN
        style = Paint.Style.FILL_AND_STROKE
        strokeWidth = 3f
    }

    private val roundedAndDashedPaint = Paint().apply {
        color = Color.BLUE
        alpha = 128
        strokeCap = Paint.Cap.ROUND
        strokeJoin = Paint.Join.ROUND
        pathEffect = DashPathEffect(
            floatArrayOf(10f, 5f),
            0f
        )
        strokeWidth = 5f
        style = Paint.Style.STROKE
        isAntiAlias = true
    }
    init {
        setWillNotDraw(false)
    }
    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        canvas.drawColor(Color.WHITE)

        // Draw same rectangle with different paints
        canvas.drawRect(50f, 50f, 150f, 150f, solidPaint)
        canvas.drawRect(200f, 50f, 300f, 150f, strokePaint)
        canvas.drawRect(350f, 50f, 450f, 150f, fillAndStrokePaint)
        canvas.drawRect(470f, 50f, 570f, 150f, roundedAndDashedPaint)
    }
}