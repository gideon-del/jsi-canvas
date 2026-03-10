#include "../../vector-engine/math/Primitives.h"
#include "../../vector-engine/math/vec2.h"
#include "../../vector-engine/debug/SvgWriter.h"
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

    Vec2 p0{50, 150}, p1{150, 30}, p2{350, 30}, p3{450, 150};

    svg.comment("=== Basic Curve with Debug Info ===");
    svg.bezierDebug(p0, p1, p2, p3);
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
int main()
{
    testCubicBezier();

    return 0;
}