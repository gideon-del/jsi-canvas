#ifndef CanvasMVP_CameraState_h
#define CanvasMVP_CameraState_h

#include "../types/PlatformTypes.h"

using namespace CanvasMVP;

namespace CanvasMVP
{
    struct CameraState
    {
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        float zoom = 1.0f;

        static constexpr float MIN_ZOOM = 0.1f;
        static constexpr float MAX_ZOOM = 10.0f;

        Point worldToScreen(const Point &worldPoint, const Size &viewSize) const
        {
            return Point{
                (worldPoint.x + offsetX) * zoom,
                (worldPoint.y + offsetY) * zoom};
        }

        Point screenToWorld(const Point &screenPoint, const Size &viewSize) const
        {

            return Point{
                (screenPoint.x / zoom) - offsetX,
                (screenPoint.y / zoom) - offsetY};
        }

        Rect worldToScreen(const Rect &worldRect, const Size &viewSize) const
        {
            Point topLeft = worldToScreen(
                Point{worldRect.x, worldRect.y},
                viewSize);

            return Rect{
                topLeft.x,
                topLeft.y,
                worldRect.width * zoom,
                worldRect.height * zoom};
        }

        Rect screenToWorld(const Rect &screenRect, const Size &viewSize) const
        {
            Point topLeft = screenToWorld(
                Point{screenRect.x, screenRect.y},
                viewSize);

            return Rect{
                topLeft.x,
                topLeft.y,
                screenRect.width / zoom,
                screenRect.height / zoom};
        }

        void clampZoom()
        {
            if (zoom < MIN_ZOOM)
                zoom = MIN_ZOOM;
            if (zoom > MAX_ZOOM)
                zoom = MAX_ZOOM;
        }

        void pan(float dx, float dy)
        {
            offsetX += dx / zoom;
            offsetY += dy / zoom;
        }

        void zoomAt(float newZoom, const Point &centerPoint, const Size &viewSize)
        {
            // Get world position of zoom center
            Point worldCenter = screenToWorld(centerPoint, viewSize);

            // Apply new zoom
            zoom = newZoom;
            clampZoom();

            // Adjust offset to keep world center in same screen position
            Point newScreenCenter = worldToScreen(worldCenter, viewSize);
            offsetX += (centerPoint.x - newScreenCenter.x) / zoom;
            offsetY += (centerPoint.y - newScreenCenter.y) / zoom;
        }
    };

}
#endif