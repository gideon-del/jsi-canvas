#import <Foundation/Foundation.h>
#import <react/renderer/components/RNCanvasMVPSpec/ComponentDescriptors.h>
#import <react/renderer/componentregistry/ComponentDescriptorProviderRegistry.h>

using namespace facebook::react;

@interface ColorBoxRegistry : NSObject
+ (void)registerComponents:(std::shared_ptr<const ComponentDescriptorProviderRegistry>)registry;
@end

@implementation ColorBoxRegistry

+ (void)registerComponents:(std::shared_ptr<const ComponentDescriptorProviderRegistry>)registry
{
  RNCanvasMVPSpec_registerComponentDescriptorsFromCodegen(registry);
}
@end
