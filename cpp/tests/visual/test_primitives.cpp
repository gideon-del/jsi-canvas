#include <iomanip>
#include "../../vector-engine/math/Primitives.h"
#include "../../vector-engine/math/vec2.h"
#include "../../vector-engine/debug/SvgWriter.h"
#include "../../vector-engine/path/PathPoint.h"
#include "../../vector-engine/path/Segment.h"
#include "../../vector-engine/path/Path.h"

void drawPathPoint(SvgWriter &svg, const PathPoint &p, std::string label)
{
    Vec2 pos = p.position;
    Vec2 in = p.absoluteHandleIn();
    Vec2 out = p.absoluteHandleOut();

    // anchor
    svg.point(pos, "#2196F3", 6);

    // handles
    if (p.hasHandleIn())
    {
        svg.line(pos, in, "#aaa", 1);
        svg.point(in, "#F44336", 4);
    }

    if (p.hasHandleOut())
    {
        svg.line(pos, out, "#aaa", 1);
        svg.point(out, "#F44336", 4);
    }

    svg.text(pos + Vec2{8, -8}, label, "black", 12);
}
void testPrimitives()
{
    SvgWriter svg({0, 0, 600, 500});
    svg.grid(50, "#eee");

    svg.comment("=== Vec2 Operations ===");

    Vec2 a{50, 100};
    Vec2 b{250, 50};

    svg.line(a, b, "#ccc", 1);
    svg.labeledPoint(a, "a", "#2196F3");
    svg.labeledPoint(b, "b", "#2196F3");

    for (double t = 0.25; t <= 0.75; t += 0.25)
    {
        Vec2 p = a.lerp(b, t);
        svg.point(p, "#4CAF50", 5);
        std::ostringstream label;
        label << "t=" << t;
        svg.text(p + Vec2{5, 15}, label.str(), "#4CAF50", 10);

        Vec2 mid = a.lerp(b, 0.5);
        Vec2 dir = (b - a).normalized();
        Vec2 perp = dir.perpendicular() * 40;
        svg.arrow(mid, mid + perp, "#9C27B0", 2);
        svg.text(mid + perp + Vec2{5, 0}, "perpendicular", "#9C27B0", 10);
    }

    svg.comment("=== Line Intersection ===");
    Line line1{{50, 200}, Vec2{1, 0.3}.normalized()};
    Line line2{{50, 350}, Vec2{1, -0.4}.normalized()};

    svg.line(line1.pointAt(0), line1.pointAt(300), "#FF5722", 2);
    svg.line(line2.pointAt(0), line2.pointAt(300), "#3F51B5", 2);

    auto isect = line1.intersect(line2);
    if (isect)
    {
        Vec2 point = line1.pointAt(isect->first);
        svg.point(point, "#E91E63", 8);
        svg.text(point + Vec2{10, -5}, "intersection", "#E91E63", 12);
    }

    svg.comment("=== Rect Operations ===");

    Rect r1{350, 150, 100, 80};
    Rect r2{400, 190, 120, 100};

    svg.rect(r1, "#2196F3", "rgba(33,150,243,0.2)", 2);
    svg.rect(r2, "#4CAF50", "rgba(76,175,80,0.2)", 2);

    svg.text({r1.x + 5, r1.y + 15}, "rect1", "#2196F3", 10);
    svg.text({r2.x + 5, r2.y + 15}, "rect2", "#4CAF50", 10);

    if (r1.intersects(r2))
    {
        Rect united = r1.united(r2);
        svg.rect(united, "#FF9800", "none", 1);
        svg.text({united.x, united.y - 5}, "united", "#FF9800", 10);
    }
    Rect container{350, 350, 200, 100};
    svg.rect(container, "#607D8B", "rgba(96,125,139,0.1)", 2);

    std::vector<Vec2> testPoints = {
        {400, 400}, // inside
        {450, 380}, // inside
        {300, 400}, // outside left
        {560, 400}, // outside right
        {450, 340}, // outside top
    };

    for (auto &p : testPoints)
    {
        std::string color = container.contains(p) ? "#4CAF50" : "#F44336";
        svg.point(p, color, 6);
    }

    svg.text({container.x, container.y - 5}, "contains test (green=inside)", "#607D8B", 10);

    bool saved = svg.save("visual/output/primitives.svg");
    if (saved)
    {

        std::cout << "Saved output/primitives.svg" << std::endl;
    }
}

void testCubicBezier()
{
    SvgWriter svg({0, 0, 600, 800});
    svg.grid(50, "#f5f5f5");

    Vec2 p0{50, 150}, p1{150, 30}, p2{550, 30}, p3{450, 150};

    svg.comment("=== Basic Curve with Debug Info ===");
    svg.bezierDebug(p0, p1, p2, p3);
    auto testBezier = CubicBezier(p0, p1, p2, p3);
    auto box = testBezier.boundingBox();
    svg.boundingBox(box, "#00f");
    svg.text({50, 20}, "Basic cubic bezier with control points", "black", 14);

    svg.comment("=== Sample Points ===");
    // Offset for second example
    Vec2 offset{0, 200};
    Vec2 q0 = p0 + offset, q1 = p1 + offset, q2 = p2 + offset, q3 = p3 + offset;

    svg.cubicBezier(q0, q1, q2, q3, "#ccc", 3);
    svg.bezierSamples(q0, q1, q2, q3, 20);
    svg.text({50, 220}, "Sample points along curve (t = 0 to 1)", "black", 14);

    svg.comment("=== Tangent Vectors ===");
    offset = {0, 400};
    Vec2 r0 = p0 + offset, r1 = p1 + offset, r2 = p2 + offset, r3 = p3 + offset;

    svg.cubicBezier(r0, r1, r2, r3, "#ccc", 3);
    svg.bezierTangents(r0, r1, r2, r3, 8, 40);
    svg.text({50, 420}, "Tangent vectors", "black", 14);

    svg.comment("=== de Casteljau Subdivision ===");
    offset = {0, 600};
    Vec2 s0 = p0 + offset, s1 = p1 + offset, s2 = p2 + offset, s3 = p3 + offset;

    svg.deCasteljauDebug(s0, s1, s2, s3, 0.35);
    svg.text({50, 620}, "de Casteljau at t=0.35", "black", 14);

    // for (int i = 0; i <= 4; i++)
    // {
    //     Vec2 newOffset = offset + Vec2{0, offset.y * (i - 1)};
    //     s0 = p0 + newOffset;
    //     s1 = p1 + newOffset;
    //     s2 = p2 + newOffset;
    //     s3 = p3 + newOffset;
    //     svg.deCasteljauDebug(s0, s1, s2, s3, i * 0.25);
    // }
    bool saved = svg.save("visual/output/bezier.svg");
    if (saved)
    {

        std::cout << "Saved output/bezier.svg" << std::endl;
    }
}
void testBoundingBox()
{
    SvgWriter svg({0, 0, 600, 900});
    svg.grid(50, "#f5f5f5");

    auto testCase = [&](Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3,
                        const std::string &label, double offsetY)
    {
        Vec2 o{0, offsetY};
        Vec2 a = p0 + o, b = p1 + o, c = p2 + o, d = p3 + o;

        svg.bezierDebug(a, b, c, d);
        auto box = CubicBezier(a, b, c, d).boundingBox();
        svg.boundingBox(box, "#00f");
        svg.text({10, offsetY - 8}, label, "#333", 11);
    };

    // 1. Symmetric arch — both endpoints same Y, bulges above
    //    Box must capture the top bulge (Y extremum at t=0.5)
    testCase({50, 150}, {150, 30}, {350, 30}, {450, 150},
             "1. Symmetric arch — Y extremum at t=0.5", 0);

    // 2. S-curve — goes above AND below the endpoint Y
    //    Two Y extrema, box must wrap both bulges
    testCase({50, 150}, {250, 30}, {250, 270}, {450, 150},
             "2. S-curve — two Y extrema", 180);

    // 3. Horizontal bulge — control points push left/right
    //    X extrema matter here, not just Y
    testCase({150, 50}, {30, 80}, {30, 120}, {150, 150},
             "3. Left bulge — X extremum", 340);

    // 4. Asymmetric — P1 up, P2 down (your original failing case)
    //    Both a Y-max and a Y-min inside (0,1)
    testCase({50, 150}, {150, 30}, {350, 300}, {450, 150},
             "4. Asymmetric — P1 up / P2 down", 500);

    // 5. Near-straight — control points barely off the line
    //    Box should be almost identical to just the endpoints
    testCase({50, 150}, {150, 145}, {350, 155}, {450, 150},
             "5. Near-straight — box hugs endpoints", 700);

    svg.save("visual/output/bbox_tests.svg");
    std::cout << "Saved output/bbox_tests.svg" << std::endl;
}

void testSubdivision()
{
    SvgWriter svg({0, 0, 600, 1000});
    svg.grid(50, "#f5f5f5");

    Vec2 p0{50, 150}, p1{150, 30}, p2{350, 300}, p3{450, 150};

    // 1. splitAt — show the two halves in different colors
    svg.text({10, 20}, "1. splitAt(0.5) — two halves", "black", 12);
    {
        auto bez = CubicBezier(p0, p1, p2, p3);
        auto [left, right] = bez.splitAt(0.5);

        svg.cubicBezier(left.p0, left.p1, left.p2, left.p3, "#E91E63", 2);
        svg.cubicBezier(right.p0, right.p1, right.p2, right.p3, "#2196F3", 2);

        // Show the split point
        svg.point(left.p3, "#4CAF50", 6);        // = right.p0, the midpoint
        svg.bezierDebug(p0, p1, p2, p3, "#ccc"); // original underneath
    }

    // 2. flatten — different tolerances, count the segments
    Vec2 o{0, 220};
    svg.text({10, 220}, "2. flatten — tolerance 20 (red) vs 2 (green)", "black", 12);
    {
        auto bez = CubicBezier(p0 + o, p1 + o, p2 + o, p3 + o);

        auto coarse = bez.flatten(20);
        auto fine = bez.flatten(2);

        // Draw polylines
        for (size_t i = 1; i < coarse.size(); i++)
            svg.line(coarse[i - 1], coarse[i], "#F44336", 1.5);
        for (auto &pt : coarse)
            svg.point(pt, "#F44336", 4);

        for (size_t i = 1; i < fine.size(); i++)
            svg.line(fine[i - 1], fine[i], "#4CAF50", 1);
        for (auto &pt : fine)
            svg.point(pt, "#4CAF50", 3);

        // Segment counts as labels
        svg.text({10, 390}, "coarse segments: " + std::to_string(coarse.size() - 1), "#F44336", 10);
        svg.text({10, 403}, "fine segments:   " + std::to_string(fine.size() - 1), "#4CAF50", 10);
    }

    // 3. subdivide(n) — uniform split into equal t-intervals
    o = {0, 470};
    svg.text({10, 470}, "3. subdivide(4) — 4 equal pieces", "black", 12);
    {
        std::vector<std::string> colors = {"#E91E63", "#FF9800", "#4CAF50", "#2196F3"};
        auto bez = CubicBezier(p0 + o, p1 + o, p2 + o, p3 + o);
        auto pieces = bez.subdivide(4);

        for (size_t i = 0; i < pieces.size(); i++)
        {
            auto &b = pieces[i];
            auto color = colors[i % colors.size()];
            svg.cubicBezier(b.p0, b.p1, b.p2, b.p3, color, 2);
            svg.bezierDebug(b.p0, b.p1, b.p2, b.p3, color);
            svg.point(b.p0, colors[i % colors.size()], 5); // junction points
        }
        svg.point(pieces.back().p3, colors[3], 5);
    }

    // 4. flatness — visualize which regions are flat vs curved
    o = {0, 700};
    svg.text({10, 700}, "4. flatness per subdivided piece", "black", 12);
    {
        auto bez = CubicBezier(p0 + o, p1 + o, p2 + o, p3 + o);
        auto pieces = bez.subdivide(8);

        for (auto &b : pieces)
        {
            double f = b.flatness();
            // Red = very curved, green = very flat
            int r = std::min(255, static_cast<int>(f * 2));
            int g = 255 - r;
            char color[32];
            std::snprintf(color, sizeof(color), "rgb(%d,%d,50)", r, g);

            svg.cubicBezier(b.p0, b.p1, b.p2, b.p3, color, 3);

            // Label each piece with its flatness value
            Vec2 mid = b.evaluate(0.5);
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << f;
            svg.text(mid + Vec2{0, -8}, ss.str(), "#333", 9);
        }
    }

    svg.save("visual/output/subdivision_tests.svg");
    std::cout << "Saved output/subdivision_tests.svg" << std::endl;
}
void testPathPoint()
{
    SvgWriter svg({0, 0, 600, 400});
    svg.grid(50, "#eee");

    // Corner
    PathPoint corner;
    corner.position = {100, 200};
    corner.setType(PointType::Corner);
    corner.setHandleOut({80, -80});
    corner.setHandleIn({-40, -40});

    drawPathPoint(svg, corner, "Corner");

    // Smooth
    PathPoint smooth;
    smooth.position = {300, 200};
    smooth.setType(PointType::Smooth);
    smooth.setHandleOut({80, -80});
    smooth.setHandleIn({-20, 80});

    drawPathPoint(svg, smooth, "Smooth");

    // Symmetric
    PathPoint symmetric;
    symmetric.position = {500, 200};
    symmetric.setType(PointType::Symmetric);
    symmetric.setHandleOut({100, -80});

    drawPathPoint(svg, symmetric, "Symmetric");

    svg.save("visual/output/pathpoint.svg");
}

void testSegment()
{
    SvgWriter svg({0, 0, 600, 700});
    svg.grid(50, "#eee");

    // ─── Helper ───────────────────────────────────────────────────
    auto drawSegment = [&](PathPoint &a, PathPoint &b,
                           const std::string &label, Vec2 offset)
    {
        PathPoint ao = a, bo = b;
        ao.position = a.position + offset;
        bo.position = b.position + offset;

        Segment seg(&ao, &bo);

        // Curve or line
        if (seg.isLine())
            svg.line(ao.position, bo.position, "#000", 2);
        else
            svg.cubicBezier(ao.position,
                            ao.absoluteHandleOut(),
                            bo.absoluteHandleIn(),
                            bo.position, "#000", 2);

        // Handle lines
        if (ao.hasHandleOut())
        {
            svg.line(ao.position, ao.absoluteHandleOut(), "#aaa", 1);
            svg.point(ao.absoluteHandleOut(), "#F44336", 4);
        }
        if (bo.hasHandleIn())
        {
            svg.line(bo.position, bo.absoluteHandleIn(), "#aaa", 1);
            svg.point(bo.absoluteHandleIn(), "#F44336", 4);
        }

        // Anchors
        svg.point(ao.position, "#2196F3", 6);
        svg.point(bo.position, "#2196F3", 6);

        // Sample pointAt + tangentAt + normalAt at t=0.25, 0.5, 0.75
        for (double t : {0.25, 0.5, 0.75})
        {
            Vec2 p = seg.pointAt(t);
            Vec2 tan = seg.tangentAt(t).normalized() * 20;
            Vec2 nor = seg.normalAt(t).normalized() * 15;

            svg.point(p, "#4CAF50", 3);
            svg.line(p, p + tan, "#9C27B0", 1); // tangent purple
            svg.line(p, p + nor, "#00BCD4", 1); // normal cyan
        }

        // Bounding box
        Rect box = seg.boundingBox();
        // box.x += offset.x;
        // box.y += offset.y;
        svg.boundingBox(box, "#00f");

        svg.text(ao.position + Vec2{0, -20}, label, "#333", 11);
    };

    // ─── 1. Line segment (no handles) ────────────────────────────
    // isLine() should return true, pointAt uses lerp, tangent is constant
    {
        PathPoint a, b;
        a.position = {80, 120};
        b.position = {300, 80};
        // no handles set
        drawSegment(a, b, "1. Line — isLine()=true, constant tangent", {0, 0});
    }

    // ─── 2. Curve segment (both handles) ─────────────────────────
    // isLine() false, toCubicBezier() used, bbox wraps the bulge
    {
        PathPoint a, b;
        a.position = {80, 120};
        a.setHandleOut({80, -80});
        b.position = {400, 120};
        b.setHandleIn({-80, -80});
        drawSegment(a, b, "2. Curve — symmetric arch, bbox above endpoints", {0, 160});
    }

    // ─── 3. Only handleOut on start ───────────────────────────────
    // One-sided curve — asymmetric shape
    {
        PathPoint a, b;
        a.position = {80, 120};
        a.setHandleOut({120, -90});
        b.position = {400, 120};
        // no handleIn on b
        drawSegment(a, b, "3. Partial handle — only handleOut on start", {0, 330});
    }

    // ─── 4. S-curve ───────────────────────────────────────────────
    // handleOut and handleIn pull in opposite vertical directions
    // bbox must expand both above and below
    {
        PathPoint a, b;
        a.position = {80, 120};
        a.setHandleOut({100, -90});
        b.position = {400, 120};
        b.setHandleIn({-100, 90}); // opposite direction → S
        drawSegment(a, b, "4. S-curve — bbox expands up AND down", {0, 490});
    }

    svg.save("visual/output/segment_tests.svg");
    std::cout << "Saved output/segment_tests.svg" << std::endl;
}
void testPath()
{
    SvgWriter svg({0, 0, 700, 800});
    svg.grid(50, "#eee");

    auto drawPath = [&](const Path &path, Vec2 offset, std::string color)
    {
        // Draw each segment
        for (size_t i = 0; i < path.segmentCount(); i++)
        {
            Segment seg = path.getSegment(i);
            CubicBezier bez = seg.toCubicBezier();

            if (seg.isLine())
                svg.line(seg.start().position + offset,
                         seg.end().position + offset, color, 2);
            else
                svg.cubicBezier(
                    bez.p0 + offset, bez.p1 + offset,
                    bez.p2 + offset, bez.p3 + offset, color, 2);
        }

        // Draw points
        for (size_t i = 0; i < path.pointCount(); i++)
        {
            const PathPoint &pt = path.pointAt(i);
            Vec2 pos = pt.position + offset;
            svg.point(pos, "#2196F3", 5);

            if (pt.hasHandleOut())
            {
                Vec2 hout = pt.absoluteHandleOut() + offset;
                svg.line(pos, hout, "#aaa", 1);
                svg.point(hout, "#F44336", 3);
            }
            if (pt.hasHandleIn())
            {
                Vec2 hin = pt.absoluteHandleIn() + offset;
                svg.line(pos, hin, "#aaa", 1);
                svg.point(hin, "#E91E63", 3);
            }

            // index label
            std::ostringstream ss;
            ss << i;
            svg.text(pos + Vec2{6, -8}, ss.str(), "#555", 10);
        }

        // Bounding box
        if (path.pointCount() > 0)
        {
            Rect box = path.bounds();
            box.x += offset.x;
            box.y += offset.y;
            svg.boundingBox(box, "#00f");
        }
    };

    // ─── 1. Open path — 4 points, 3 segments ─────────────────────
    // segmentCount should be 3
    svg.text({20, 22}, "1. Open path — 4 pts, 3 segs, bbox hugs curve", "#333", 11);
    {
        Path path;
        PathPoint a;
        a.position = {60, 100};
        a.setHandleOut({60, -60});
        PathPoint b;
        b.position = {200, 80};
        b.setHandleIn({-40, 40});
        b.setHandleOut({40, -40});
        PathPoint c;
        c.position = {360, 120};
        c.setHandleIn({-60, 0});
        c.setHandleOut({60, 0});
        PathPoint d;
        d.position = {500, 80};
        d.setHandleIn({-60, 60});

        path.addPoint(a);
        path.addPoint(b);
        path.addPoint(c);
        path.addPoint(d);

        std::cout << "1. segmentCount (expect 3): " << path.segmentCount() << "\n";
        std::cout << "1. isClosed    (expect 0): " << path.isClosed() << "\n";

        drawPath(path, {0, 0}, "#000");
    }

    // ─── 2. Closed path — same 4 points, now 4 segments ──────────
    // closing adds segment from last → first
    svg.text({20, 222}, "2. Closed path — 4 pts, 4 segs (last→first added)", "#333", 11);
    {
        Path path;
        PathPoint a;
        a.position = {60, 300};
        a.setHandleOut({40, -60});
        PathPoint b;
        b.position = {200, 260};
        b.setHandleIn({-40, 40});
        b.setHandleOut({40, -40});
        PathPoint c;
        c.position = {360, 320};
        c.setHandleIn({-60, 0});
        c.setHandleOut({60, 0});
        PathPoint d;
        d.position = {500, 280};
        d.setHandleIn({-60, 60});
        d.setHandleOut({-40, 60});

        path.addPoint(a);
        path.addPoint(b);
        path.addPoint(c);
        path.addPoint(d);
        path.close();

        std::cout << "2. segmentCount (expect 4): " << path.segmentCount() << "\n";
        std::cout << "2. isClosed    (expect 1): " << path.isClosed() << "\n";

        drawPath(path, {0, 0}, "#000");
    }

    // ─── 3. insertPoint — insert a point at index 1 ───────────────
    // should appear between P0 and old P1
    svg.text({20, 422}, "3. insertPoint at index 1 — green dot is inserted point", "#333", 11);
    {
        Path path;
        PathPoint a;
        a.position = {60, 520};
        PathPoint b;
        b.position = {500, 520};
        path.addPoint(a);
        path.addPoint(b);

        PathPoint mid;
        mid.position = {280, 420};
        mid.setHandleOut({60, 0});
        mid.setHandleIn({-60, 0});
        path.insertPoint(1, mid);

        std::cout << "3. pointCount after insert (expect 3): " << path.pointCount() << "\n";
        std::cout << "3. segmentCount            (expect 2): " << path.segmentCount() << "\n";

        drawPath(path, {0, 0}, "#000");
        // highlight the inserted point
        svg.point(path.pointAt(1).position, "#4CAF50", 8);
    }

    // ─── 4. removePoint — remove middle point ────────────────────
    // path should go back to direct line P0→P2
    svg.text({20, 602}, "4. removePoint index 1 — middle point removed", "#333", 11);
    {
        Path path;
        PathPoint a;
        a.position = {60, 700};
        PathPoint b;
        b.position = {280, 630};
        b.setHandleIn({-50, 0});
        b.setHandleOut({50, 0});
        PathPoint c;
        c.position = {500, 700};
        path.addPoint(a);
        path.addPoint(b);
        path.addPoint(c);

        std::cout << "4. before remove — segmentCount (expect 2): " << path.segmentCount() << "\n";
        path.removePoint(1);
        std::cout << "4. after  remove — segmentCount (expect 1): " << path.segmentCount() << "\n";

        drawPath(path, {0, 0}, "#000");
    }

    // ─── 5. reverse — check point order and handle swap ──────────
    svg.text({380, 422}, "5. reverse — handles swap, direction flips", "#333", 11);
    {
        Path path;
        PathPoint a;
        a.position = {420, 520};
        a.setHandleOut({60, -80});
        PathPoint b;
        b.position = {560, 480};
        b.setHandleIn({-40, 40});
        b.setHandleOut({40, 20});
        PathPoint c;
        c.position = {650, 520};
        c.setHandleIn({-60, 60});
        path.addPoint(a);
        path.addPoint(b);
        path.addPoint(c);

        // Draw original faint
        drawPath(path, {0, 0}, "#ccc");

        path.reverse();
        // Draw reversed bold — direction should be right to left
        drawPath(path, {0, -100}, "#E91E63");

        std::cout << "5. after reverse — P0 pos (expect 650,520): "
                  << path.pointAt(0).position.x << ","
                  << path.pointAt(0).position.y << "\n";
    }

    svg.save("visual/output/path_tests.svg");
    std::cout << "Saved output/path_tests.svg" << std::endl;
}

int main()
{
    testPrimitives();
    testCubicBezier();
    testBoundingBox();
    testSubdivision();
    testPathPoint();
    testSegment();
    testPath();
    return 0;
}