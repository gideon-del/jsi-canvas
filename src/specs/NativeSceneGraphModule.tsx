import { TurboModule, TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  installSceneGraph: () => boolean;
}

export default TurboModuleRegistry.getEnforcing<Spec>('SceneGraphModule');
