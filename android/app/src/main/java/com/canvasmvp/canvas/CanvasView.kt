package com.canvasmvp.canvas

import android.content.Context
import android.graphics.Canvas
import android.view.View
import android.graphics.Paint
import android.graphics.Color
import android.util.Log
class CanvasView(context: Context): View(context) {

    private val debugPaint = Paint().apply {
        color = Color.RED
        style= Paint.Style.FILL
    }

    init {
        Log.d(TAG,"Android view initialized")
        setWillNotDraw(false)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)

        canvas.drawRect(0f,0f, width.toFloat(), height.toFloat(), debugPaint)
    }

    override fun onLayout(changed: Boolean, left: Int, top: Int, right: Int, bottom: Int) {
        super.onLayout(changed, left, top, right, bottom)
        if(changed) invalidate()
    }

    companion object {
        const val TAG = "CanvasView"
    }
}