#include <gtest/gtest.h>
#include <RealNode.h>

class GraphNodeTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(GraphNodeTest, constructor_with_scale){
    GraphNode<nullptr_t> graphNode(1, 0.3);
    EXPECT_EQ(1, graphNode.getResolution());
    EXPECT_EQ(0.3, graphNode.getScale());
}

TEST_F(GraphNodeTest, constructor_with_negative_scale){
    GraphNode<nullptr_t> graphNode(1, -0.3);
    EXPECT_EQ(1, graphNode.getResolution());
    EXPECT_EQ(0.3, graphNode.getScale());
}

TEST_F(GraphNodeTest, constructor_with_parent){
    GraphNode<nullptr_t>* parent = new GraphNode<nullptr_t>(2, 1);
    GraphNode<nullptr_t> graphNode(3, parent);
    EXPECT_EQ(3, graphNode.getResolution());
    EXPECT_EQ(0.5, graphNode.getScale());
}

// TODO: Do larger case (a few levels of nodes deep, and odd resolution and scale would be good)
TEST_F(GraphNodeTest, getCoordinatesOfNode_small_case){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();

    // Get a list of coordinates from the subnodes
    std::vector<Coordinates> coordinates;
    for (auto& row : sub_nodes){
        for (Node<nullptr_t>* node : row) {
            coordinates.push_back(graph_node.getCoordinatesOfNode(node));
        }
    }

    // Our GraphNode should be 2x2, so we should have 4 subnodes
    ASSERT_EQ(4, coordinates.size());

    // Sort the coordinates so this test is deterministic
    // (will always produce the same result, even if some implementation details change)
    std::sort(coordinates.begin(), coordinates.end(),
              [&](auto& c1, auto& c2){
                  return c1.x < c2.x && c1.y < c2.y;
              });

    std::vector<Coordinates> expected = {
            {0.25,0.25},
            {0.75,0.25},
            {0.25,0.75},
            {0.75,0.75},
    };

    // Sadly this seems like the nicest way to compare vectors of floating point
    // values in gtest :(
    for (int i = 0; i < 4; i++){
        EXPECT_FLOAT_EQ(expected[i].x, coordinates[i].x);
        EXPECT_FLOAT_EQ(expected[i].y, coordinates[i].y);
    }
}

// TODO: A larger case that's a few levels deep
TEST_F(GraphNodeTest, changeResolutionOfNode_small_case) {
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();

    // Choose an arbitrary node to expand
    Node<nullptr_t>* node_to_expand = sub_nodes[0][0];

    // Before we expand it, this node should be a RealNode
    EXPECT_EQ(typeid(RealNode<nullptr_t>), typeid(*node_to_expand));

    // Expand the node
    graph_node.changeResolutionOfNode(node_to_expand,3);

    // Get the subnodes again
    sub_nodes = graph_node.getSubNodes();
    Node<nullptr_t>* expanded_node = sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

    EXPECT_EQ(3, new_graph_node->getResolution());
    EXPECT_EQ(0.5, new_graph_node->getScale());
}

// TODO: A larger case that's a few levels deep
TEST_F(GraphNodeTest, changeResolutionOfClosestNode_small_case){
    GraphNode<nullptr_t> graph_node(2,1);

    // Expand the node
    graph_node.changeResolutionOfClosestNode({0.75,0.75}, 3);

    // Get the subnodes
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();
    Node<nullptr_t>* expanded_node = sub_nodes[1][1];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

    EXPECT_EQ(3, new_graph_node->getResolution());
    EXPECT_EQ(0.5, new_graph_node->getScale());
}

TEST_F(GraphNodeTest, getCoordinates_top_level_graphnode){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();

    // Check the coordinates of the top level node
    Coordinates expected_top_level_node_coordinates = {0.5,0.5};
    EXPECT_EQ(expected_top_level_node_coordinates, graph_node.getCoordinates());
}

TEST_F(GraphNodeTest, getCoordinates_for_subnodes){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();

    // Check the coordinates of subnodes
    std::vector<Coordinates> coordinates;
    for (auto& row : sub_nodes){
        for (Node<nullptr_t>* node : row){
            coordinates.emplace_back(node->getCoordinates());
        }
    }

    // Our GraphNode should be 2x2, so we should have 4 subnodes
    ASSERT_EQ(4, coordinates.size());

    // Sort the coordinates so this test is deterministic
    // (will always produce the same result, even if some implementation details change)
    std::sort(coordinates.begin(), coordinates.end(),
              [&](auto& c1, auto& c2){
                  return c1.x < c2.x && c1.y < c2.y;
              });

    std::vector<Coordinates> expected_subnode_coordinates = {
            {0.25,0.25},
            {0.75,0.25},
            {0.25,0.75},
            {0.75,0.75},
    };

    // Sadly this seems like the nicest way to compare vectors of floating point
    // values in gtest :(
    for (int i = 0; i < 4; i++){
        EXPECT_FLOAT_EQ(expected_subnode_coordinates[i].x, coordinates[i].x);
        EXPECT_FLOAT_EQ(expected_subnode_coordinates[i].y, coordinates[i].y);
    }
}

TEST_F(GraphNodeTest, getCoordinates_for_expanded_subnodes){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();

    // Expand one of the subnodes
    Node<nullptr_t>* expanded_sub_node = sub_nodes[0][0];
    graph_node.changeResolutionOfNode(expanded_sub_node, 3);

    // Get the new subnodes
    sub_nodes = graph_node.getSubNodes();
    expanded_sub_node = sub_nodes[0][0];

    Coordinates expected = {0.25, 0.25};
    EXPECT_EQ(expected, graph_node.getCoordinatesOfNode(expanded_sub_node));
}

// TODO: Larger case that's a few level deep
TEST_F(GraphNodeTest, getClosestNodeToCoordinates_small_case){
    GraphNode<nullptr_t> graph_node(4,10);
    std::vector<std::vector<Node<nullptr_t>*>> sub_nodes = graph_node.getSubNodes();

    // Expand one of the subnodes
    graph_node.changeResolutionOfNode(sub_nodes[0][0],2);

    boost::optional<RealNode<nullptr_t>*> found_node;
    Coordinates expected_coordinates;

    // For every one of the following coordinates:
    // - check that we actually got a result
    // - check that it was the node we were expecting
    // - check that the result has the coordinates we were expecting

    found_node = graph_node.getClosestNodeToCoordinates({0,0});
    ASSERT_TRUE(found_node.is_initialized());

    // TODO: YOU ARE HERE - finish this test
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

