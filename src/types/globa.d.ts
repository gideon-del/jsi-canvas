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
export interface SceneGraphAPI {
  addNode(config: NodeConfig): string;
  getNode(id: string): Node | null;
  getAllNodes(): Node[];
  getNodeCount(): number;
  updateNode(id: string, update: NodeUpdate): boolean;
  removeNode(id: string): boolean;
  clear(): void;
}

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
