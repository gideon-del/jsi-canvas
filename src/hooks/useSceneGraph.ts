import SceneGraphModule from '../specs/NativeSceneGraphModule';
export function useSceneGraph() {
  if (!global.sceneGraph) {
    throw new Error('SceneGraph not installed');
  }
  return global.sceneGraph;
}
export function installSceneGraph() {
  return SceneGraphModule.installSceneGraph();
}
