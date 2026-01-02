import { codegenNativeComponent, HostComponent, ViewProps } from 'react-native';

interface NativeProps extends ViewProps {}

export default codegenNativeComponent<NativeProps>(
  'CanvasView',
) as HostComponent<NativeProps>;
