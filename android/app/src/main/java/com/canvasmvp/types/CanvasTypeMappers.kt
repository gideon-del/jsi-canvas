package com.canvasmvp.types

import android.graphics.Color
import android.graphics.RectF

fun CanvasTypes.Color.toArgb(): Int {
    return Color.argb((a*255).toInt(),(r * 255).toInt(),(g * 255).toInt(),(b*255).toInt())
}

fun  CanvasTypes.Rect.toRectF(): RectF {
    return RectF(
         x,y, x+width, y+height

    )
}