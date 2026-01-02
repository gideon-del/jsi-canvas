import {
  codegenNativeComponent,
  ColorValue,
  HostComponent,
  ViewProps,
} from 'react-native';

interface NativeProps extends ViewProps {
  color?: ColorValue;
}

export default codegenNativeComponent<NativeProps>(
  'ColorBox',
) as HostComponent<NativeProps>;
