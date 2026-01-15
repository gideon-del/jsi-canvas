import { TurboModule, TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  installSceneGraph: () => void;
}

export default TurboModuleRegistry.getEnforcing<Spec>('SceneGraphModule');
