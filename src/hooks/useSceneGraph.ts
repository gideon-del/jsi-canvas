import { useEffect, useState } from 'react';
import SceneGraphModule from '../specs/NativeSceneGraphModule';
import {
  AddNodeCommand,
  AddNodeEvent,
  AnyCommand,
  ClearedGraphEvent,
  NodeConfig,
  RemoveNodeCommand,
  RemoveNodeEvent,
  SelectionChangeEvent,
  UpdateNodeCommand,
} from '@types/globa';

interface SceneGraphParams {
  onAddNode?: (event: AddNodeEvent) => void;
  onRemoveNode?: (event: RemoveNodeEvent) => void;
  onSelectionChange?: (event: SelectionChangeEvent) => void;
  onClearGraph?: (event: ClearedGraphEvent) => void;
}
export function useSceneGraph(params?: SceneGraphParams) {
  const [canRedo, setCanRedo] = useState(false);
  const [canUndo, setCanUndo] = useState(false);

  if (!global.sceneGraph) {
    throw new Error('SceneGraph not installed');
  }
  const executeCommand = (command: AnyCommand) => {
    const isSuccessFull = global.sceneGraph?.executeCommand(command);

    if (isSuccessFull) {
      setCanRedo(!!global.sceneGraph?.canRedo());
      setCanUndo(!!global.sceneGraph?.canUndo());
    }
  };
  const addNode = (node: NodeConfig) => {
    try {
      const command: AddNodeCommand = {
        type: 'addNode',
        node,
      };
      executeCommand(command);
    } catch (error) {
      console.error(error);
    }
  };
  const removeNode = (nodeId: string) => {
    try {
      const command: RemoveNodeCommand = {
        type: 'removeNode',
        nodeId,
      };
      executeCommand(command);
    } catch (error) {
      console.error(error);
    }
  };
  const updateNode = ({
    node,
    nodeId,
  }: {
    nodeId: string;
    node: NodeConfig;
  }) => {
    try {
      const command: UpdateNodeCommand = {
        type: 'updateNode',
        nodeId,
        node,
      };
      executeCommand(command);
    } catch (error) {
      console.error(error);
    }
  };

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
  return {
    sceneGraph: global.sceneGraph,
    addNode,
    removeNode,
    updateNode,
    canRedo,
    canUndo,
  };
}
export function installSceneGraph() {
  return SceneGraphModule.installSceneGraph();
}
