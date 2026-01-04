#import "CanvasView.h"
#import <React/RCTConversions.h>
#import <UIKit/UIKit.h>
#import <react/renderer/components/RNCanvasMVPSpec/ComponentDescriptors.h>
#import <react/renderer/components/RNCanvasMVPSpec/Props.h>
#import <react/renderer/components/RNCanvasMVPSpec/RCTComponentViewHelpers.h>
#import "camera-state/CameraState.h"
#import "CoreGraphics/CoreGraphics.h"
#import "CanvasDrawingView.h"
using namespace facebook::react;


@interface CanvasView() <RCTCanvasViewViewProtocol>

@end
@implementation CanvasView
{
  CanvasDrawingView *_canvasLayer;
  CanvasMVP::CameraState _camera;
}

- (instancetype)initWithFrame:(CGRect)frame {
  if(self = [super initWithFrame:frame]){
    [self setupView];
  }
  
  return self;
}
- (void)setupView {
  _canvasLayer = [[CanvasDrawingView alloc] initWithFrame:self.bounds camera:&_camera];
  
  _canvasLayer.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  
  
  [self addSubview:_canvasLayer];
  
  NSLog(@"✅ [CanvasView] Camera initialized - offset:(%.2f, %.2f) zoom:%.2f",
           _camera.offsetX, _camera.offsetY, _camera.zoom);
}

-(void)layoutSubviews {
  [super layoutSubviews];
  
  _canvasLayer.frame = self.bounds;
}
+ (facebook::react::ComponentDescriptorProvider)componentDescriptorProvider {
  NSLog(@"Component descriptor is called");
  return concreteComponentDescriptorProvider<CanvasViewComponentDescriptor>();
}

Class<RCTCanvasViewViewProtocol> CanvasViewCls(void) {
  return CanvasView.class;
}




@end



