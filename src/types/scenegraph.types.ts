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

declare global {
  var sceneGraph: SceneGraphAPI | undefined;
}
export {};
