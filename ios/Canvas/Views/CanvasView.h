//
//  CanvasView.h
//  CanvasMVP
//
//  Created by macbook pro on 02/01/2026.
//
#import <React/RCTViewComponentView.h>
#import <UIKit/UIKit.h>

#ifndef CanvasView_h
#define CanvasView_h
namespace CanvasMVP {
    class SceneGraph;
    class CameraState;
}
@interface CanvasView : RCTViewComponentView
@property (nonatomic, readonly) CanvasMVP::CameraState* camera;
@property (nonatomic, readonly) CanvasMVP::SceneGraph* sceneGraph;

- (void)createTestScene:(int)nodeCount;

@end
#endif /* CanvasView_h */
