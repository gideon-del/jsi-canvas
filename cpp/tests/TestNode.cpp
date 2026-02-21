#include "../types/Node.h"
#include <iostream>
#include <cassert>

using namespace CanvasMVP;

void testRect()
{
    std::cout << "Testing Rect..." << std::endl;

    Rect r(100, 100, 200, 150);

    // Test contains

    assert(r.contains(150, 150) == true);  // Inside
    assert(r.contains(100, 100) == true);  // Top-left corner
    assert(r.contains(300, 250) == true);  // Bottom-right corner
    assert(r.contains(50, 50) == false);   // Outside
    assert(r.contains(350, 300) == false); // Outside
    std::cout << "  ✓ Rect::contains works" << std::endl;

    Rect r1(0, 0, 100, 100);
    Rect r2(50, 50, 100, 100);
    assert(r1.intersects(r2) == true); // Overlap

    Rect r3(200, 200, 100, 100);
    assert(r1.intersects(r3) == false); // Separate
    std::cout << "  ✓ Rect::intersects works" << std::endl;
};

void testNode()
{
    std::cout << "Testing Node..." << std::endl;

    Node node("test1", NodeType::RECTANGLE, 50, 50, 100, 100);

    assert(node.data.id == "test1");
    assert(node.data.bounds.x == 50);
    assert(node.data.bounds.y == 50);
    assert(node.data.visible == true);
    assert(node.data.selected == false);
    std::cout << "  ✓ Node constructor works" << std::endl;

    assert(node.containsPoint(100, 100) == true);
    assert(node.containsPoint(0, 0) == false);
    std::cout << "  ✓ Node::containsPoint works" << std::endl;
}

int main()
{
    std::cout << "Running Node tests..." << std::endl;
    testRect();
    testNode();
    std::cout << "✅ All Node tests passed!" << std::endl;
    return 0;
}
