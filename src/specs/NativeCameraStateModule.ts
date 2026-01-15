import { TurboModuleRegistry, TurboModule } from 'react-native';

export interface Spec extends TurboModule {
  installCameraState(): boolean;
}

export default TurboModuleRegistry.getEnforcing<Spec>('CameraStateModule');
