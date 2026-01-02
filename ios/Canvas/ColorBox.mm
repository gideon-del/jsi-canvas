
#import "ColorBox.h"
#import <UIKit/UIKit.h>
#import <react/renderer/components/RNCanvasMVPSpec/ComponentDescriptors.h>
#import <react/renderer/components/RNCanvasMVPSpec/Props.h>
#import <react/renderer/components/RNCanvasMVPSpec/RCTComponentViewHelpers.h>
#import <React/RCTConversions.h>



using namespace facebook::react;

@interface ColorBox() <RCTColorBoxViewProtocol>
@end

@implementation ColorBox

+ (ComponentDescriptorProvider)componentDescriptorProvider
{
  NSLog(@"Component descriptor is called");
  return concreteComponentDescriptorProvider<ColorBoxComponentDescriptor>();
}


- (instancetype)initWithFrame:(CGRect)frame {
  NSLog(@"🟡 ColorBox initWithFrame called");
  if (self = [super initWithFrame:frame]){
    self.contentView = [UIView new];
    self.contentView.backgroundColor = [UIColor greenColor];
  }
  return self;
}

- (void)updateProps:(Props::Shared const &)props oldProps:(Props::Shared const &)oldProps
{
  NSLog(@"🔵 ColorBox: updateProps called");
  
  const auto &newViewProps = std::static_pointer_cast<ColorBoxProps const>(props);
  UIColor *newColor = RCTUIColorFromSharedColor(newViewProps->color);
  
  if (newColor != nil && ![newColor isEqual:self.contentView.backgroundColor]) {
    NSLog(@"🎨 New color: %@", newColor);
    self.contentView.backgroundColor = newColor;
  }

  [super updateProps:props oldProps:oldProps];
}

Class<RCTComponentViewProtocol> ColorBoxCls(void)
{
  return ColorBox.class;
}
@end
