package com.canvasmvp.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Color
import android.view.View

class TransformView(context: Context) : View(context) {

    private val redPaint = Paint().apply {
        color = Color.RED
        style = Paint.Style.FILL
    }

    private val bluePaint = Paint().apply {
        color = Color.BLUE
        style = Paint.Style.FILL
    }

    private val greenPaint = Paint().apply {
        color = Color.GREEN
        style = Paint.Style.FILL
    }

    init {
        setWillNotDraw(false)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawColor(Color.WHITE)

        // Draw original square
        canvas.drawRect(50f, 50f, 150f, 150f, redPaint)

        // TODO: Save canvas state
         canvas.save()
        // TODO: Translate 200 pixels to the right

//         canvas.translate(200f, 0f)
//        canvas.rotate(45f)
//         canvas.scale(1.5f,1.5f)

        val rectLeft = 50f
        val rectTop = 50f
        val rectRight = 150f
        val rectBottom = 150f

        val centerX = (rectLeft + rectRight) / 2
        val centerY = (rectTop + rectBottom) / 2

        canvas.save()
// TODO: Move origin to center
        canvas.translate(centerX, centerY)
// TODO: Rotate 45 degrees
        canvas.rotate(45f)
// TODO: Move origin back
        canvas.translate(-centerX, -centerY)
        canvas.drawRect(rectLeft, rectTop, rectRight, rectBottom, greenPaint)
        canvas.restore()
        // Draw the same square (where will it appear?)
        canvas.drawRect(50f, 50f, 150f, 150f, bluePaint)

        // TODO: Restore canvas state
        canvas.restore()
        // Draw another square (where will THIS appear?)
        canvas.drawRect(50f, 200f, 150f, 300f, greenPaint)
    }
}