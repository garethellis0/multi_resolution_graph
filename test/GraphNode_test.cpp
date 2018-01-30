#include <gtest/gtest.h>
#include <RealNode.h>

class GraphNodeTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(GraphNodeTest, constructor_with_scale){
    GraphNode graphNode(1, 0.3);
    EXPECT_EQ(1, graphNode.getResolution());
    EXPECT_EQ(0.3, graphNode.getScale());
}

TEST_F(GraphNodeTest, constructor_with_negative_scale){
    GraphNode graphNode(1, -0.3);
    EXPECT_EQ(1, graphNode.getResolution());
    EXPECT_EQ(0.3, graphNode.getScale());
}

TEST_F(GraphNodeTest, constructor_with_parent){
    GraphNode* parent = new GraphNode(2, 1);
    GraphNode graphNode(3, parent);
    EXPECT_EQ(3, graphNode.getResolution());
    EXPECT_EQ(0.5, graphNode.getScale());
}

TEST_F(GraphNodeTest, getClosestNodeThatPassesFilter_passes_filter_Test){

}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

