#import "CanvasView.h"
#import <React/RCTConversions.h>
#import <UIKit/UIKit.h>
#import <react/renderer/components/RNCanvasMVPSpec/ComponentDescriptors.h>
#import <react/renderer/components/RNCanvasMVPSpec/Props.h>
#import <react/renderer/components/RNCanvasMVPSpec/RCTComponentViewHelpers.h>
using namespace facebook::react;


@interface CanvasView() <RCTCanvasViewViewProtocol>

@end
@implementation CanvasView
{
  UIView *_contentView;
}

- (instancetype)initWithFrame:(CGRect)frame {
  if(self = [super initWithFrame:frame]){
    [self setupView];
  }
  
  return self;
}
- (void)setupView {
  _contentView = [[UIView alloc] initWithFrame:self.bounds];
  
  _contentView.backgroundColor = UIColor.redColor;
  
  _contentView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  
  
  [self addSubview:_contentView];
  
  NSLog(@"✅ [CanvasView] iOS view initialized");
}

-(void)layoutSubviews {
  [super layoutSubviews];
  
  _contentView.frame = self.bounds;
}
+ (facebook::react::ComponentDescriptorProvider)componentDescriptorProvider {
  NSLog(@"Component descriptor is called");
  return concreteComponentDescriptorProvider<CanvasViewComponentDescriptor>();
}

Class<RCTCanvasViewViewProtocol> CanvasViewCls(void) {
  return CanvasView.class;
}

@end



