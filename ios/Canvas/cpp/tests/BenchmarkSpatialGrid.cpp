#include "../scene-graph/SceneGraph.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

using namespace CanvasMVP;
using namespace std::chrono;
template <typename Func>
double measureMs(Func f, int iterations = 1)
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        f();
    }
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count() / 1000.0 / iterations;
}

std::vector<std::unique_ptr<Node>> generateNodes(int count)
{
    std::vector<std::unique_ptr<Node>> nodes;
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<float> posX(-1000, 5000);
    std::uniform_real_distribution<float> posY(-1000, 5000);
    std::uniform_real_distribution<float> size(50, 300);

    for (int i = 0; i < count; ++i)
    {
        nodes.push_back(std::make_unique<Node>(
            "node_" + std::to_string(i),
            NodeType::RECTANGLE,
            posX(gen), posY(gen), size(gen), size(gen)));
    }
    return nodes;
}

void benchmarkAdd()
{
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "BENCHMARK 1: Add Performance" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
              << std::endl;
    std::cout << "Target: < 100μs per node\n"
              << std::endl;

    for (int count : {100, 1000, 10000})
    {
        SceneGraph graph;
        auto nodes = generateNodes(count);

        double timeMs = measureMs([&]()
                                  {
            for (auto& node : nodes) {
                graph.addNode(std::move(node));
            } });

        double perNodeUs = (timeMs / count) * 1000;
        bool meetsTarget = perNodeUs < 100;

        std::cout << "  " << std::setw(6) << count << " nodes: ";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(7) << timeMs << "ms total, ";
        std::cout << std::setw(6) << perNodeUs << "μs/node";
        std::cout << (meetsTarget ? " ✅" : " ❌") << std::endl;
    }
}

void benchmarkQuery()
{
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "BENCHMARK 2: Query Performance" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
              << std::endl;
    std::cout << "Target: < 5ms for 10,000 nodes\n"
              << std::endl;

    Rect viewport(500, 500, 1000, 800); // Typical screen

    for (int count : {100, 1000, 10000})
    {
        SceneGraph graph;
        auto nodes = generateNodes(count);
        for (auto &node : nodes)
            graph.addNode(std::move(node));

        double timeMs = measureMs([&]()
                                  { graph.queryVisible(viewport); }, 100); // Average over 100 queries

        auto visible = graph.queryVisible(viewport);
        bool meetsTarget = (count == 10000) ? (timeMs < 5.0) : true;

        std::cout << "  " << std::setw(6) << count << " nodes: ";
        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::setw(6) << timeMs << "ms (" << visible.size() << " visible)";
        std::cout << (meetsTarget ? " ✅" : " ❌") << std::endl;
    }
}

void benchmarkComparison()
{
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "BENCHMARK 3: Grid vs Linear Search" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
              << std::endl;
    std::cout << "Target: 15-50x speedup\n"
              << std::endl;

    const int count = 10000;
    SceneGraph graph;
    auto testNodes = generateNodes(count);

    std::vector<Node *> nodeList;
    for (auto &node : testNodes)
    {
        nodeList.push_back(node.get());
        graph.addNode(std::move(node));
    }

    Rect viewport(500, 500, 1000, 800);

    // Linear search
    double linearMs = measureMs([&]()
                                {
        std::vector<Node*> results;
        for (Node* node : nodeList) {
            if (viewport.intersects(node->bounds)) {
                results.push_back(node);
            }
        } }, 100);

    // Grid search
    double gridMs = measureMs([&]()
                              { graph.queryVisible(viewport); }, 100);

    double speedup = linearMs / gridMs;
    bool meetsTarget = speedup >= 15.0;

    std::cout << "  Linear: " << std::fixed << std::setprecision(3);
    std::cout << std::setw(7) << linearMs << "ms" << std::endl;
    std::cout << "  Grid:   " << std::setw(7) << gridMs << "ms" << std::endl;
    std::cout << "  Speedup: " << std::fixed << std::setprecision(1);
    std::cout << std::setw(6) << speedup << "x";
    std::cout << (meetsTarget ? " ✅" : " ❌") << std::endl;

    if (speedup >= 50)
    {
        std::cout << "  🌟 Exceptional!" << std::endl;
    }
    else if (speedup >= 30)
    {
        std::cout << "  🎯 Excellent!" << std::endl;
    }
    else if (speedup >= 15)
    {
        std::cout << "  ✓ Good!" << std::endl;
    }
}

void benchmarkHitTest()
{
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "BENCHMARK 4: Hit Test Performance" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
              << std::endl;
    std::cout << "Target: < 0.1ms\n"
              << std::endl;

    const int count = 10000;
    SceneGraph graph;
    auto nodes = generateNodes(count);
    for (auto &node : nodes)
        graph.addNode(std::move(node));

    double timeMs = measureMs([&]()
                              { graph.hitTest(1000, 1000); }, 1000);

    std::cout << "  " << count << " nodes: ";
    std::cout << std::fixed << std::setprecision(3) << timeMs << "ms";
    std::cout << (timeMs < 0.1 ? " ✅" : " ❌") << std::endl;
}

void benchmarkMemory()
{
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "BENCHMARK 5: Memory Usage" << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
              << std::endl;
    std::cout << "Target: < 5MB for 10,000 nodes\n"
              << std::endl;

    const int count = 10000;
    SceneGraph graph;
    auto nodes = generateNodes(count);
    for (auto &node : nodes)
        graph.addNode(std::move(node));

    size_t nodeData = count * sizeof(Node);
    size_t mapOverhead = count * 48;
    size_t sortedList = count * 8;
    size_t spatialEntries = graph.spatialIndex_.totalEntries() * 8;
    size_t gridOverhead = graph.spatialIndex_.cellCount() * 48;

    size_t totalBytes = nodeData + mapOverhead + sortedList +
                        spatialEntries + gridOverhead;
    double totalMB = totalBytes / (1024.0 * 1024.0);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Node data:     " << (nodeData / 1024) << " KB" << std::endl;
    std::cout << "  Map overhead:  " << (mapOverhead / 1024) << " KB" << std::endl;
    std::cout << "  Sorted list:   " << (sortedList / 1024) << " KB" << std::endl;
    std::cout << "  Spatial index: " << (spatialEntries / 1024) << " KB" << std::endl;
    std::cout << "  Grid overhead: " << (gridOverhead / 1024) << " KB" << std::endl;
    std::cout << "  ─────────────────────" << std::endl;
    std::cout << "  TOTAL:         " << totalMB << " MB";
    std::cout << (totalMB < 5.0 ? " ✅" : " ❌") << std::endl;

    std::cout << "\n  Grid stats:" << std::endl;
    std::cout << "  • Cells: " << graph.spatialIndex_.cellCount() << std::endl;
    std::cout << "  • Entries: " << graph.spatialIndex_.totalEntries() << std::endl;
    std::cout << "  • Avg/cell: " << std::fixed << std::setprecision(1);
    std::cout << (float)graph.spatialIndex_.totalEntries() / graph.spatialIndex_.cellCount() << std::endl;
}

int main()
{
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║   SPATIAL GRID BENCHMARKS                ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;

    benchmarkAdd();
    benchmarkQuery();
    benchmarkComparison();
    benchmarkHitTest();
    benchmarkMemory();

    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║   ALL BENCHMARKS COMPLETE                ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝\n"
              << std::endl;

    return 0;
}