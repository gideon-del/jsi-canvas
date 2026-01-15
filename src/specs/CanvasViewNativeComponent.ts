import {
  codegenNativeCommands,
  codegenNativeComponent,
  HostComponent,
  ViewProps,
  CodegenTypes,
} from 'react-native';

interface NativeProps extends ViewProps {}

interface NativeCommand {
  createTestScene(
    viewRef: React.ElementRef<HostComponent<NativeProps>>,
    count: CodegenTypes.Int32,
  ): void;
  redrawNative(viewRef: React.ElementRef<HostComponent<NativeProps>>): void;
}

export const Commands = codegenNativeCommands<NativeCommand>({
  supportedCommands: ['createTestScene', 'redrawNative'],
});
export default codegenNativeComponent<NativeProps>(
  'CanvasView',
) as HostComponent<NativeProps>;
