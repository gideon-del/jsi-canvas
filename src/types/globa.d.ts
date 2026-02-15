// Entities
export interface NodeConfig {
  id?: string;
  x: number;
  y: number;
  width?: number;
  height?: number;
  fillColor?: string;
  strokeColor?: string;
  strokeWidth?: number;
  zIndex?: number;
}

export interface Node {
  id: string;
  x: number;
  y: number;
  width: number;
  height: number;
  fillColor: string;
  strokeColor: string;
  strokeWidth: number;
  zIndex: number;
}
export type NodeUpdate = Partial<Omit<Node, 'id'>>;
//Events
export interface SceneGraphEvent {
  type: string;
}

export interface Command {
  type: string;
}
export interface AddNodeCommand extends Command {
  type: 'addNode';
  node: NodeConfig;
}
export interface UpdateNodeCommand extends Command {
  type: 'updateNode';
  node: NodeConfig;
  nodeId: string;
}
export interface RemoveNodeCommand extends Command {
  type: 'removeNode';
  nodeId: string;
}

export type AnyCommand = RemoveNodeCommand | AddNodeCommand | UpdateNodeCommand;
export interface AddNodeEvent extends SceneGraphEvent {
  node: Node;
}
export interface RemoveNodeEvent extends SceneGraphEvent {
  node: Node;
}
export interface SelectionChangeEvent extends SceneGraphEvent {
  node: Node;
}
export interface ClearedGraphEvent extends SceneGraphEvent {}
export interface SceneGraphEventMap {
  nodeAdded: AddNodeEvent;
  nodeRemoved: RemoveNodeEvent;
  selectionChanged: SelectionChangeEvent;
  cleared: ClearedGraphEvent;
}

export interface SceneGraphAPI {
  getNode(id: string): Node | null;
  getAllNodes(): Node[];
  getNodeCount(): number;
  on: <K extends keyof SceneGraphEventMap>(
    eventType: K,
    fn: (event: SceneGraphEventMap[K]) => void,
  ) => number;
  off: <K extends keyof SceneGraphEventMap>(eventType: K, id: number) => void;
  executeCommand: (command: AnyCommand) => boolean;
  redo: () => boolean;
  undo: () => boolean;
  canUndo: () => boolean;
  canRedo: () => boolean;
  clearHistory: () => boolean;
}

// Camera

export interface CameraAPI {
  screenToWorld(x: number, y: number): { x: number; y: number };
  worldToScreen(x: number, y: number): { x: number; y: number };

  pan(dx: number, dy: number): void;
  zoomAt(scale: number, x: number, y: number): void;

  getZoom(): number;
  getOffset(): { x: number; y: number };
}
declare global {
  var sceneGraph: SceneGraphAPI | undefined;
  var canvasCamera: CameraAPI | undefined;
}
export {};
