package com.canvasmvp.canvas

import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.uimanager.SimpleViewManager
import com.facebook.react.uimanager.ThemedReactContext

@ReactModule(name = ReactCanvasViewManager.NAME)
class ReactCanvasViewManager(
    private val reactContext: ReactApplicationContext
) : SimpleViewManager<CanvasView>() {

    companion object {
        const val NAME = "CanvasView"
    }

    override fun getName(): String {
        return NAME
    }

    override fun createViewInstance(context: ThemedReactContext): CanvasView {
       return CanvasView(context)
    }


}