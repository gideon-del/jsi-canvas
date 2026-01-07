


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "camera-state/CameraState.h"
#import "scene-graph/SceneGraph.h"
#import "CanvasDrawingView.h"
@interface CanvasDrawingView() {
  CanvasMVP::CameraState *_camera;
  CanvasMVP::SceneGraph *_sceneGraph;
}

@end


@implementation CanvasDrawingView

-(instancetype)initWithFrame:(CGRect)frame camera:(void *)camera sceneGraph:(void *)sceneGraph {
  if(self = [super initWithFrame:frame]) {
    _camera = static_cast<CanvasMVP::CameraState *>(camera);
    _sceneGraph = static_cast<CanvasMVP::SceneGraph *>(sceneGraph);
    self.backgroundColor = [UIColor whiteColor];
    self.opaque = YES;
  };
  
  return self;
}

- (void)drawRect:(CGRect)rect {
  CGContextRef ctx = UIGraphicsGetCurrentContext();
  
  if(!ctx || !_camera){
    return;
  }
  
  [[UIColor whiteColor] setFill];
  CGContextFillRect(ctx, rect);
  
  
  // Draw grid lines
  [self drawGridInContext:ctx];
  
  [self drawOriginInContext:ctx];
  //This is to convert the screen to world co-ordinates
  //So that we can query with the world co-ordinates
  CanvasMVP::RectF worldViewPort = [self calculateViewPort];
  
  auto visibleNodes = _sceneGraph->queryVisible(CanvasMVP::Rect(worldViewPort.x, worldViewPort.y, worldViewPort.width, worldViewPort.height));
  
  // Sort by z Index
  std::sort(visibleNodes.begin(),visibleNodes.end(),
            [](const CanvasMVP::Node* a, const CanvasMVP::Node* b){
    return  a->zIndex < b->zIndex;
  }
            );
  
  for(const auto* node: visibleNodes) {
      [self drawNode:node context:ctx];
  }
  
}

-(void)drawNode:(const CanvasMVP::Node *)node context:(CGContextRef)ctx {
  if(!node || !node->visible) return;
  
  CanvasMVP::RectF screenBounds = _camera->worldToScreen(node->bounds.toRectF());
  UIColor* nodeFillColor = [self fromCppColor:node->fillColor];
  UIColor* nodeStrokeColor = [self fromCppColor:node->strokeColor];
  


  
  CGContextSetFillColorWithColor(ctx, nodeFillColor.CGColor);
  CGContextFillRect(ctx,[self fromCppRect:screenBounds]);
  if(node->strokeWidth > 0) {
    CGFloat nodeLineWidth = node->strokeWidth;
    CGContextSetStrokeColorWithColor(ctx, nodeStrokeColor.CGColor);
    CGContextSetLineWidth(ctx, nodeLineWidth);
    CGContextStrokeRect(ctx, [self fromCppRect:screenBounds]);
  }
  if(node->selected) {
    CGContextSetStrokeColorWithColor(ctx, [[UIColor blueColor] CGColor]);
    CGContextSetLineWidth(ctx, 3.0f);
    CGRect selectionBound = CGRectInset([self fromCppRect:screenBounds], -4, -4);
    CGContextStrokeRect(ctx, selectionBound);
  }
  CGContextStrokePath(ctx);
}

- (void)drawGridInContext:(CGContextRef)ctx {
  const float gridSpacing = 100.0f;
  const float minorGridSpacing = 20.0f;
  const UIColor *majorLineColor = [UIColor darkGrayColor];
  const UIColor *minorLineColor = [UIColor lightGrayColor];
  CGSize viewSize = self.bounds.size;
  
  // Converting the screen view to world view
  // Basically the world view is the infinite canvas which exceeds the screen view
  CanvasMVP::Point startPoint = [self toCppPoint:CGPointZero];
  CanvasMVP::Point endPoint = [self toCppPoint:CGPointMake(viewSize.width, viewSize.height)];

  
  CanvasMVP::Point topLeftCpp = _camera->screenToWorld(startPoint);
  CanvasMVP::Point bottomRightCpp = _camera->screenToWorld(endPoint);
  
  
  
  float startX =
      floor(topLeftCpp.x / minorGridSpacing) * minorGridSpacing;

  float endX = bottomRightCpp.x;

  float startY =
      floor(topLeftCpp.y / minorGridSpacing) * minorGridSpacing;

  float endY = bottomRightCpp.y;
  
  
  
  float x = startX;
  
  // Vertical grid lines
  while (x < endX) {
    bool isMajorLine = fmodf(x, gridSpacing) == 0.0f;
    
    CanvasMVP::Point worldTop(x, topLeftCpp.y);
    CanvasMVP::Point worldBottom(x, bottomRightCpp.y);
  
    CanvasMVP::Point screenTop = _camera->worldToScreen(worldTop);
    CanvasMVP::Point screenBottom = _camera->worldToScreen(worldBottom);
    
    float lineWidth =
        (isMajorLine ? 1.5f : 0.75f) / _camera->zoom;
    CGColorRef color = isMajorLine ? majorLineColor.CGColor : minorLineColor.CGColor;
    
   
    CGContextSetStrokeColorWithColor(ctx, color);
    CGContextSetLineWidth(ctx,lineWidth);
    CGContextMoveToPoint(ctx, screenTop.x, screenTop.y);
    CGContextAddLineToPoint(ctx, screenBottom.x, screenBottom.y);
    CGContextStrokePath(ctx);
    
    x += minorGridSpacing;
  }
  
  
  float y = startY;
  
  while (y < endY) {
    bool isMajorLine = fmodf(y, gridSpacing) == 0.0f;
    
    CanvasMVP::Point worldLeft(topLeftCpp.x, y);
    CanvasMVP::Point worldRight(bottomRightCpp.x, y);
  
    CanvasMVP::Point screenLeft = _camera->worldToScreen(worldLeft);
    CanvasMVP::Point screenRight = _camera->worldToScreen(worldRight);
    
    float lineWidth =
        (isMajorLine ? 1.5f : 0.75f) / _camera->zoom;
    CGColorRef color = isMajorLine ? majorLineColor.CGColor : minorLineColor.CGColor;
    
  
    CGContextSetStrokeColorWithColor(ctx, color);
    CGContextSetLineWidth(ctx,lineWidth);
    CGContextMoveToPoint(ctx, screenLeft.x, screenLeft.y);
    CGContextAddLineToPoint(ctx, screenRight.x, screenRight.y);
    CGContextStrokePath(ctx);
    
    y += minorGridSpacing;
  }
  
  
 
}


- (void)drawOriginInContext:(CGContextRef)ctx {
  CanvasMVP::Size viewSizeCpp = [self toCppSize:self.bounds.size];
    
    // Convert world origin (0,0) to screen space
  CanvasMVP::Point screenOrigin = _camera->worldToScreen([self toCppPoint:CGPointZero]);
    
    // Draw red cross
    CGContextSetStrokeColorWithColor(ctx, [[UIColor redColor] CGColor]);
    CGContextSetLineWidth(ctx, 3.0f);
    
    const float crossSize = 20.0f;
    
  
    
    // Horizontal line
    CGContextMoveToPoint(ctx, screenOrigin.x - crossSize, screenOrigin.y);
    CGContextAddLineToPoint(ctx, screenOrigin.x + crossSize, screenOrigin.y);
    
    // Vertical line
    CGContextMoveToPoint(ctx, screenOrigin.x, screenOrigin.y - crossSize);
    CGContextAddLineToPoint(ctx, screenOrigin.x, screenOrigin.y + crossSize);
    
    CGContextStrokePath(ctx);
}

// Camera Utility functions

- (CanvasMVP::Point)toCppPoint:(CGPoint)point {
  return CanvasMVP::Point((float)point.x, (float)point.y);
}

- (CanvasMVP::Size)toCppSize:(CGSize)size {
  return CanvasMVP::Size((float)size.width, (float)size.height);
}

- (CanvasMVP::RectF)toCppRect:(CGRect)rect {
    return CanvasMVP::RectF{
        (float)rect.origin.x,
        (float)rect.origin.y,
        (float)rect.size.width,
        (float)rect.size.height
    };
}

- (CanvasMVP::Color)toCppColor:(UIColor *)color {
    CGFloat r, g, b, a;
    [color getRed:&r green:&g blue:&b alpha:&a];
    return CanvasMVP::Color{(float)r, (float)g, (float)b, (float)a};
}

// C++ → iOS conversions
- (CGPoint)fromCppPoint:(CanvasMVP::Point)point {
    return CGPointMake(point.x, point.y);
}

- (CGSize)fromCppSize:(CanvasMVP::Size)size {
    return CGSizeMake(size.width, size.height);
}

- (CGRect)fromCppRect:(CanvasMVP::RectF)rect {
    return CGRectMake(rect.x, rect.y, rect.width, rect.height);
}

- (UIColor *)fromCppColor:(CanvasMVP::Color)color {
  return [UIColor colorWithRed:color.r green:color.g blue:color.b alpha:color.a];
  
}

- (CanvasMVP::RectF)calculateViewPort {
  CanvasMVP::RectF viewSize =[self toCppRect:self.bounds];
  CanvasMVP::RectF worldSize = _camera->screenToWorld(viewSize);
  return worldSize;
}
@end
