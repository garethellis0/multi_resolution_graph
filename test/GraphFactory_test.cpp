// Testing Includes
#include <gtest/gtest.h>

// C++ STD Includes
#include <memory>
#include <vector>

// Thunderbots Includes
#include <GraphFactory.h>
#include <chrono>

// TODO: Finish Tests
class GraphFactoryTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

// Test that we get the expected graph from the default constructor
TEST_F(GraphFactoryTest, default_constructor){
    GraphFactory<nullptr_t> graph_factory;
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 1);
    EXPECT_EQ(generated_graph.getResolution(), 2);
}

// Test setting the top level scale of the graph
TEST_F(GraphFactoryTest, setGraphScale){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 10);
    EXPECT_EQ(generated_graph.getResolution(), 2);
}

// Test setting the top level resolution of the graph
TEST_F(GraphFactoryTest, setGraphResolution){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphTopLevelResolution(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 1);
    EXPECT_EQ(generated_graph.getResolution(), 10);
}

// Test that setting the size and resolution at the same time works
TEST_F(GraphFactoryTest, setGraphScale_and_setGraphResolution){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(20);
    graph_factory.setGraphTopLevelResolution(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 20);
    EXPECT_EQ(generated_graph.getResolution(), 10);
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

// Test setting the scale for a rectangular area which is entirely within a single node
// (that is, the node does not overlay more then a single node)
TEST_F(GraphFactoryTest, setMaxScaleInArea_Rectangle_area_within_single_node){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(1);
    graph_factory.setGraphTopLevelResolution(1);

    // Set the scale for a single rectangular area that is entirely within
    // the single top level node
    GraphFactory<nullptr_t>::Rectangle rectangle =
            GraphFactory<nullptr_t>::Rectangle(0.1,0.1,(Coordinates){0.25,0.25});
    graph_factory.setMaxScaleInArea(rectangle, 0.51);

    // Generate the graph
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get all the RealNodes
    std::function<bool(Node<nullptr_t>&)> always_true_filter = [&](Node<nullptr_t> &n) {
        return true;
    };
    std::vector<RealNode<nullptr_t>*> all_real_nodes =
            generated_graph.getAllNodesThatPassFilter(always_true_filter);

    // The area we choose should have forced the top level node to split into
    // 4 sub-nodes
    EXPECT_EQ(4, all_real_nodes.size());
    std::vector<Coordinates> expected_coordinates = {
            (Coordinates){0,0},
            (Coordinates){0.5,0},
            (Coordinates){0,0.5},
            (Coordinates){0.5,0.5},
    };
    std::vector<Coordinates> actual_coordinates;
    for (auto& node : all_real_nodes){
        actual_coordinates.emplace_back(node->getCoordinates());
    }
    // TODO: Add in GMock for unordered vector comparison
    // NOTE: List equality checks are *order-sensitive*. If it fails, check
    // that it's just not because the order has changed (which is fine)
    EXPECT_EQ(expected_coordinates, actual_coordinates);
}

// Test setting the scale for a circular area which is entirely within a single node
// (that is, the node does not overlay more then a single node)
TEST_F(GraphFactoryTest, setMaxScaleInArea_Circle_area_within_single_node){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(1);
    graph_factory.setGraphTopLevelResolution(1);

    // Set the scale for a single circular area that is entirely within
    // the single top level node
    GraphFactory<nullptr_t>::Circle circle =
            GraphFactory<nullptr_t>::Circle(0.1, (Coordinates){0.1,0.1});
    graph_factory.setMaxScaleInArea(circle, 0.51);

    // Generate the graph
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get all the RealNodes
    std::function<bool(Node<nullptr_t>&)> always_true_filter = [&](Node<nullptr_t> &n) {
        return true;
    };
    std::vector<RealNode<nullptr_t>*> all_real_nodes =
            generated_graph.getAllNodesThatPassFilter(always_true_filter);

    // The area we choose should have forced the top level node to split into
    // 4 sub-nodes
    EXPECT_EQ(4, all_real_nodes.size());
    std::vector<Coordinates> expected_coordinates = {
            (Coordinates){0,0},
            (Coordinates){0.5,0},
            (Coordinates){0,0.5},
            (Coordinates){0.5,0.5},
    };
    std::vector<Coordinates> actual_coordinates;
    for (auto& node : all_real_nodes){
        actual_coordinates.emplace_back(node->getCoordinates());
    }
    // TODO: Add in GMock for unordered vector comparison
    // NOTE: List equality checks are *order-sensitive*. If it fails, check
    // that it's just not because the order has changed (which is fine)
    EXPECT_EQ(expected_coordinates, actual_coordinates);
}

// Test setting the scale of a circular area overlapping a single node
// (that is, the node does not overlay more then a single node)
TEST_F(GraphFactoryTest, setMaxScaleInArea_Circle_area_overlapping_single_node){
    GraphFactory<nullptr_t> graph_factory;
    // Create a 2x2 (# of nodes) graph
    graph_factory.setGraphScale(1.5);
    graph_factory.setGraphTopLevelResolution(3);

    // Set the scale for a single circular area that just overlaps the center node
    GraphFactory<nullptr_t>::Circle circle =
            GraphFactory<nullptr_t>::Circle(0.2, (Coordinates){0.4,0.4});
    graph_factory.setMaxScaleInArea(circle, 0.26);

    // Generate the graph
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get all the RealNodes
    std::function<bool(Node<nullptr_t>&)> always_true_filter = [&](Node<nullptr_t> &n) {
        return true;
    };
    std::vector<RealNode<nullptr_t>*> all_real_nodes =
            generated_graph.getAllNodesThatPassFilter(always_true_filter);

    // The area we choose should have forced the top level node to split into
    // 4 sub-nodes, so we now have the  top-level RealNodes, and the 4 within
    // our newly split node (3+4=7)
    EXPECT_EQ(7, all_real_nodes.size());

    // Check that the center node was the one that split
    Node<nullptr_t>* center_node = generated_graph.getSubNodes()[1][1];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*center_node));
}

// Test setting multiple rectangular and circular areas (with overlap)


// TODO: Area overlapping node at edge of graph

// TODO: Test setMaxScaleInArea (at least one test for every area type)
// TODO: Test generating a massive graph with lots of areas and points with increased resolution

// TODO: Is this even a valid test?
// TODO: Some performance tests - pretty performance sensitive component (but performance is dependent on the docker container CI is running in)
// Test generating a giant graph with lots of random shapes and resolutions
// (mostly for runtime testing)
TEST_F(GraphFactoryTest, generate_large_and_complex_graph){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(100);

    // Generate some rectangles
    //for (int i = 0; i < 20; i++){
    //    GraphFactory<nullptr_t>::Rectangle rectangle = GraphFactory<nullptr_t>::Rectangle(2*i,3*i,(Coordinates){i,i});
    //    graph_factory.setMaxScaleInArea(rectangle, 0.01);
    //}

    GraphFactory<nullptr_t>::Rectangle rectangle =
            GraphFactory<nullptr_t>::Rectangle(0,0,(Coordinates){0,0});
    rectangle = GraphFactory<nullptr_t>::Rectangle(10,10,(Coordinates){10,10});
    graph_factory.setMaxScaleInArea(rectangle, 1);
    rectangle = GraphFactory<nullptr_t>::Rectangle(10,10,(Coordinates){30,40});
    graph_factory.setMaxScaleInArea(rectangle, 1);
    rectangle = GraphFactory<nullptr_t>::Rectangle(30,10,(Coordinates){60,40});
    graph_factory.setMaxScaleInArea(rectangle, 1);
    rectangle = GraphFactory<nullptr_t>::Rectangle(10,60,(Coordinates){70,40});
    graph_factory.setMaxScaleInArea(rectangle, 0.5);
    rectangle = GraphFactory<nullptr_t>::Rectangle(5,10,(Coordinates){80,80});
    graph_factory.setMaxScaleInArea(rectangle, 0.5);


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto graph = graph_factory.createGraph();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time to generate graph = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;

    // Massive time dump, get all real nodes and all neighbours of all nodes
    std::function<bool(Node<nullptr_t>&)> filter = [&](Node<nullptr_t>& n) { return true; };

    std::vector<RealNode<nullptr_t>*> matching_nodes = graph.getAllNodesThatPassFilter(filter);

    std::cout << "We have " << matching_nodes.size() << " nodes" << std::endl;

    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < matching_nodes.size(); i++){
        matching_nodes[i]->getNeighbours();
        //std::cout << "found neighbour" << std::endl;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time to get all neighbours = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

