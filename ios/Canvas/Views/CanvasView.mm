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
#import "jsi/SceneGraphJSI.h"
#import <React/RCTBridge+Private.h>
#import <ReactCommon/RCTTurboModule.h>
#import <jsi/jsi.h>

using namespace facebook::react;


static std::shared_ptr<CanvasMVP::SceneGraph> g_sceneGraph = nullptr;

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
+ (void)load {
  static dispatch_once_t onceToken;
  
  dispatch_once(&onceToken, ^{
    g_sceneGraph =std::make_shared<CanvasMVP::SceneGraph>();
    NSLog(@"[CanvasView] Scene graph created");
  });
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
    [self installJSI];
    [self setupGestures];
  }
  
  return self;
}

- (void)installJSI {
  RCTBridge *bridge = [RCTBridge currentBridge];
      if (!bridge) {
          NSLog(@"[CanvasView] WARNING: No bridge yet, will retry...");
          // Retry after a delay
          dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1 * NSEC_PER_SEC)),
                        dispatch_get_main_queue(), ^{
              [self installJSI];
          });
          return;
      }
      
      RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
      if (!cxxBridge.runtime) {
          NSLog(@"[CanvasView] ERROR: No JSI runtime available!");
          return;
      }
      
      jsi::Runtime *runtime = (jsi::Runtime *)cxxBridge.runtime;
      
      // Only install once
      if (runtime->global().hasProperty(*runtime, "sceneGraph")) {
          NSLog(@"[CanvasView] JSI already installed");
          return;
      }
      
      NSLog(@"[CanvasView] Installing JSI...");
      
      // Create JSI host object
      auto jsiWrapper = std::make_shared<CanvasMVP::SceneGraphJSI>(g_sceneGraph);
      
      // Install as global.sceneGraph
      runtime->global().setProperty(
          *runtime,
          "sceneGraph",
          jsi::Object::createFromHostObject(*runtime, jsiWrapper)
      );
      
      NSLog(@"[CanvasView] ✅ JSI installed! Available as global.sceneGraph");
}
- (void)setupView {
  _canvasLayer = [[CanvasDrawingView alloc] initWithFrame:self.bounds camera:&_camera sceneGraph:&_sceneGraph];
  
  _canvasLayer.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  
  
  [self addSubview:_canvasLayer];
  
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
      _camera.zoomAt(newZoom, CanvasMVP::Point(pinchPoint.x, pinchPoint.y));
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
  return concreteComponentDescriptorProvider<CanvasViewComponentDescriptor>();
}

Class<RCTCanvasViewViewProtocol> CanvasViewCls(void) {
  return CanvasView.class;
}
-(void)handleCommand:(const NSString *)commandName args:(const NSArray *)args{
  NSLog(@"[CanvasView] Command called: %@, args: %@",
         commandName,
         args);
  RCTCanvasViewHandleCommand(self, commandName, args);
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
        
        node->zIndex = i;
          
          _sceneGraph.addNode(std::move(node));
      }
      NSLog(@"Created %d nodes", nodeCount);
      [_canvasLayer setNeedsDisplay];
}

@end



