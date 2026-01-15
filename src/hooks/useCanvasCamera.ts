import { CameraAPI } from '../types/globa';
import { useEffect, useState } from 'react';
import CameraStateModule from '../specs/NativeCameraStateModule';
export function useCanvasCamera() {
  const [canvasCamera, setCanvasCamera] = useState<CameraAPI | undefined>(
    undefined,
  );

  useEffect(() => {
    const installed = CameraStateModule.installCameraState();

    if (installed && global.canvasCamera) {
      setCanvasCamera(global.canvasCamera);
    }
  }, []);

  return canvasCamera;
}
