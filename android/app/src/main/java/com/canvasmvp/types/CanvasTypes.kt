package com.canvasmvp.types

object CanvasTypes {
    data class Color(
        var r: Float = 0f,
        var g: Float = 0f,
        var b: Float = 0f,
        var a: Float = 0f
    )

    data class Rect(
        var x: Float,
        var y: Float,
        var width: Float,
        var height: Float,
    )

    data class Point(
        var x: Float,
        var y: Float,
    )

    data class Size(
        var width: Float,
        var height: Float,
    )
}