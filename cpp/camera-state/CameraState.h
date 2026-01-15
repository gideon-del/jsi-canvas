#pragma once

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

        Point worldToScreen(const Point &worldPoint) const
        {
            return Point{
                (worldPoint.x + offsetX) * zoom,
                (worldPoint.y + offsetY) * zoom};
        }

        Point screenToWorld(const Point &screenPoint) const
        {

            return Point{
                (screenPoint.x / zoom) - offsetX,
                (screenPoint.y / zoom) - offsetY};
        }

        RectF worldToScreen(const RectF &worldRect) const
        {
            Point topLeft = worldToScreen(
                Point{worldRect.x, worldRect.y});

            return RectF{
                topLeft.x,
                topLeft.y,
                worldRect.width * zoom,
                worldRect.height * zoom};
        }

        RectF screenToWorld(const RectF &screenRect) const
        {
            Point topLeft = screenToWorld(
                Point{screenRect.x, screenRect.y});

            return RectF{
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

        void zoomAt(float newZoom, const Point &centerPoint)
        {
            // Get world position of zoom center
            Point worldCenter = screenToWorld(centerPoint);

            // Apply new zoom
            zoom = newZoom;
            clampZoom();

            // Adjust offset to keep world center in same screen position
            Point newScreenCenter = worldToScreen(worldCenter);
            offsetX += (centerPoint.x - newScreenCenter.x) / zoom;
            offsetY += (centerPoint.y - newScreenCenter.y) / zoom;
        }
    };

}