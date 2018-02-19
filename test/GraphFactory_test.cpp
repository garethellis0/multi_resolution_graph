// Testing Includes
#include <gtest/gtest.h>

// C++ STD Includes
#include <memory>

// Thunderbots Includes
#include <GraphFactory.h>

// TODO: Finish Tests
class GraphFactoryTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(GraphFactoryTest, default_constructor){
    GraphFactory<nullptr_t> graph_factory;
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 1);
    EXPECT_EQ(generated_graph.getResolution(), 2);
}

TEST_F(GraphFactoryTest, setGraphSize){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphSize(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 10);
    EXPECT_EQ(generated_graph.getResolution(), 2);
}

TEST_F(GraphFactoryTest, setMaxScaleAtPoint_0_0){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setMaxScaleAtPoint((Coordinates) {0, 0}, 0.25);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get the top-level subnodes
    std::vector<std::vector<Node<nullptr_t>*>> top_level_sub_nodes = generated_graph.getSubNodes();

    // We should have expanded the bottom left node, so it should now be a GraphNode
    Node<nullptr_t>* expanded_node = top_level_sub_nodes[0][0];
    EXPECT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

    EXPECT_EQ(new_graph_node->getResolution(), 2);
    EXPECT_EQ(new_graph_node->getScale(), 0.5);
}

TEST_F(GraphFactoryTest, setMaxScaleAtPoint_1_1){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setMaxScaleAtPoint((Coordinates) {1, 1}, 0.25);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get the top-level subnodes
    std::vector<std::vector<Node<nullptr_t>*>> top_level_sub_nodes = generated_graph.getSubNodes();

    // We should have expanded the bottom left node, so it should now be a GraphNode
    Node<nullptr_t>* expanded_node = top_level_sub_nodes[1][1];
    EXPECT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

    EXPECT_EQ(new_graph_node->getResolution(), 2);
    EXPECT_EQ(new_graph_node->getScale(), 0.5);
}

TEST_F(GraphFactoryTest, setMaxScaleInArea_Rectangle){
    GraphFactory<nullptr_t> graph_factory;
    GraphFactory<nullptr_t>::Rectangle rectangle = GraphFactory<nullptr_t>::Rectangle(1,1,(Coordinates){1,1});
    graph_factory.setMaxScaleInArea(rectangle, 0.25);

    // TODO: YOU ARE HERE - FINISH ME
}

// TODO: YOU ARE HERE: Finish these tests
// TODO: Test setMaxScaleInArea (at least one test for every area type)
// TODO: Test generating a massive graph with lots of areas and points with increased resolution

// TODO: Is this even a valid test?
// Test generating a giant graph with lots of random shapes and resolutions
// (mostly for runtime testing)
TEST_F(GraphFactoryTest, generate_large_and_complex_graph){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphSize(100);

    // Generate some rectangles
    for (int i = 0; i < 20; i++){
        GraphFactory<nullptr_t>::Rectangle rectangle = GraphFactory<nullptr_t>::Rectangle(2*i,3*i,(Coordinates){i,i});
        graph_factory.setMaxScaleInArea(rectangle, 0.01);
    }

    auto graph = graph_factory.createGraph();

    // Massive time dump, get all real nodes and all neighbours of all nodes
    std::function<bool(Node<nullptr_t>&)> filter = [&](Node<nullptr_t>& n) { return true; };

    std::vector<RealNode<nullptr_t>*> matching_nodes = graph.getAllNodesThatPassFilter(filter);

    std::cout << matching_nodes.size();

    for (auto& node : matching_nodes){
        node->getNeighbours();
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

