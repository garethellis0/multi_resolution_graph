#include <gtest/gtest.h>

#include "multi_resolution_graph/RealNode.h"
#include "multi_resolution_graph/Area.h"
#include "multi_resolution_graph/Rectangle.h"

// TODO: Some note about how intertwined GraphNode and RealNode  are (and hence the all the tests of both are)

using namespace multi_resolution_graph;

namespace {
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

    // Clean up
    delete parent;
}

TEST_F(GraphNodeTest, getScale_with_subnodes){
    GraphNode<nullptr_t> graph_node(2,1);

    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes = graph_node.getSubNodes();

    // Choose an arbitrary node to expand
    std::shared_ptr<Node<nullptr_t>> node_to_expand = sub_nodes[0][0];

    // Before we expand it, this node should be a RealNode
    EXPECT_EQ(typeid(RealNode<nullptr_t>), typeid(*node_to_expand));

    // Expand the node
    graph_node.changeResolutionOfNode(node_to_expand,2);

    // Get the subnodes again
    sub_nodes = graph_node.getSubNodes();
    std::shared_ptr<Node<nullptr_t>> expanded_node = sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    // Get the subnodes of our newly expanded node
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> expanded_node_sub_nodes =
            new_graph_node->getSubNodes();

    // Check the scale of the top level node
    EXPECT_EQ(1, graph_node.getScale());

    // Check the scale of the RealNodes directly below the top level (the ones we didn't expand)
    EXPECT_EQ(0.5, sub_nodes[1][0]->getScale());
    EXPECT_EQ(0.5, sub_nodes[1][1]->getScale());
    EXPECT_EQ(0.5, sub_nodes[0][1]->getScale());

    // Check the scale of the expanded node
    EXPECT_EQ(0.5, new_graph_node->getScale());

    // Check the scale of the subnodes beneath the newly expanded node
    EXPECT_EQ(0.25, expanded_node_sub_nodes[0][0]->getScale());
    EXPECT_EQ(0.25, expanded_node_sub_nodes[0][1]->getScale());
    EXPECT_EQ(0.25, expanded_node_sub_nodes[1][0]->getScale());
    EXPECT_EQ(0.25, expanded_node_sub_nodes[1][1]->getScale());

}

// TODO: Do larger case (a few levels of nodes deep, and odd resolution and scale would be good)
TEST_F(GraphNodeTest, getCoordinatesOfNode_small_case){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes =
            graph_node.getSubNodes();

    // Get a list of coordinates from the subnodes
    std::vector<Coordinates> coordinates;
    for (auto& row : sub_nodes){
        for (std::shared_ptr<Node<nullptr_t>> node : row) {
            coordinates.push_back(graph_node.getCoordinatesOfNode(node));
        }
    }

    // Our GraphNode should be 2x2, so we should have 4 subnodes
    ASSERT_EQ(4, coordinates.size());

    // Sort the coordinates so this test is deterministic
    // (will always produce the same result, even if some implementation details change)
    std::sort(coordinates.begin(), coordinates.end(),
              [&](Coordinates& c1, Coordinates& c2){
                  return c1.x < c2.x && c1.y < c2.y;
              });

    std::vector<Coordinates> expected = {
            {0,0},
            {0.5,0},
            {0,0.5},
            {0.5,0.5},
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
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes = graph_node.getSubNodes();

    // Choose an arbitrary node to expand
    std::shared_ptr<Node<nullptr_t>> node_to_expand = sub_nodes[0][0];

    // Before we expand it, this node should be a RealNode
    EXPECT_EQ(typeid(RealNode<nullptr_t>), typeid(*node_to_expand));

    // Expand the node
    graph_node.changeResolutionOfNode(node_to_expand,3);

    // Get the subnodes again
    sub_nodes = graph_node.getSubNodes();
    std::shared_ptr<Node<nullptr_t>> expanded_node = sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    EXPECT_EQ(3, new_graph_node->getResolution());
    EXPECT_EQ(0.5, new_graph_node->getScale());
}

// TODO: A larger case that's a few levels deep
TEST_F(GraphNodeTest, changeResolutionOfClosestNode_small_case){
    GraphNode<nullptr_t> graph_node(2,1);

    // Expand the node
    graph_node.changeResolutionOfClosestNode({0.5,0.5}, 3);

    // Get the subnodes
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes = graph_node.getSubNodes();
    std::shared_ptr<Node<nullptr_t>> expanded_node = sub_nodes[1][1];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    EXPECT_EQ(3, new_graph_node->getResolution());
    EXPECT_EQ(0.5, new_graph_node->getScale());
}

TEST_F(GraphNodeTest, getCoordinates_top_level_graphnode){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes = graph_node.getSubNodes();

    // Check the coordinates of the top level node
    Coordinates expected_top_level_node_coordinates = {0,0};
    EXPECT_EQ(expected_top_level_node_coordinates, graph_node.getCoordinates());
}

TEST_F(GraphNodeTest, getCoordinates_for_subnodes){
    GraphNode<nullptr_t> graph_node(2,1);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes = graph_node.getSubNodes();

    // Check the coordinates of subnodes
    std::vector<Coordinates> coordinates;
    for (auto& row : sub_nodes){
        for (std::shared_ptr<Node<nullptr_t>> node : row){
            coordinates.emplace_back(node->getCoordinates());
        }
    }

    // Our GraphNode should be 2x2, so we should have 4 subnodes
    ASSERT_EQ(4, coordinates.size());

    // Sort the coordinates so this test is deterministic
    // (will always produce the same result, even if some implementation details change)
    std::sort(coordinates.begin(), coordinates.end(),
              [&](Coordinates& c1, Coordinates& c2){
                  return c1.x < c2.x && c1.y < c2.y;
              });

    std::vector<Coordinates> expected_subnode_coordinates = {
            {0,0},
            {0.5,0},
            {0,0.5},
            {0.5,0.5},
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
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> sub_nodes =
            graph_node.getSubNodes();

    // Expand one of the subnodes
    std::shared_ptr<Node<nullptr_t>> expanded_sub_node = sub_nodes[0][0];
    graph_node.changeResolutionOfNode(expanded_sub_node, 3);

    // Get the new subnodes
    sub_nodes = graph_node.getSubNodes();
    expanded_sub_node = sub_nodes[0][0];

    Coordinates expected = {0, 0};
    EXPECT_EQ(expected, graph_node.getCoordinatesOfNode(expanded_sub_node));
}

// This test is more of a higher level integration test. If it's failing,
// it's likely other, smaller/more focused tests are also failing
TEST_F(GraphNodeTest, getClosestNodeToCoordinates_small_case){
    GraphNode<nullptr_t> graph_node(4,8);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> top_level_sub_nodes = graph_node.getSubNodes();

    // Expand the bottom left subnode
    graph_node.changeResolutionOfNode(top_level_sub_nodes[0][0],2);

    // Get the new top level subnodes
    top_level_sub_nodes = graph_node.getSubNodes();

    // Get the node we just expanded
    std::shared_ptr<Node<nullptr_t>> expanded_node = top_level_sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    // Get the subnodes of the node we just expanded
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> expanded_node_sub_nodes =
            new_graph_node->getSubNodes();

    std::optional<std::shared_ptr<RealNode<nullptr_t>>> found_node;
    Coordinates expected_coordinates;

    // TODO: These should def. be seperate tests
    // For every one of the following coordinates:
    // - check that we actually got a result
    // - check that it was the node we were expecting
    // - check that the result has the coordinates we were expecting

    found_node = graph_node.getClosestNodeToCoordinates({0,0});
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(expanded_node_sub_nodes[0][0], *found_node);
    expected_coordinates = {0, 0};
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());

    found_node = graph_node.getClosestNodeToCoordinates({0.9,0});
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(expanded_node_sub_nodes[0][1], *found_node);
    expected_coordinates = {1, 0};
    Coordinates actual = (*found_node)->getCoordinates();
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());

    found_node = graph_node.getClosestNodeToCoordinates({0,1.1});
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(expanded_node_sub_nodes[1][0], *found_node);
    expected_coordinates = {0, 1};
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());

    found_node = graph_node.getClosestNodeToCoordinates({4.4,6.6});
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(top_level_sub_nodes[3][2], *found_node);
    expected_coordinates = {4, 6};
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());
}

// This test is more of a higher level integration test. If it's failing,
// it's likely other, smaller/more focused tests are also failing
TEST_F(GraphNodeTest, getClosestNodeToCoordinatesThatPassesFilter_small_case){
    GraphNode<nullptr_t> graph_node(4,8);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> top_level_sub_nodes =
            graph_node.getSubNodes();

    // Expand the bottom left subnode
    graph_node.changeResolutionOfNode(top_level_sub_nodes[0][0],2);

    // Get the new top level subnodes
    top_level_sub_nodes = graph_node.getSubNodes();

    // Get the node we just expanded
    std::shared_ptr<Node<nullptr_t>> expanded_node = top_level_sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    // Get the subnodes of the node we just expanded
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> expanded_node_sub_nodes =
            new_graph_node->getSubNodes();

    std::optional<std::shared_ptr<RealNode<nullptr_t>>> found_node;
    Coordinates expected_coordinates;
    std::function<bool(Node<nullptr_t>&)> filter;

    // TODO: These should def. be seperate tests
    // For every one of the following coordinates:
    // - check that we actually got a result
    // - check that it was the node we were expecting
    // - check that the result has the coordinates we were expecting

    // This filter forces us to find a node with specific coordinates
    expected_coordinates = {4,6};
    filter = [&](Node<nullptr_t> &n) { return n.getCoordinates() == expected_coordinates; };
    found_node = graph_node.getClosestNodeToCoordinatesThatPassesFilter({0, 0}, filter);
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(top_level_sub_nodes[3][2], *found_node);
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());

    // This filter forces us to find a node in the top 1/2 of the graph
    expected_coordinates = {2,4};
    filter = [&](Node<nullptr_t> &n) { return n.getCoordinates().y > 3; };
    found_node = graph_node.getClosestNodeToCoordinatesThatPassesFilter({3, 0},
                                                                        filter,
                                                                        false);
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(top_level_sub_nodes[2][1], *found_node);
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());

    // This filter forces us to find a node in the bottom left 1/4 of the graph
    expected_coordinates = {2,2};
    filter = [&](Node<nullptr_t> &n) {
        return n.getCoordinates().y < 4 && n.getCoordinates().x < 4;
    };
    found_node = graph_node.getClosestNodeToCoordinatesThatPassesFilter({7, 7},
                                                                        filter,
                                                                        false);
    ASSERT_TRUE(found_node.has_value());
    EXPECT_EQ(top_level_sub_nodes[1][1], *found_node);
    EXPECT_EQ(expected_coordinates, (*found_node)->getCoordinates());
}

TEST_F(GraphNodeTest, getAllNodesThatPassFilter_small_case){
    GraphNode<nullptr_t> graph_node(4,8);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> top_level_sub_nodes =
            graph_node.getSubNodes();

    // Expand the bottom left subnode
    graph_node.changeResolutionOfNode(top_level_sub_nodes[0][0],2);

    // Get the new top level subnodes
    top_level_sub_nodes = graph_node.getSubNodes();

    // Get the node we just expanded
    std::shared_ptr<Node<nullptr_t>> expanded_node = top_level_sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    // Get the subnodes of the node we just expanded
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> expanded_node_sub_nodes =
            new_graph_node->getSubNodes();

    std::vector<std::shared_ptr<RealNode<nullptr_t>>> found_nodes;
    std::vector<std::shared_ptr<RealNode<nullptr_t>>> expected_nodes;
    std::function<bool(Node<nullptr_t>&)> filter;

    // TODO: These should def. be seperate tests
    // For every one of the following coordinates:
    // - check that we got the right number of nodes
    // - check that it was the nodes we were expecting
    // TODO: Add in GMock for unordered vector comparison
    // NOTE: List equality checks are *order-sensitive*. If it fails, check
    // that it's just not because the order has changed (which is fine)

    // This filter forces us to find a node with specific coordinates
    expected_nodes = {
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][2])
    };
    filter = [&](Node<nullptr_t> &n) { return n.getCoordinates() == (Coordinates){4,6}; };
    found_nodes = graph_node.getAllNodesThatPassFilter(filter);
    EXPECT_EQ(found_nodes.size(), 1);
    EXPECT_EQ(expected_nodes, found_nodes);


    // This filter forces us to find nodes in the top 1/2 of the graph
    filter = [&](Node<nullptr_t> &n) { return n.getCoordinates().y > 3; };
    expected_nodes = {
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][2]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][3]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][2]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][3]),
    };
    found_nodes = graph_node.getAllNodesThatPassFilter(filter);
    EXPECT_EQ(found_nodes.size(), 8);
    EXPECT_EQ(expected_nodes, found_nodes);

    // This filter forces us to find a node in the bottom left 1/4 of the graph
    filter = [&](Node<nullptr_t> &n) {
        return n.getCoordinates().y < 4 && n.getCoordinates().x < 4;
    };
    expected_nodes = {
            // All the RealNodes under the expanded node
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[0][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[0][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[1][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[1][1]),
            // All the top level RealNodes in the bottom left 1/4
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[0][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[1][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[1][1]),
    };
    found_nodes = graph_node.getAllNodesThatPassFilter(filter);
    EXPECT_EQ(found_nodes.size(), 7);
    EXPECT_EQ(expected_nodes, found_nodes);
}

TEST_F(GraphNodeTest, getAllNodesInArea_small_case){
    GraphNode<nullptr_t> graph_node(4,8.0);
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> top_level_sub_nodes =
            graph_node.getSubNodes();

    // Expand the bottom left subnode
    graph_node.changeResolutionOfNode(top_level_sub_nodes[0][0],2);

    // Get the new top level subnodes
    top_level_sub_nodes = graph_node.getSubNodes();

    // Get the node we just expanded
    std::shared_ptr<Node<nullptr_t>> expanded_node = top_level_sub_nodes[0][0];

    // The node we just expanded should now be GraphNode
    ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node.get());

    // Get the subnodes of the node we just expanded
    std::vector<std::vector<std::shared_ptr<Node<nullptr_t>>>> expanded_node_sub_nodes =
            new_graph_node->getSubNodes();

    std::vector<std::shared_ptr<RealNode<nullptr_t>>> found_nodes;
    std::vector<std::shared_ptr<RealNode<nullptr_t>>> expected_nodes;

    // TODO: These should def. be seperate tests
    // For every one of the following coordinates:
    // - check that we got the right number of nodes
    // - check that it was the nodes we were expecting
    // TODO: Add in GMock for unordered vector comparison
    // NOTE: List equality checks are *order-sensitive*. If it fails, check
    // that it's just not because the order has changed (which is fine)

    // This area forces us to find nodes in the top 1/2 of the graph
    Rectangle<nullptr_t> area(8, 4, {0,4.1});
    expected_nodes = {
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][2]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[2][3]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][2]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[3][3]),
    };
    found_nodes = graph_node.getAllNodesInArea(area);
    EXPECT_EQ(found_nodes.size(), 8);
    EXPECT_EQ(expected_nodes, found_nodes);

    // This area forces us to find a node in the bottom left 1/4 of the graph
    area = Rectangle<nullptr_t>(3.9, 3.9, {0,0});
    expected_nodes = {
            // All the RealNodes under the expanded node
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[0][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[0][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[1][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(expanded_node_sub_nodes[1][1]),
            // All the top level RealNodes in the bottom left 1/4
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[0][1]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[1][0]),
            std::dynamic_pointer_cast<RealNode<nullptr_t>>(top_level_sub_nodes[1][1]),
    };
    found_nodes = graph_node.getAllNodesInArea(area);
    EXPECT_EQ(found_nodes.size(), 7);
    EXPECT_EQ(expected_nodes, found_nodes);
}


// TODO: Test conditions that would cause functions to throw exceptions (which GraphNode *DOES*)

}

