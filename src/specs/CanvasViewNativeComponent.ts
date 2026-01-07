import {
  codegenNativeCommands,
  codegenNativeComponent,
  HostComponent,
  ViewProps,
} from 'react-native';
import { Int32 } from 'react-native/Libraries/Types/CodegenTypesNamespace';

interface NativeProps extends ViewProps {}

interface NativeCommand {
  createTestScene(
    viewRef: React.ElementRef<HostComponent<NativeProps>>,
    count: Int32,
  ): void;
}

export const Commands = codegenNativeCommands<NativeCommand>({
  supportedCommands: ['createTestScene'],
});
export default codegenNativeComponent<NativeProps>(
  'CanvasView',
) as HostComponent<NativeProps>;
