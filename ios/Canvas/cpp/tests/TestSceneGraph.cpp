#include "../scene-graph/SceneGraph.h"
#include <iostream>
#include <cassert>

using namespace CanvasMVP;

std::unique_ptr<Node> makeNode(const std::string &id, float x, float y)
{
    return std::make_unique<Node>(id, NodeType::RECTANGLE, x, y, 100, 100);
}

void testAddAndGet()
{
    std::cout << "Test: Add and Get" << std::endl;

    SceneGraph graph;

    assert(graph.addNode(makeNode("node1", 0, 0)) == true);
    assert(graph.nodeCount() == 1);

    Node *node = graph.getNode("node1");
    assert(node != nullptr);
    assert(node->id == "node1");
    std::cout << "  ✓ Can add and retrieve node" << std::endl;

    // Duplicate ID should fail
    assert(graph.addNode(makeNode("node1", 100, 100)) == false);
    assert(graph.nodeCount() == 1);
    std::cout << "  ✓ Rejects duplicate ID" << std::endl;

    // Non-existent node
    assert(graph.getNode("node999") == nullptr);
    std::cout << "  ✓ Returns nullptr for missing node" << std::endl;
}

void testRemove()
{
    std::cout << "Test: Remove" << std::endl;

    SceneGraph graph;
    graph.addNode(makeNode("node1", 0, 0));
    graph.addNode(makeNode("node2", 100, 100));

    assert(graph.removeNode("node1") == true);
    assert(graph.nodeCount() == 1);
    assert(graph.getNode("node1") == nullptr);
    std::cout << "  ✓ Can remove node" << std::endl;

    assert(graph.removeNode("node1") == false);
    std::cout << "  ✓ Can't remove twice" << std::endl;
}

void testZIndexSort()
{
    std::cout << "Test: Z-index Sorting" << std::endl;

    SceneGraph graph;

    auto n1 = makeNode("node1", 0, 0);
    n1->zIndex = 5;
    auto n2 = makeNode("node2", 0, 0);
    n2->zIndex = 1;
    auto n3 = makeNode("node3", 0, 0);
    n3->zIndex = 10;

    graph.addNode(std::move(n1));
    graph.addNode(std::move(n2));
    graph.addNode(std::move(n3));

    const auto &sorted = graph.getSortedNodes();
    assert(sorted.size() == 3);
    assert(sorted[0]->zIndex == 1);
    assert(sorted[1]->zIndex == 5);
    assert(sorted[2]->zIndex == 10);
    std::cout << "  ✓ Nodes sorted correctly" << std::endl;
}

int main()
{
    std::cout << "Running SceneGraph tests..." << std::endl;
    testAddAndGet();
    testRemove();
    testZIndexSort();
    std::cout << "✅ All SceneGraph tests passed!" << std::endl;
    return 0;
}