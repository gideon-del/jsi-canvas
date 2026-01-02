package com.canvasmvp

import android.app.Activity
import android.os.Bundle
import com.canvasmvp.views.GridView
import com.canvasmvp.views.TransformedGridView

class TestGraphicsActivity: Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
       val customDrawingView = TransformedGridView(this)

        setContentView(customDrawingView)
    }
}