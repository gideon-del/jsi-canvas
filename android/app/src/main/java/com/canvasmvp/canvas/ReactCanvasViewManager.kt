package com.canvasmvp.canvas

import android.util.Log
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReadableArray
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.uimanager.SimpleViewManager
import com.facebook.react.uimanager.ThemedReactContext
import com.facebook.react.uimanager.ViewManagerDelegate
import com.facebook.react.viewmanagers.CanvasViewManagerDelegate
import com.facebook.react.viewmanagers.CanvasViewManagerInterface
@ReactModule(name = ReactCanvasViewManager.NAME)
class ReactCanvasViewManager(
    private val reactContext: ReactApplicationContext
) : SimpleViewManager<CanvasView>(), CanvasViewManagerInterface<CanvasView> {

    companion object {
        const val NAME = "CanvasView"

    }

    private val delegate: CanvasViewManagerDelegate<CanvasView, ReactCanvasViewManager> =
        CanvasViewManagerDelegate(this)

    override fun getName(): String {
        return NAME
    }

    override fun getDelegate(): ViewManagerDelegate<CanvasView> = delegate


    override fun createViewInstance(context: ThemedReactContext): CanvasView {
        val view = CanvasView(context)
        view.setUpListeners()
       return view
    }

    override fun onDropViewInstance(view: CanvasView) {
        super.onDropViewInstance(view)
        view.removeListeners()
    }


    override fun createTestScene(view: CanvasView?, count: Int) {
        Log.d("CanvasView","Create test sceene called")
      view?.createTestScene(count)
    }

    override fun redrawNative(view: CanvasView?) {
        view?.invalidate()
    }


}