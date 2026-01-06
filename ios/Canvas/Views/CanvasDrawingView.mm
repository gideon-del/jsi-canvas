//
//  CanvasDrawingView.m
//  CanvasMVP
//
//  Created by macbook pro on 04/01/2026.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "camera-state/CameraState.h"
#import "CanvasDrawingView.h"
@interface CanvasDrawingView() {
  CanvasMVP::CameraState *_camera;
}

@end


@implementation CanvasDrawingView

-(instancetype)initWithFrame:(CGRect)frame camera:(void *)camera {
  if(self = [super initWithFrame:frame]) {
    _camera = static_cast<CanvasMVP::CameraState *>(camera);
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
  CanvasMVP::Size viewSizeCpp = [self toCppSize:viewSize];
  
  CanvasMVP::Point topLeftCpp = _camera->screenToWorld(startPoint, viewSizeCpp);
  CanvasMVP::Point bottomRightCpp = _camera->screenToWorld(endPoint, viewSizeCpp);
  
  
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
  
    CanvasMVP::Point screenTop = _camera->worldToScreen(worldTop, viewSizeCpp);
    CanvasMVP::Point screenBottom = _camera->worldToScreen(worldBottom, viewSizeCpp);
    
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
  
    CanvasMVP::Point screenLeft = _camera->worldToScreen(worldLeft, viewSizeCpp);
    CanvasMVP::Point screenRight = _camera->worldToScreen(worldRight, viewSizeCpp);
    
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
  CanvasMVP::Point screenOrigin = _camera->worldToScreen([self toCppPoint:CGPointZero], viewSizeCpp);
    
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
@end
