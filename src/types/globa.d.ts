export interface SceneGraphAPI {
  addNode(config: {
    x: number;
    y: number;
    width?: number;
    height?: number;
    fillColor?: string;
    zIndex?: number;
  }): string;
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
