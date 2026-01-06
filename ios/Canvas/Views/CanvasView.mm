#import "CanvasView.h"
#import <React/RCTConversions.h>
#import <UIKit/UIKit.h>
#import <react/renderer/components/RNCanvasMVPSpec/ComponentDescriptors.h>
#import <react/renderer/components/RNCanvasMVPSpec/Props.h>
#import <react/renderer/components/RNCanvasMVPSpec/RCTComponentViewHelpers.h>
#import "camera-state/CameraState.h"
#import "scene-graph/SceneGraph.h"
#import "CoreGraphics/CoreGraphics.h"
#import "CanvasDrawingView.h"
using namespace facebook::react;


@interface CanvasView() <RCTCanvasViewViewProtocol, UIGestureRecognizerDelegate>

@end
@implementation CanvasView
{
  CanvasDrawingView *_canvasLayer;
  CanvasMVP::CameraState _camera;
  CanvasMVP::SceneGraph _sceneGraph;
  UIPanGestureRecognizer *_panGesture;
  UIPinchGestureRecognizer *_pinchGesture;
  CGPoint _lastPanTranslation;
}

- (CanvasMVP::CameraState*)camera {
    return &_camera;
}

- (CanvasMVP::SceneGraph*)sceneGraph {
    return &_sceneGraph;
}
- (instancetype)initWithFrame:(CGRect)frame {
  if(self = [super initWithFrame:frame]){
    [self setupView];
    [self setupGestures];
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
-(void)setupGestures {
  _panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  _pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinch:)];
  
  _panGesture.minimumNumberOfTouches =1;
  _panGesture.maximumNumberOfTouches = 1;
  
  [self addGestureRecognizer:_panGesture];
  
  [self addGestureRecognizer:_pinchGesture];
  
  _panGesture.delegate = self;
  _pinchGesture.delegate= self;
  
  _lastPanTranslation = CGPointZero;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}


-(void)handlePinch:(UIPinchGestureRecognizer *)gesture {
  CGPoint pinchPoint = [gesture locationInView:self];
  switch (gesture.state) {
    case UIGestureRecognizerStateBegan:
      NSLog(@"[Canvas view] Pin begin");
      break;
    case  UIGestureRecognizerStateChanged:{
      float newZoom = _camera.zoom * gesture.scale;
      _camera.zoomAt(newZoom, CanvasMVP::Point(pinchPoint.x, pinchPoint.y), CanvasMVP::Size(self.frame.size.width, self.frame.size.height));
      gesture.scale = 1.0;
      [_canvasLayer setNeedsDisplay];
      
      
      break;
    }
      
    case UIGestureRecognizerStateEnded:
    case UIGestureRecognizerStateCancelled:
        NSLog(@"[CanvasView] Pinch ended - zoom:%.2f", _camera.zoom);
      break;
    default:
      break;
  }
}
-(void)handlePan:(UIPanGestureRecognizer *)gesture {
  CGPoint currentTranslation = [gesture translationInView:self];
  
  switch (gesture.state) {
    case UIGestureRecognizerStateBegan:
      _lastPanTranslation = CGPointZero;
      NSLog(@"[CanvasView] Pan began");
      break;
    case  UIGestureRecognizerStateChanged:{
      float dx = currentTranslation.x - _lastPanTranslation.x;
      float dy = currentTranslation.y - _lastPanTranslation.y;
      _camera.pan(dx, dy);
      _lastPanTranslation = currentTranslation;
      [_canvasLayer setNeedsDisplay];
      break;
    }
   
    case  UIGestureRecognizerStateEnded:
    case UIGestureRecognizerStateCancelled:
      NSLog(@"[CanvasView] Pan ended - camera(%.2f,%.2f)", _camera.offsetX, _camera.offsetY);
      _lastPanTranslation = CGPointZero;
    default:
      break;
  }
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




- (void)createTestScene:(int)nodeCount {
  _sceneGraph.clear();
      
      int cols = (int)std::ceil(std::sqrt(nodeCount));
      float spacing = 250;
      
      for (int i = 0; i < nodeCount; ++i) {
          int row = i / cols;
          int col = i % cols;
          
          auto node = std::make_unique<CanvasMVP::Node>(
              "node_" + std::to_string(i),
              CanvasMVP::NodeType::RECTANGLE,
              col * spacing, row * spacing,
              100, 100
          );
          
          node->fillColor = (i % 2 == 0) ?
              CanvasMVP::Color::red() :
              CanvasMVP::Color::blue();
          
          _sceneGraph.addNode(std::move(node));
      }
      
      [self setNeedsDisplay];
}

@end



