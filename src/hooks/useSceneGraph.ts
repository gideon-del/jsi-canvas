import { useEffect } from 'react';
import SceneGraphModule from '../specs/NativeSceneGraphModule';
import {
  AddNodeEvent,
  ClearedGraphEvent,
  RemoveNodeEvent,
  SelectionChangeEvent,
} from '@types/globa';

interface SceneGraphParams {
  onAddNode?: (event: AddNodeEvent) => void;
  onRemoveNode?: (event: RemoveNodeEvent) => void;
  onSelectionChange?: (event: SelectionChangeEvent) => void;
  onClearGraph?: (event: ClearedGraphEvent) => void;
}
export function useSceneGraph(params?: SceneGraphParams) {
  if (!global.sceneGraph) {
    throw new Error('SceneGraph not installed');
  }

  useEffect(() => {
    const sceneGraph = global.sceneGraph;
    if (!sceneGraph) return;
    const addNodeListenerId = sceneGraph.on('nodeAdded', event => {
      console.log(event);
      params?.onAddNode?.(event);
    });
    const removeNodeListenerId = sceneGraph.on('nodeRemoved', event => {
      console.log(event);
      params?.onRemoveNode?.(event);
    });
    const selectionListenerId = sceneGraph.on('selectionChanged', event => {
      console.log(event);
      params?.onRemoveNode?.(event);
    });
    const clearedGraphId = sceneGraph.on('cleared', event => {
      console.log(event);
      params?.onClearGraph?.(event);
    });

    return () => {
      sceneGraph.off('cleared', clearedGraphId);
      sceneGraph.off('nodeAdded', addNodeListenerId);
      sceneGraph.off('nodeRemoved', removeNodeListenerId);
      sceneGraph.off('selectionChanged', selectionListenerId);
    };
  }, [params]);
  return global.sceneGraph;
}
export function installSceneGraph() {
  return SceneGraphModule.installSceneGraph();
}
