#include <gtest/gtest.h>
#include <RealNode.h>
#include <memory>

// TODO: Some note about how intertwined GraphNode and RealNode  are (and hence the all the tests of both are)

// TODO: Finish Tests
class RealNodeTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// TODO: Larger test case
TEST_F(RealNodeTest, getNeighbours_small_case){
    GraphNode<nullptr_t> graph_node(3,3);
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
}

