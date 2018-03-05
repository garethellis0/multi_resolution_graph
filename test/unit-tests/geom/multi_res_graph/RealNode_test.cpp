// Testing Includes
#include <gtest/gtest.h>

// C++ STD Includes
#include <memory>

// Thunderbots Includes
#include "geom/multi_res_graph/RealNode.h"

// Force the compiler to instantiate all the member functions for this template class
// (so that we can use things like the `[]` operator when debugging)
template class std::vector<RealNode<nullptr_t>*>;

// TODO: Some note about how intertwined GraphNode and RealNode  are (and hence the all the tests of both are)

namespace{

    class RealNodeTest : public testing::Test {
    protected:
        virtual void SetUp() {
        }
    };

    // TODO: Larger test case
    TEST_F(RealNodeTest, getNeighbours_small_case){
        GraphNode<nullptr_t> graph_node(2,2);
        std::vector<std::vector<Node<nullptr_t>*>> top_level_sub_nodes = graph_node.getSubNodes();

        // Expand the bottom left subnode
        graph_node.changeResolutionOfNode(top_level_sub_nodes[0][0],2);

        // Get the new top level subnodes
        top_level_sub_nodes = graph_node.getSubNodes();

        // Get the node we just expanded
        Node<nullptr_t>* expanded_node = top_level_sub_nodes[0][0];

        // The node we just expanded should now be GraphNode
        ASSERT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

        // Cast the expanded node to a GraphNode so we can do some more checks
        auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

        // Get the subnodes of the node we just expanded
        std::vector<std::vector<Node<nullptr_t>*>> expanded_node_sub_nodes =
                new_graph_node->getSubNodes();

        // TODO: All below cases should probably be their own test cases
        std::vector<RealNode<nullptr_t>*> neighbours;
        std::vector<RealNode<nullptr_t>*> expected;
        RealNode<nullptr_t>* real_node;

        // Find the neighbours of the bottom left node
        ASSERT_EQ(typeid(RealNode<nullptr_t>), typeid(*expanded_node_sub_nodes[0][0]));
        real_node = dynamic_cast<RealNode<nullptr_t>*>(expanded_node_sub_nodes[0][0]);
        neighbours = real_node->getNeighbours();
        // The bottom left node should only have two neighbours (above and to the right)
        EXPECT_EQ(2, neighbours.size());
        expected = {
                (RealNode<nullptr_t>*)expanded_node_sub_nodes[1][0],
                (RealNode<nullptr_t>*)expanded_node_sub_nodes[0][1]
        };
        // TODO: Add in GMock for unordered vector comparison
        // NOTE: This equality check is *order-sensitive*. If it fails, check
        // that it's just not because the order has changed (which is fine)
        EXPECT_EQ(expected, neighbours);
        // TODO: Check coordinates

        // Find the neighbours of 1 up (in the y direction) from the bottom left node
        ASSERT_EQ(typeid(RealNode<nullptr_t>), typeid(*expanded_node_sub_nodes[1][0]));
        real_node = dynamic_cast<RealNode<nullptr_t>*>(expanded_node_sub_nodes[1][0]);
        neighbours = real_node->getNeighbours();
        // We should have 3 neighbours (below, above, and the to right)
        // The top neighbour should be in the parent node
        EXPECT_EQ(3, neighbours.size());
        expected = {
                (RealNode<nullptr_t>*)expanded_node_sub_nodes[0][0],
                (RealNode<nullptr_t>*)top_level_sub_nodes[1][0],
                (RealNode<nullptr_t>*)expanded_node_sub_nodes[1][1]
        };
        // TODO: Add in GMock for unordered vector comparison
        // NOTE: This equality check is *order-sensitive*. If it fails, check
        // that it's just not because the order has changed (which is fine)
        EXPECT_EQ(expected, neighbours);
        // TODO: Check coordinates

        // Find the neighbours of the top right node OF the bottom left expandeded node
        ASSERT_EQ(typeid(RealNode<nullptr_t>), typeid(*expanded_node_sub_nodes[1][1]));
        real_node = dynamic_cast<RealNode<nullptr_t>*>(expanded_node_sub_nodes[1][1]);
        neighbours = real_node->getNeighbours();
        // We should have 4 neighbours:
        // the ones on the top and right should be in the super node
        // the ones on the bottom and left should be in the lower left expanded node
        EXPECT_EQ(4, neighbours.size());
        expected = {
                (RealNode<nullptr_t>*)expanded_node_sub_nodes[0][1],
                (RealNode<nullptr_t>*)top_level_sub_nodes[1][0],
                (RealNode<nullptr_t>*)expanded_node_sub_nodes[1][0],
                (RealNode<nullptr_t>*)top_level_sub_nodes[0][1]
        };
        // TODO: Add in GMock for unordered vector comparison
        // NOTE: This equality check is *order-sensitive*. If it fails, check
        // that it's just not because the order has changed (which is fine)
        EXPECT_EQ(expected, neighbours);
        // TODO: Check coordinates

    }

    TEST_F(RealNodeTest, get_and_set_containedValue) {
        // We must give a RealNode a GraphNode when we create it
        GraphNode<int> graph_node;
        RealNode<int> real_node(&graph_node);

        real_node.containedValue() = 39;

        EXPECT_EQ(39, real_node.containedValue());
    }
}

