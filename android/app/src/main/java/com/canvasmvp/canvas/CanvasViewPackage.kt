package com.canvasmvp.canvas


import com.facebook.react.BaseReactPackage
import com.facebook.react.bridge.NativeModule
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.module.model.ReactModuleInfo
import com.facebook.react.module.model.ReactModuleInfoProvider
import com.facebook.react.uimanager.ViewManager


class CanvasViewPackage: BaseReactPackage() {
    override fun createViewManagers(reactContext: ReactApplicationContext): List<ViewManager<*, *>> {
        return listOf(ReactCanvasViewManager(reactContext))
    }
    override fun getModule(s: String, reactApplicationContext: ReactApplicationContext): NativeModule? {
        when (s) {
            ReactCanvasViewManager.NAME -> ReactCanvasViewManager(reactApplicationContext)
        }
        return null
    }

    override fun getReactModuleInfoProvider(): ReactModuleInfoProvider = ReactModuleInfoProvider {
        mapOf(ReactCanvasViewManager.NAME to ReactModuleInfo(
            name = ReactCanvasViewManager.NAME,
            className = ReactCanvasViewManager.NAME,
            canOverrideExistingModule = false,
            needsEagerInit = false,
            isCxxModule = false,
            isTurboModule = true,
        )
        )
    }

}